/*
 * By Felipe Castelo Branco
 * June 2015
 * Vancouver BC
 */

#include <ESP8266WiFi.h>
#include <Servo.h>

#define DEBUG

void setup() {
  Serial.begin(115200);

  delay(10);
  initWifi();
  getWiFi();
  initServo();
  listenEncoder();
}

void loop() {
  getWiFi();
  connectToServer();
}
