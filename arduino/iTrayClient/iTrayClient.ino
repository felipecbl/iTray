/*
 * By Felipe Castelo Branco
 * June 2015
 * Vancouver BC
 */

#include <ESP8266WiFi.h>
#include <Servo.h>

Servo myservo;
int pos = 0;
int trayPos = 0;

const String current_angle = "\"current_angle\":\"";
const String tray_connected = "\"tray_connected\":\"";
const String user_connected = "\"user_connected\":\"";
const String last_angle = "\"last_angle\":\"";

const char* ssid     = "White Castle";
const char* password = "castel0branc0";

//const char* host = "data.sparkfun.com";
const char* host = "www.itray.website";

void getWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, HIGH);
    delay(500);
    Serial.print(".");
    digitalWrite(2, LOW);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  getWiFi();
  myservo.attach(14);

}

void loop() {
  getWiFi();
  //  delay(2000);
  digitalWrite(2, HIGH);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    digitalWrite(2, LOW);
    return;
  }

  int trayAngle = 45;
  if (client.connect(host, 80)) {

    Serial.print("Connected to: ");
    Serial.println(host);

    String postString = " /getWheel.php?json";
    //    postString += "?angle=";
    //    postString += trayAngle;
    //    postString += "&json" ;

    Serial.print("Requesting URL: ");
    Serial.println(postString);

    // This will send the request to the server
    client.print(String("GET ") + postString + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(500);

    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);

      if (line.indexOf("\"success\":true") != -1) {
        Serial.println("Success!!");

        Serial.print("Current angle: ");
        Serial.println(getValue(line, current_angle, "\""));

        Serial.print("Current Tray Connected: ");
        Serial.println(getValue(line, tray_connected, "\""));

        Serial.print("Current User Connected: ");
        Serial.println(getValue(line, user_connected, "\""));

        Serial.print("Current last angle: ");
        Serial.println(getValue(line, last_angle, "\""));

        trayPos = getValue(line, current_angle, "\"").toInt();

        if (pos != trayPos) {
          Serial.println("New values: ");
          Serial.print("Old Position: ");
          Serial.println(pos);
          Serial.print("New tray position: ");
          Serial.println(trayPos);

          rotate();
        }
      }
    }

    Serial.println();
    Serial.println("closing connection");
  }
}

boolean isBigger(int firstNumber, int secondNumber) {
  if (firstNumber > secondNumber) {
    return true;
  }
  return false;
}

void rotate() {
  if (isBigger(trayPos, pos)) {
    int diff = (trayPos - pos) / 10;
    Serial.print("Spinning time: ");
    Serial.println(diff);
    for (pos = 90; pos <= 180; pos += 1) {
      myservo.write(pos);
      delay(diff);
    }
  } else {
    int diff = (pos - trayPos) / 10;
    Serial.print("Spinning time: ");
    Serial.println(diff);
    for (pos = 90; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(diff);
    }
  }
  myservo.write(90);
  pos = trayPos;
}

String getValue(String webString, String startString, String endString) {
  if (webString.indexOf(startString) != -1) {
    int sStart = webString.indexOf(startString) + startString.length();
    int sEnd = webString.indexOf(endString, sStart);

    return  webString.substring(sStart, sEnd);
  }
  return "";
}

