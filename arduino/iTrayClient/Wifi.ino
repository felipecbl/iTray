
//************************************
// Substrings from the Server
//************************************
const String current_angle = "\"current_angle\":\"";
const String tray_connected = "\"tray_connected\":\"";
const String user_connected = "\"user_connected\":\"";
const String last_angle = "\"last_angle\":\"";


//************************************
// Temporary Wifi credentials
//************************************
const char* ssid     = "White Castle";
const char* password = "castel0branc0";

//Server URL (Domain)
const char* host = "www.itray.website";

//************************************
// Wifi functions
//************************************

void initWifi(){
  
  // We start by connecting to a WiFi network
  log("Connecting to ");
  logln(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

void getWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    log(".");
  }

  logln("");
  logln("WiFi connected");
  logln("IP address: ");
//  logln(String(WiFi.localIP()));
  Serial.println(WiFi.localIP());
}

void connectToServer(){
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    logln("connection failed");
    digitalWrite(2, LOW);
    return;
  }

  if (client.connect(host, 80)) {

    log("Connected to: ");
    logln(host);

    String postString = " /getWheel.php?json";

    log("Requesting URL: ");
    logln(postString);

    // This will send the request to the server
    client.print(String("GET ") + postString + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(500);

    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      String line = client.readStringUntil('\r');
      log(line);

      if (line.indexOf("\"success\":true") != -1) {
        logln("Success!!");

        log("Current angle: ");
        logln(getValue(line, current_angle, "\""));

//        log("Current Tray Connected: ");
//        logln(getValue(line, tray_connected, "\""));
//
//        log("Current User Connected: ");
//        logln(getValue(line, user_connected, "\""));
//
//        log("Current last angle: ");
//        logln(getValue(line, last_angle, "\""));

        remotePosition = getValue(line, current_angle, "\"").toInt();
        // Convert 0 to 360
        if(remotePosition == 0) remotePosition = 360;
        
        // if (trayPosition != remotePosition) {
        if (!inTheRange()){
          rotate();
        }
      }
    }

    logln("closing connection");
  }
}

