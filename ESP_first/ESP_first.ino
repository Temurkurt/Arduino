#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
ESP8266WebServer server(80); // Server'ın dinleyeceği portu belirler. Default port 80'dir.
 
void setup() {
 
  Serial.begin(115200);           // 115200 baud rate'de seri haberleşmeyi başlatır.
  WiFi.begin("Vefa_Suleymaniye_yb", "1888-1959");  // Wifi ağa bağlanır. Ağ adınızı ve şifrenizi girin.
 
  while (WiFi.status() != WL_CONNECTED) {  // Bağlantı başarılı ise aşağıdaki komutları ekrana yansıtır.
 
    delay(500);
    Serial.println("Bağlantı bekleniyor.");
 
  }
 
  Serial.print("IP adresi: ");
  Serial.println(WiFi.localIP());  // ESP8266 modülünün IP adresini ekrana yazar.
 
 
  server.begin();  // Server'ı başlatır.
 
}
 
void loop() {
 
  server.handleClient();         // Server'a gelen http requestleri handle eder.
}
 
void handleRootPath() {          
 
  server.send(200, "text/plain", "Merhaba Dunya!"); // Server'a giriş yapıldığında ekrana yansıtır.
 
}
