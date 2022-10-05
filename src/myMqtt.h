//--------------- MQTT declarations ---------------
#ifndef mqttD
#define mqttD

#include <Kaywinnet.h>          // WiFi credentials
#include <PubSubClient.h>       // connect to a MQTT broker and publish/subscribe messages in topics.

// Declare an object of class WiFiClient, which allows to establish a connection to a specific IP and port
// Declare an object of class PubSubClient, which receives as input the constructor previously defined with WiFiClient.
// The constructor MUST be unique on the network. (Does it?)
WiFiClient xyzzy;
PubSubClient client(xyzzy);

const char *mqttServer = MQTT_SERVER;         // Local broker defined in Kaywinnet.h
const int mqttPort = MQTT_PORT;

// Declare strings for the topics. Topics will be created in setup_mqtt().
char statusTopic[20];
char cmndTopic[20];
char rssiTopic[20];

namespace myMqtt {
//--------------- Prototypes ---------------
void createTopics();
void mqttConnect();
void callback(String topic, byte * payload, unsigned int length);



// ==================================  setup_mqtt ==================================

void setup_mqtt() {
  //MUST follow setupWiFi()
  // Call the setServer method on the PubSubClient object
  client.setServer(mqttServer, mqttPort);
  createTopics();
  mqttConnect();
}



// ==================================  mqttConnect ==================================
void mqttConnect() {
  client.setServer(mqttServer, mqttPort); //Call the setServer method
  while (!client.connected()) {
    Serial.print(F("MQTT connecting..."));
    if (client.connect(hostName)) {
      Serial.println(F("connected"));

      client.setCallback(callback);

      //Subscriptions:
      client.subscribe(cmndTopic);
      Serial.print(F("Subscribing to "));
      Serial.println(cmndTopic);
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F("- trying again in 5-seconds."));
      delay(5000);
    }
  }
}


// ==================================  mqttReconnect ==================================
void mqttReconnect() {
  //Make sure we stay connected to the mqtt broker
  if (!client.connected()) {
    mqttConnect();
  }
  if (!client.loop()) {
    client.connect(hostName);
    Serial.println(F("Should not get here"));
  }
}


// ==================================  createTopics ==================================
void createTopics() {

  // Make sure we have a nodename.
#ifndef nodeNameD
#define nodeNameD
  char nodeName[] = SKETCH;
#endif

  strcpy(cmndTopic, nodeName);            //nodeName is defined in the wifi tab.
  strcat(cmndTopic, "/cmnd");             //Incoming commands, payload is a command.
  strcpy(statusTopic, nodeName);
  strcat(statusTopic, "/stat");
  strcpy(rssiTopic, nodeName);
  strcat(rssiTopic, "/rssi");

  myFunctions::dbugs("cmndTopic= ", cmndTopic);
  myFunctions::dbugs("statusTopic= ", statusTopic);
  myFunctions::dbugs("rssiTopic= ", rssiTopic);
  Serial.println();

}




// ==================================  mqtt callback ==================================
// This function is executed when some device publishes a message to a topic that this ESP8266 is subscribed to.
// The MQTT payload is the filename of the message to play when the phone is picked up.  The payload is case-sensitive.
//
void callback(String topic, byte * payload, unsigned int length) {
  char message[length + 1];

  // copy contents of payload to message
  // convert the payload from a byte array to a char array
  memcpy(message, payload, length);
  message[length] = '\0';                 // add NULL terminator to message

  // Sometimes in the MQTT tool, I accdentally hit "Enter" on my keyboard.
  // This removes it.
  for (size_t i = 0; i == strlen(message); i++) {
    if (message[i] == 10) {
      message[i] = '\0';
      break;
    }
  }

  // Change to lower-case
  for (int i = 0; message [i]; i++ )
  {
    //message[i] &= 0b11011111; //Upper case- clear bit 5
    message[i] |= 0b00100000; //Lower case- set bit 5
  }

  Serial.println();
  Serial.println();
  Serial.print(F("Message arrived on topic: "));
  Serial.print(topic);
  Serial.println(F("."));

  Serial.print("message: ");
  Serial.println(message);
  Serial.print(F("Length= "));
  Serial.print(strlen(message));
  Serial.println();

  // If the message terminates in a line-feed, make it the terminating null char.
  int j = strlen(message) - 1;
  if (message[j] == 10) message[j] = '\0';


  // --------- Process incoming commands ---------
  if (topic == cmndTopic) {
    Serial.print(F("received message on cmdTopic: '"));
    Serial.print(message);
    Serial.println(F("'"));

    if (!strcmp(message, "wag")) {
      Serial.println(F("wag 1"));
      //wag(1);
    }

    if (!strcmp(message, "twitch")) {     // if message=="twitch", then twitch the tail
      Serial.println(F("twitch"));
      //twitch(1);
    }

    if (!strcmp(message, "largetwitch")) {
      Serial.println(F("twitch"));
      //largeTwitch(1);
    }

    if (!strcmp(message, "slowwag")) {    // SLow-wag the tail
      Serial.println(F("slowWag"));
      //slowWag(1);
    }

    if (!strcmp(message, "rssi")) {       // Publish the WiFi RSSI
      Serial.print(F("Publishing WiFi RSSI, "));
      int myRssi = WiFi.RSSI();
      itoa(myRssi, rssiBuffer, 10);

      Serial.print(F("rssiBuffer= "));
      Serial.println(rssiBuffer);
      client.publish(rssiTopic, rssiBuffer);
    }

    if (!strcmp(message, "help")) {
      //Print the available commands
      Serial.println(F("Commands:"));
      Serial.println(F("wag -    Wag the tail once."));
      Serial.println(F("slowWag - Slow-wag the tail once."));
      Serial.println(F("twitch - Twitch the tail once."));
      Serial.println(F("largeTwitch - Twitch the tail once."));
    }
  }

} //callback
} //Namespace
#endif
