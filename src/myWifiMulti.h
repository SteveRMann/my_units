//--------------- WiFiMulti declarations ---------------
#ifndef kaywinnet
#include <Kaywinnet.h>          // WiFi credentials
#endif

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#ifndef hostD
#define hostD
char hostNamePrefix[] = SKETCH;   //Define a unique host prefix.
char hostName[24];                //Holds hostNamePrefix + the last three bytes of the MAC address.
#endif

#ifndef ipBufferD
#define ipBufferD
char ipBuffer[24];                //IP address in a string
char rssiBuffer[6];               //RSSI in a string
#endif

#ifndef nodeNameD
#define nodeNameD
char nodeName[] = SKETCH;
#endif



// ========================= Namespace myWiFiMulti =========================
namespace myWifiMulti {

//Prototypes:
void makeHostname();
void wifiConnected();

void setup_wifiMulti() {
  Serial.println(F("Connecting to WiFi"));

  // makeHostname();
  // WiFi.hostname(hostName);          //NOTE, this does not work in the WifiMulti.h library.

  // WiFi connect timeout per AP. Increase when connecting takes longer.
  const uint32_t connectTimeoutMs = 5000;

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Register multi WiFi networks
  wifiMulti.addAP(MYSSID, MY_PASSWORD);
  wifiMulti.addAP("Naboo", NABOO_PASSWORD);
  wifiMulti.addAP("Serenity", SERENITY_PASSWORD);

  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
    wifiConnected();
  }
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


// --------------- WiFiConnected ---------------
// Print the connection details.
void wifiConnected() {
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

  sprintf(ipBuffer, "%s", WiFi.localIP().toString().c_str());
  Serial.print(F("myWiFi::ipBuffer= "));
  Serial.println(ipBuffer);
  Serial.print(F("hostName= \""));
  Serial.print(hostName);
  Serial.println(F("\""));
  Serial.println();
}
} //End of namespace
