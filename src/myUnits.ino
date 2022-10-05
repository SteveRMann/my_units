/*
   This program tests my various functions
*/

#define SKETCH "myUnits"
#define VERSION "1.00"

#include "myFunctions.h"
#include "myOTA.h"
#include "myWiFi.h"
#include "myWifiMulti.h"
#include "myMqtt.h"


//****************** SETUP ******************
void setup() {
  myFunctions::beginSerial();

  //Only one
  //myWiFi::setup_wifi();
  myWifiMulti::setup_wifiMulti();

  myOTA::start_OTA();
  myMqtt::setup_mqtt();

}


//****************** LOOP *********************
void loop() {
  ArduinoOTA.handle();
  myMqtt::mqttReconnect();          // Make sure we stay connected to the mqtt broker
}
