// #include <WiFi.h>
// #include <WebServer.h>     // Replace with WebServer.h for ESP32
// #include <AutoConnect.h>
// WebServer Server;          // Replace with WebServer for ESP32
// AutoConnect      Portal(Server);

// void setup() {
//   delay(1000);
//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);
//   delay(100);
//   WiFi.begin();
//   if (WiFi.waitForConnectResult() == WL_CONNECTED) {
//     WiFi.disconnect();
//     while (WiFi.status() == WL_CONNECTED)
//       delay(100);
//   }
//   Serial.println("WiFi disconnected.");
// }

// void loop() {
//   delay(1000);
// }
//-----------------------------------------------------------------------------------------------
// Load Wi-Fi library
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include "ServerHelper.h"

// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);
ServerHelper serverHelper(13);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  serverHelper.test();
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #include <WiFi.h>
// #include <WebServer.h>
// #include <WiFiClientSecure.h>
// #include <SparkFun_Qwiic_Button.h>

// #include <AutoConnect.h>

// QwiicButton button;
// WebServer Server;
// AutoConnect Portal(Server);

// // OLED declarations
// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 32 // OLED display height, in pixels
// #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// const char * ERROR_TXT = "Error: " ;
// const char * INFO_TXT = "Info: " ;


// // Button declarations
// uint8_t brightness = 100;   //The brightness to set the LED to when the button is pushed
//                             //Can be any value between 0 (off) and 255 (max)

// // WiFi network name and password:
// const char * _networkName = "IntoTheSpiderWebs";
// const char * _networkPswd = "4thDimension";

// // Internet domain to request from:
// const char * _hostDomain = "khappservice.azurewebsites.net";
// const char * _resource = "/api/KS-SendEmail?code=1mPYgJrkZSrB8ZHiN06tPSx52Tzst9IE7mBlScxRzmTuVVFTdfubIw==";
// const int _hostPort = 80;

// const int BUTTON_PIN = 0;
// const int LED_PIN = 13;

// /* use 
// openssl s_client -showcerts -connect www.howsmyssl.com:443 </dev/null 
// to get this certificate */
// const char* ca_cert = \
//   "-----BEGIN CERTIFICATE-----\n" \
// "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n" \
// "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n" \
// "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n" \
// "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n" \
// "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n" \
// "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n" \
// "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n" \
// "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n" \
// "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n" \
// "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n" \
// "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n" \
// "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n" \
// "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n" \
// "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n" \
// "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n" \
// "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n" \
// "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n" \
// "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n" \
// "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n" \
// "-----END CERTIFICATE-----\n";
// struct Logger{
//   Logger(){};
//   private:void WriteErrorToScreen (char * msg){
//     display.clearDisplay();
//     display.setCursor(0,0);
//     display.write(ERROR_TXT);
//     display.write(msg);
//     display.display();
//     delay(100);
//   }
//   private:void WriteInfoToScreen (char * msg, bool append){
//     if(!append){
//       display.clearDisplay();
//       display.setCursor(0,0);
//       display.write(INFO_TXT);
//     }
//     display.write(msg);
//     display.display();
//     delay(100);
//   }

//   public:void logError(char * errorMsg){ 
//     // write into the serial port
//     Serial.println(ERROR_TXT);
//     Serial.print(errorMsg);

//     // write into the screen
//     WriteErrorToScreen(errorMsg);
//   }
//   public:void logInfo(StringSumHelper infoMsg){
//       logInfo( strdup( infoMsg.c_str()) );
//   }
//   public:void logInfo(char * infoMsg){ 
//     // write into the serial port
//     Serial.println(INFO_TXT);
//     Serial.print(infoMsg);

//     // write into the screen
//     WriteInfoToScreen(infoMsg, (bool)false);
//   }
//   public:void logInfoAppend(StringSumHelper infoMsg){
//       logInfoAppend( strdup( infoMsg.c_str()) );
//   }
//   public:void logInfoAppend(char * infoMsg){
//     // write into the serial port
//     Serial.println(INFO_TXT);
//     Serial.print(infoMsg);

//     // write into the screen
//     WriteInfoToScreen(infoMsg, (bool)true);
//   }
// };
// Logger logger;
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////        SETUP Functions     ////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////


// void setup()
// {
//   // Initilize hardware:
//   Serial.begin(115200);
//   pinMode(BUTTON_PIN, INPUT_PULLUP);
//   pinMode(LED_PIN, OUTPUT);

//   InitScreen();

//   if(!InitButton()){
//     logger.logError("Unable to initialize Button ... Stopping...");
//     while (1);
//   };
  
  
//   // // Connect to the WiFi network (see function below loop)
//   Server.on("/", rootPage);
//   if (Portal.begin()) {
//     Serial.println("HTTP server:" + WiFi.localIP().toString());
//   }

//   digitalWrite(LED_PIN, LOW); // LED off

//   logger.logInfo("Init Success ... Waiting for input ...");
// }

// void rootPage() {
//   char content[] = "Hello, world";
//   Server.send(200, "text/plain", content);
// }
// // ----------------------------------------------------
// // Startup Aux Functions
// // ----------------------------------------------------
// bool InitScreen(){
//   // configure the screen
//   Serial.println("Initializing Screen");
//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
//     Serial.println(F("SSD1306 allocation failed"));
//     return false;
//   }
//   Serial.println("Screen initialized");
//   display.display();
//   delay(2000); // Pause for 2 seconds
//   display.clearDisplay();
//   display.setTextSize(1);      // Normal 1:1 pixel scale
//   display.setTextColor(SSD1306_WHITE); // Draw white text
//   display.setCursor(0, 0);     // Start at top-left corner
//   display.cp437(true);         // Use full 256 char 'Code Page 437' font
//   display.display();

//   return true;
// }

// bool InitButton(){
//   // configuring the button
//   Serial.println("Connecting Qwicc button");
//   Wire.begin(); //Join I2C 
//   if (button.begin() == false) {
//     Serial.println("Buton Device did not acknowledge! Freezing.");
//     return false;
//   }
//   Serial.println("Button acknowledged.");
//   //Start with the LED off
//   button.LEDoff();
//   button.clearEventBits();
//   return true;
// }

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////        MAIN LOOP Function    //////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////

// void loop()
// { 
//   Portal.handleClient();
//   if (button.isPressed() == true) {
//     button.LEDon(brightness);
//     while(button.isPressed() == true)
//       delay(10);  //wait for user to stop pressing
//     button.LEDoff();
//   }
//   if(button.hasBeenClicked()){
//     button.clearEventBits();
//     connectToWiFi(_networkName, _networkPswd);
//     delay(2000);
//     requestURL(_hostDomain,_hostPort);
//   }
  
//   delay(500); //Don't hammer too hard on the I2C bus
// }


// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////        AUX Functions    ///////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void connectToWiFi(const char * ssid, const char * pwd)
// {
//   int ledState = 0;
//   logger.logInfo( "Connecting to WiFi network: " + String(ssid));
//   if (WiFi.status() != WL_CONNECTED)
//   {
//     WiFi.begin(ssid, pwd);
//   }
//   else{logger.logInfo("Wifi already connected. ");}

//   while (WiFi.status() != WL_CONNECTED) 
//   {
//     // Blink LED while we're connecting:
//     digitalWrite(LED_PIN, ledState);
//     ledState = (ledState + 1) % 2; // Flip ledState
//     delay(500);
//     logger.logInfoAppend(".");
//   }
//   logger.logInfoAppend("IP: " + String(WiFi.localIP()));
// }

// void requestURL(const char * host, uint8_t port)
// {
//   printLine();
//   Serial.println("Connecting to domain: " + String(host));

//   // Use WiFiClient class to create TCP connections
//   WiFiClient client;    //Declare object of class HTTPClient
  
//   /* set SSL/TLS certificate */
//   //client.setCACert(ca_cert);
//   if (!client.connect(host, port))
//   {
//     Serial.println("Connection error");
//     return;
//   }
  
//   printLine();

//   String body = "{\"dummy\":\"data\"}";
//   // This will send the request to the server
//   client.print("POST " + String(_resource)+ " HTTP/1.1\r\n" +
//                "Host: " + String(host) + "\r\n" +
//                "Content-Length: " + body.length() + "\r\n" +
//                "Content-Type: application/json\r\n" +
//                "Connection: close\r\n\r\n"+
//                "\r\n" + body);
//   unsigned long timeout = millis();
//   while (client.available() == 0) 
//   {
//     if (millis() - timeout > 30000) 
//     {
//       Serial.println(">>> Client Timeout !");
//       client.stop();
//       return;
//     }
//   }

//   // Read all the lines of the reply from server and print them to Serial
//   String responseLines = "";
//   while (client.available()) 
//   {
//     String line = client.readStringUntil('\r');
//     responseLines = responseLines + " " + line;
//     Serial.print(line);
//   }
//   logger.logInfo(responseLines);
//   Serial.println();
//   Serial.println("closing connection");
//   client.stop();
// }

// void printLine()
// {
//   Serial.println();
//   for (int i=0; i<30; i++)
//     Serial.print("-");
//   Serial.println();
// }

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////        Error Functions    ////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// // struct Logger{
// //   Logger(){};
// //   void logError(char * errorMsg){ 
// //     // write into the serial port
// //     Serial.println(ERROR_TXT);
// //     Serial.print(errorMsg);

// //     // write into the screen
// //     WriteErrorToScreen(errorMsg);
// //   }
// //   void logInfo(char * infoMsg){ 
// //     // write into the serial port
// //     Serial.println(INFO_TXT);
// //     Serial.print(infoMsg);

// //     // write into the screen
// //     WriteErrorToScreen(infoMsg);
// //   }
// // };



// void WriteErrorToScreen (char * msg){
//   display.clearDisplay();
//   display.display();
//   display.write(ERROR_TXT);
//   display.write(msg);
//   display.display();
//   delay(100);
// }
// void WriteInfoToScreen (char * msg){
//   display.clearDisplay();
//   display.display();
//   display.write(INFO_TXT);
//   display.write(msg);
//   display.display();
//   delay(100);
// }


