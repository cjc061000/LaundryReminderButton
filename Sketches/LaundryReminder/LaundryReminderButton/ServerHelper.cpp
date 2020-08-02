#include <Arduino.h>
#include "ServerHelper.h"

ServerHelper::ServerHelper()
{
}

void ServerHelper::handleNewClient(WiFiClient client, String header)
{
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
          Serial.println("Thea request header is:");
          Serial.println(header);
          String paramStr = "";
          String requestUrl = getRequestUrl(header, paramStr);
          Serial.println("Request url is: " + requestUrl);
          Serial.println("detected params: "+ paramStr);
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();

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
          
          client.println("<br><br>");

          client.println("");
          client.println("<form action='/submit' method='get'>");
          client.println("  <label for='ssid'>SSID:</label>");
          client.println("  <input type='text' id='ssid' name='ssid'><br><br>");
          client.println("  <label for='pw'>PW:</label>");
          client.println("  <input type='text' id='pw' name='pw'><br><br>");
          client.println("  <input type='submit' value='Submit'>");
          client.println("</form>");

          // Display current state, and ON/OFF buttons for GPIO 26  
          //client.println("<p>GPIO 26 - State " + output26State + "</p>");
          // If the output26State is off, it displays the ON button       
          // if (output26State=="off") {
          //   client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
          // } else {
          //   client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
          // } 
              
          // Display current state, and ON/OFF buttons for GPIO 27  
          //client.println("<p>GPIO 27 - State " + output27State + "</p>");
          // If the output27State is off, it displays the ON button       
          // if (output27State=="off") {
          //   client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
          // } else {
          //   client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
          // }
          client.println("</body></html>");
          
          // The HTTP response ends with another blank line
          client.println();
          // Break out of the while loop
          break;
        } 
        else { // if you got a newline, then clear currentLine
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
}

String ServerHelper::getRequestUrl(String header, String &detectedParams){
  String detectedUrl = "";
  bool startDetect = false;
  bool detectParams = false;
  int spaceCount = 0;

  for(int i = 0 ; i < header.length() ; i++){
    char currChar = header[i];
    Serial.print("Working with Char: ");
    Serial.println((String)currChar);
    // start detect
    if(currChar == '/'){
      Serial.println("First Slash Detected");
      startDetect = true;
    }
    // if space, break
    else if(currChar == ' '){Serial.println("Space Detected");spaceCount++;}
    if (spaceCount > 1)
    {
      break;
    }
    

    // if start detected, save characters
    if (startDetect && currChar!= '?')
    {
      Serial.println("Adding to url var");
      detectedUrl+=currChar;
    }
    else if(detectParams){
      Serial.println("Adding to params var");
      String cc = String(currChar);
      detectedParams += cc;
    }
    
    // if ? then detect params
    if (currChar == '?')
    {
      Serial.println("? Detected");
      startDetect = false;
      detectParams = true;
    }
  }
  Serial.println("params before return: " + detectedParams);
  return detectedUrl;
}
void ServerHelper::getRequestParameters(String header,String ssid, String pw){
  
}

// void Morse::dash()
// {
//   digitalWrite(_pin, HIGH);
//   delay(1000);
//   digitalWrite(_pin, LOW);
//   delay(250);
// }