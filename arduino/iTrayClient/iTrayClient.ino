/*
 * By Felipe Castelo Branco
 * June 2015
 * Vancouver BC
 */

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

// Servo library
#include <Servo.h>


#include <EEPROM.h>

//Debug mode on/off
#define DEBUG

void setup() {
  Serial.begin(115200);
//  EEPROM.begin(512); //uncomment

//  initServo(); //uncomment
//  listenEncoder(); //uncomment

//  delay(10); //uncomment
  initWifi();
//  getWiFi();
}

void loop() {
//  getWiFi();
//  connectToServer();

  processCaptive();

}
