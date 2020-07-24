#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>
#include <SparkFun_Qwiic_Button.h>
QwiicButton button;

// OLED declarations
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button declarations
uint8_t brightness = 100;   //The brightness to set the LED to when the button is pushed
                            //Can be any value between 0 (off) and 255 (max)

// WiFi network name and password:
const char * networkName = "IntoTheSpiderWebs";
const char * networkPswd = "4thDimension";

// Internet domain to request from:
const char * hostDomain = "google.com";
const int hostPort = 80;

const int BUTTON_PIN = 0;
const int LED_PIN = 13;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////        SETUP Functions     ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup()
{
  // Initilize hardware:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  InitScreen();

  // configuring the button
  Serial.println("Connecting Qwicc button");
  Wire.begin(); //Join I2C 
  if (button.begin() == false) {
    Serial.println("Buton Device did not acknowledge! Freezing.");
    while (1);
  }
  Serial.println("Button acknowledged.");
  //Start with the LED off
  button.LEDoff();
  
  // Connect to the WiFi network (see function below loop)
  connectToWiFi(networkName, networkPswd);

  digitalWrite(LED_PIN, LOW); // LED off
  Serial.print("Press button 0 to connect to ");
  Serial.println(hostDomain);
}

bool InitScreen(){
  // configure the screen
  Serial.println("Initializing Screen");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }
  Serial.println("Screen initialized");
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("- Hello World -- Hello World -- Hello World -- Hello World -");
  display.display();

  return true;
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////        MAIN LOOP Function    //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  if (digitalRead(BUTTON_PIN) == LOW)
  { // Check if button has been pressed
    while (digitalRead(BUTTON_PIN) == LOW)
      ; // Wait for button to be released

    digitalWrite(LED_PIN, HIGH); // Turn on LED
    requestURL(hostDomain, hostPort); // Connect to server
    digitalWrite(LED_PIN, LOW); // Turn off LED
  }
  //check if button is pressed, and tell us if it is!
  if (button.isPressed() == true) {
    Serial.println("The button is pressed!");
    button.LEDon(brightness);
    while(button.isPressed() == true)
      delay(10);  //wait for user to stop pressing
    Serial.println("The button is not pressed.");
    button.LEDoff();
  }
  delay(20); //Don't hammer too hard on the I2C bus
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////        AUX Functions    ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void connectToWiFi(const char * ssid, const char * pwd)
{
  int ledState = 0;

  printLine();
  Serial.println("Connecting to WiFi network: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) 
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_PIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void requestURL(const char * host, uint8_t port)
{
  printLine();
  Serial.println("Connecting to domain: " + String(host));

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.println("Connected!");
  printLine();

  // This will send the request to the server
  client.print((String)"GET / HTTP/1.1\r\n" +
               "Host: " + String(host) + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 5000) 
    {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) 
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  client.stop();
}

void printLine()
{
  Serial.println();
  for (int i=0; i<30; i++)
    Serial.print("-");
  Serial.println();
}
