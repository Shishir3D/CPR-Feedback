//Bluetooth_Terminal
#include <SoftwareSerial.h>
#include <Servo.h>

Servo servo;
const byte rxPin = 3; //rx on arduino
const byte txPin = 2; //tx on arduino
SoftwareSerial BTserial(rxPin,txPin); //Rx Tx
char Position;

void setup() {
  // put your setup code here, to run once:
  
  servo.attach(11);
  servo.write(85);
  
  pinMode(rxPin,INPUT);
  pinMode(txPin, OUTPUT);
  BTserial.begin(9600);
  Serial.begin(9600);


}

void loop()
{
  // put your main code here, to run repeatedly:
  if (BTserial.available()>0)
  {
    Position = BTserial.read();
    if (Position == 'a')
    {
      servo.write(105);
      delay(200);
      servo.write(85);
      delay(5);
    }
  }
  

  //if (BTserial.available()>0 || Position > 0) 
  //{
    //Position = 1;
    //servo.write(180); 
    //delay(1000);
    //servo.write(90);
    //delay(1000);
  //}

}

