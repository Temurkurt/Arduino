#include <SoftwareSerial.h>

int veri;
int kirmizipin = 9;
int yesilpin = 10;
int mavipin = 11;
void setup() {
 Serial.begin(9600);
 pinMode(kirmizipin, OUTPUT);
 pinMode(yesilpin, OUTPUT);
 pinMode(mavipin, OUTPUT);
}

void loop() {
 if(Serial.available()>0){
  veri = Serial.read();
  }
if(veri == '0'){
  digitalWrite(kirmizipin, LOW);
  digitalWrite(yesilpin, HIGH);
  digitalWrite(mavipin, HIGH);
}
else if(veri == '1'){
    digitalWrite(kirmizipin, HIGH);
    digitalWrite(yesilpin, LOW);
    digitalWrite(mavipin, HIGH);
}
else if(veri == '2'){
  digitalWrite(kirmizipin, HIGH);
  digitalWrite(yesilpin, HIGH);
  digitalWrite(mavipin, LOW);
      
}
else {
  digitalWrite(kirmizipin, HIGH);
  digitalWrite(yesilpin, HIGH);
  digitalWrite(mavipin, HIGH);
}

}
