// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Servo.h>
/*#include <LiquidCrystal.h> 
int rs = 3, en = 2, d4 = 0, d5 = 14, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en,d4,d5, d6, d7);*/

Servo Doorlock; // create servo object to control a servo
int val;

// Replace with your network credentials
const char* ssid     = "Vefa_Suleymaniye_yb";
const char* password = "1888-1959";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";
String output16State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;
const int output16 = 16;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
//  lcd.begin(16,2);
  //Starting the LCD Screen
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

  Doorlock.attach(16);

  /*lcd.setCursor(0,0);
  lcd.print("IP adress:");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());*/
}

void loop(){
 
  WiFiClient client = server.available();   // Listen for incoming clients

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
              Serial.println("Light 2 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Light 2 off");
              output4State = "off";
              digitalWrite(output4, LOW);
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
            client.println("<p>Light 1</p>");
            // If the output5State is off, it displays the ON button
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">Closed</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">Opened</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for Light 2 
            client.println("<p>Light 2</p>");
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">Closed</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">Opened</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Door
            client.println("<p>Door</p>");
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
