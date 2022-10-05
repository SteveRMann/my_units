#include "ESP8266WiFi.h"

#ifndef kaywinnet
#include <Kaywinnet.h>          // WiFi credentials
#endif

#ifndef hostD
#define hostD
char hostNamePrefix[] = "Xyzzy-";  //Define a unique host prefix.
char hostName[24];                 //Holds hostNamePrefix + the last three bytes of the MAC address.
#endif

#ifndef ipBufferD
#define ipBufferD
char ipBuffer[24];                 //IP address in a string
char rssiBuffer[6];                //RSSI in a string
#endif

// =========================================================================================
namespace myWiFi {

//Prototypes:
void makeHostname();

void setup_wifi() {
  int tryCount = 0;                 //Count the attempts to connect

  Serial.println(F("\n"));
  Serial.println(F("Connecting to WiFi"));

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

  WiFi.mode(WIFI_STA);
  makeHostname();
  WiFi.hostname(hostName);
  WiFi.begin(MY_SSID, MY_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(WiFi.status()); Serial.print(F(" "));
    tryCount += 1;
    if (tryCount % 10 == 0)Serial.println();
#ifdef LEDPIN
    //Blink an LED while connecting
    digitalWrite(ledPin, !digitalRead(ledPin));
#endif
  }
  Serial.println(F("\nWiFi connected."));
  Serial.print(F("WiFi.SSID: "));
  Serial.println(WiFi.SSID());
  Serial.print(F("WiFi.macAddress: "));
  Serial.println(WiFi.macAddress());
  Serial.print(F("WiFi.localIP: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("WiFi.hostname: "));
  Serial.println(WiFi.hostname());

  long rssi = WiFi.RSSI();
  Serial.print("WiFi.RSSI: ");
  Serial.println(rssi);

  sprintf(ipBuffer, "\n%s", WiFi.localIP().toString().c_str());
  Serial.print(F("myWiFi::ipBuffer: "));
  Serial.println(ipBuffer);
  Serial.print(F("hostName = \""));
  Serial.print(hostName);

  //////// Show rssiBuffer
  int16_t myRssi = WiFi.RSSI();
//  sprintf(rssiBuffer, "%s", myRssi.toString().c_str());
  Serial.print(F("RSSI= "));
  Serial.println(myRssi);

  Serial.println(F("\""));

  /* WiFi status codes
      WL_IDLE_STATUS = 0,
      WL_NO_SSID_AVAIL = 1,
      WL_SCAN_COMPLETED = 2,
      WL_CONNECTED = 3,
      WL_CONNECT_FAILED = 4,
      WL_CONNECTION_LOST = 5,
      WL_WRONG_PASSWORD = 6,
      WL_DISCONNECTED = 7
  */
}


// --------------- makeHostname ---------------
void makeHostname() {
  byte mac[6];                      //MAC address of your Wifi shield
  char macBuffer[24];       // Holds the last three digits of the MAC, in hex.

  // Get the last three numbers of the mac address.
  // "4C:11:AE:0D:83:86" becomes "0D8386" in macBuffer.
  WiFi.macAddress(mac);
  snprintf(macBuffer, sizeof(macBuffer), "%02X%02X%02X", mac[3], mac[4], mac[5]);

  // Build hostNamePrefix + last three bytes of the MAC address.
  strcpy(hostName, hostNamePrefix);
  strcat(hostName, macBuffer);
}

} //End of namespace
