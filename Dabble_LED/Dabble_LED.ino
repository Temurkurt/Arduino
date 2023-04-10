#include <Servo.h>


int reading;
Servo Window1;
Servo Window2;
Servo Window3;
Servo Window4;
//bool statusWindows;
int degreeWindow1;
int degreeWindow2;
int degreeWindow3;
int degreeWindow4;

void setup() {
  Window1.attach(9);
  Window2.attach(10);
  Window3.attach(11);
  Window4.attach(6);
}

void loop() {

reading = analogRead(A0);

degreeWindow1 = map(reading,0,1023,0,180);
degreeWindow2 = map(reading,0,1023,0,180);
degreeWindow3 = map(reading,0,1023,0,180);
degreeWindow4 = map(reading,0,1023,0,180);
Window1.write(degreeWindow1);
Window2.write(degreeWindow2);
Window3.write(degreeWindow3);
Window4.write(degreeWindow4);

}
