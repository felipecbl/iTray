/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "White Castle";
const char* password = "castel0branc0";

//const char* host = "data.sparkfun.com";
const char* host = "itray.website";

void setup() {
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
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

//int value = 0;

void loop() {
  delay(2000);
//  ++value;
  digitalWrite(2, HIGH);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    digitalWrite(2, LOW);
//    return;
  }

  int trayAngle = 45;
  if (client.connect(host, 80)) {

    Serial.print("Connected to: ");
    Serial.println(host);

    String postString = " /getWheel.php";
//    postString += "?angle=";
//    postString += trayAngle;
//    postString += "&json" ;


    client.print(String("GET ") + postString + "HTTP/1.1\n"); 
//    client.print("Host: buyatab.com\n"); 
//     client.print("Connection: close\n"); 
//    client.print("Content-Type: text/html\n"); 
//    client.print("Content-Length: "); 
//    client.print(postString.length());  
//    client.print("\n\n"); 
//    client.print(postString);   

client.print("Accept:  text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
client.print("Accept-Encoding: gzip, deflate");
client.print("Accept-Language: en-US,en;q=0.5");
client.print("Cache-Control: max-age=0");
client.print("Connection:  keep-alive");
client.print("Host:  itray.website");

    
    Serial.print("Requesting URL: ");
    Serial.println(postString);
    
    // This will send the request to the server
  //  client.print(String("GET ") + url + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n"); 
  //               "Host: " + host + "\r\n\r\n" + "Connection: close\r\n\r\n");
    
    // Read all the lines of the reply from server and print them to Serial
  //  while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.println(line);

  //  }
    
    Serial.println();
    Serial.println("closing connection");
  }
}

