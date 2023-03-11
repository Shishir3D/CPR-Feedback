#include <SoftwareSerial.h>
#include <Servo.h>

Servo myservo;
SoftwareSerial mySerial(2,3; //Rx and Tx)

int angle = 90 ;
int state  = 0;

void setup() {
  myservo.attach(8);
  myservo.write(angle);
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop(){
  if(Serial.available() > 0){
    state = Serial.read();
  }

  if (state == "a"){
    myservo.write(80);
    state = 0;
  }
}