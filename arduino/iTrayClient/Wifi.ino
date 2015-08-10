//************************************
// Captive Server Settings
//************************************

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML;

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
//const char* ssid     = "White Castle";
//const char* password = "castel0branc0";

char ssid[32];
char password[64];

//Server URL (Domain)
const char* host = "www.itray.website";
int wifiAtempt = 0;

//************************************
// Wifi functions
//************************************

void initWifi(){

  //    setNetworkId("My Id");
  //    setNetworkPassword("My Password");


  // Check eeprom
  logln("Checking EEPROM...");

  getNetworkId().toCharArray(ssid, 32);
  getNetworkPassword().toCharArray(password, 64);

  if(getNetworkId() != NULL){
    logln("Retrieving Network info from EEPROM...");
    log("EEPROM Network Name: ");
    logln(ssid);
    log("EEPROM Network Password: ");
    logln(password);
    
    //Try to connect using EEPROM credentials
    logln("Connecting...");    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    getWiFi();
      
  }else{
    logln("No network info found on EEPROM.");
    // Set ESP to Access point
    accessPoint();
  }  
}

void getWiFi() {
  wifiAtempt++;
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {
    timeout++;
    delay(500);
    log(".");

    if(timeout >= 20){ //10 seconds enough?
      logln("Connection timeout");
      accessPoint();
    }
  }

  logln("");
  logln("WiFi connected");
  logln("IP address: ");
//  logln(String(WiFi.localIP()));
  Serial.println(WiFi.localIP());
}

void processCaptive(){
  dnsServer.processNextRequest();
  webServer.handleClient();
}

void accessPoint(){ 
  Serial.println("Starting Access Point mode (iTray Network).");   
  WiFi.mode(WIFI_AP);
  
  Serial.println("Setting IP address..."); 
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("iTray Network");
  
  Serial.println("Listening for DNS request..."); 

  // if DNSServer is started with "*" for domain name, it will reply with provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);
  
  Serial.println("Waiting for remote connection..."); 
  
  // replay to all requests with same HTML
  webServer.onNotFound([]() {

  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" networks found");

  
  responseHTML = "<ul>";
  for (int i = 0; i < n; ++i){
      // Print SSID and RSSI for each network found
      responseHTML += "<li><input type='radio' name='ssid' value='";
      responseHTML += WiFi.SSID(i);
      responseHTML += "' data-open='";
      responseHTML += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?"true":"false";
      responseHTML += "'>";
//        responseHTML +=i + 1;
//        responseHTML += ": ";
      responseHTML += WiFi.SSID(i);
//        responseHTML += " (";
//        responseHTML += WiFi.RSSI(i); // Signal strength in dBm
//        responseHTML += ")";
      responseHTML += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" (Open)":"";
      responseHTML += "</li>";

      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
    }
    responseHTML += "</ul>";

    String s;
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    s += "<!DOCTYPE html><html><head><title>iTray Network</title></head><body> ";
    s += "<h1>Select your WiFi Network</h1><p>If you can't see your network try to restart your router.</p>";
//    s += ipStr;
//    s += "<p>";
    s += "<form method='get' action='a'>";
    s += responseHTML;
    s += "<label>Password (if applicable): </label><input name='pass' length=64><input type='submit'></form>";
    s += "</body></html>\r\n\r\n";  
              
    webServer.send(200, "text/html", s);
  });
  
  webServer.begin();

  WiFiClient client;


  String req = client.readStringUntil('\r');

  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
      Serial.print("Invalid request: ");
      Serial.println(req);
//      return(20);
  }
  req = req.substring(addr_start + 1, addr_end);
  Serial.print("Request: ");
  Serial.println(req);

}

void connectToServer(){
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    logln("connection failed");
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
