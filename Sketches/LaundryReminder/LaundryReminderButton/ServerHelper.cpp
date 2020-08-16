#include <Arduino.h>
#include "ServerHelper.h"

ServerHelper::ServerHelper()
{
}

void ServerHelper::handleNewClient(WiFiClient client, String header, String &requestUrl, String &queryParamsStr)
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
          requestUrl = getRequestUrl(header, queryParamsStr);
          Serial.println("Request url is: " + requestUrl);
          Serial.println("detected params: "+ queryParamsStr);
          break;
          // if(requestUrl=="/"){
          //   PrintHomePage(client);
          //   // Break out of the while loop
          //   break;
          // }
          // if(requestUrl=="/submit"){
            
          //   // Break out of the while loop
            
          // }
        } 
        else { // if you got a newline, then clear currentLine
          currentLine = "";
        }
      } else if (c != '\r') {  // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }
  }
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
      startDetect = true;
    }
    // if space, break
    else if(currChar == ' '){spaceCount++;}
    if (spaceCount > 1)
    {
      break;
    }
    
    // if start detected, save characters
    if (startDetect && currChar!= '?')
    {
      detectedUrl+=currChar;
    }
    else if(detectParams){
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
  return detectedUrl;
}

void ServerHelper::GetRequestParameters(String queryParamStr,String &ssid, String &pw){
  bool ssidKeyDetected = false;
  bool pwKeyDetected = false;
  int ampIndedx = 0;
  String ssidDst = "";
  String pwDst = "";
  if(queryParamStr.length() > 0){
    // localte the ampersand
    for(int i = 0 ; i < queryParamStr.length() ; i++){
      char currChar = queryParamStr[i];
      if(currChar == '&'){ampIndedx = i;}
    }

    // extract the ssid
    ssid = queryParamStr.substring(5,ampIndedx);
    pw = queryParamStr.substring(ampIndedx+4,queryParamStr.length());

    Urldecode2(ssidDst, ssid);
    Urldecode2(pwDst, pw);

    ssid = ssidDst;
    pw = pwDst;
    Serial.println("ssid: "+ssid);
    Serial.println("pw: "+ pw);
  }
}

void ServerHelper::Urldecode2(String &dst, String src)
{
  char a, b;
  int i = 0;
  while (i < src.length())
  {
    char currChar = src[i];
    /* code */
    if ((currChar == '%') &&
      ((a = src[i+1]) && (b = src[i+2])) &&
      (isxdigit(a) && isxdigit(b))) {
        if (a >= 'a')
                a -= 'a'-'A';
        if (a >= 'A')
                a -= ('A' - 10);
        else
                a -= '0';
        if (b >= 'a')
                b -= 'a'-'A';
        if (b >= 'A')
                b -= ('A' - 10);
        else
                b -= '0';
        dst += (char)(16*a+b);
        i+=3;
    } else if (currChar == '+') {
      dst += ' ';
      i++;
    } else {
      dst += currChar;
      i++;
    }
  }
  // dst += '\0';
}

void ServerHelper::PrintHomePage(WiFiClient client, String ssid, String pw){
  Serial.println("Home page detected");
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
  client.println("<body><h1>Reminder Button WiFi Credentials</h1>");

  client.println("<br><br>");

  client.println("");
  client.println("<form action='/submit' method='get'>");
  client.println("  <label for='ssid'>SSID:</label>");
  client.println("  <input type='text' id='ssid' name='ssid'><br><br>");
  client.println("  <label for='pw'>PW:</label>");
  client.println("  <input type='text' id='pw' name='pw'><br><br>");
  client.println("  <input type='submit' value='Submit'>");
  client.println("</form>");

  if(pw.length() > 0 || ssid.length() > 0){
    client.println("<br><br>");
    client.println("<label>Current SSID in memory:</label>");
    client.println("<label>"+ssid+"</label>");
    client.println("<br><br>");
    client.println("<label>Current PW in memory:</label>");
    client.println("<label>"+pw+"</label>");
  }

  client.println("</body></html>");

  // The HTTP response ends with another blank line
  client.println();
}

void ServerHelper::PrintSubmitPage(WiFiClient client, String ssid, String pw){
  Serial.println("Submit page detected");

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
  client.println("<body><h1>Reminder Button WiFi Credentials</h1>");
  
  client.println("<br><br>");

  client.println("");
  client.println("  <label>SSID:</label><br>");
  client.println("  <label>"+ssid+"</label><br><br>");
  client.println("  <label>PW:</label><br>");
  client.println("  <label>"+pw+"</label><br><br>");
  
  client.println("<a href='/'>Home</a>");
  
  client.println("</body></html>");
  
  // The HTTP response ends with another blank line
  client.println();
}