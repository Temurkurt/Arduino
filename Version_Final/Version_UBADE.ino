// Load Wi-Fi, servo, lcd libraries
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//We are defining the lcd's pizxels and writable areas ???????????????????????
int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

Servo Doorlock; // create servo object to control a servo
Servo Window;
int val; 

// Replace with your network credentials
const char* ssid     = "xyz";
const char* password = "abcde123";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";
String output16State = "off";

// Assign output variables to GPIO pins
const int output5 = 13;
const int output4 = 12;
const int output16 = 15;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  //Starting the connection between esp8266 and lcd monitor
  Wire.begin();
  
  //Starting the Serial communication
  Serial.begin(115200);
  
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output16, OUTPUT);
  
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output16, LOW);

  // Connect to Wi-Fi network with SSID and password
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();


 //Defining the servo motor
  Doorlock.attach(12);

  Window.attach(15);
  
 //LCD code to turn on backlight and 
 lcd.init();
 lcd.backlight();

}

void loop(){

//To control and follow the status of the esp8266's connection we print the status to serial monitor
/*
byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  */

 //We set our cursor location to first line 
 lcd.setCursor(0,0);
 //To have a better user experience we write the first line of code
 lcd.print("Your IP is:");
 //We set our cursor location to second line
 lcd.setCursor(0,1);
 //To write the IP
 lcd.print(WiFi.localIP());
 
  WiFiClient client = server.available();   // Listen for incoming clients

//The following line of codes work with html to create a webpage

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
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
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("Light 1 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("Light 1 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("Window opened");
              output4State = "on";
              Window.write(180);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Window closed");
              output4State = "off";
              Window.write(0);
            } else if (header.indexOf("GET /16/on") >= 0) {
              Serial.println("Door unlocked");
              output16State = "on";
              Doorlock.write(180);
            } else if (header.indexOf("GET /16/off") >= 0) {
              Serial.println("Door locked");
              output16State = "off";
              Doorlock.write(0);}

            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #000000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #00C255;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Smart Sweet Home</h1>");
            
            // Display current state, and ON/OFF buttons for Light 1
            client.println("<p>Living Room Light</p>");
            // If the output5State is off, it displays the ON button
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">Closed</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">Opened</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for Light 2 
            client.println("<p>Living Room Window</p>");
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">Closed</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">Opened</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Door
            client.println("<p>Living Room Door</p>");
            if (output16State=="off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">Locked</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">Unlocked</button></a></p>");
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
