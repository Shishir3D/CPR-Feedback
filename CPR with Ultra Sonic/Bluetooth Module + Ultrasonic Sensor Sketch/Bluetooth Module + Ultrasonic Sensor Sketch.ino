#include <SoftwareSerial.h> // Include the SoftwareSerial library

#define trigPin 10 // Trig pin of ultrasonic sensor
#define echoPin 11 // Echo pin of ultrasonic sensor

SoftwareSerial BTSerial(0, 1); // RX, TX

void setup() {
  BTSerial.begin(9600); // Initialize the Bluetooth module
  pinMode(trigPin, OUTPUT); // Set the trig pin as an output
  pinMode(echoPin, INPUT); // Set the echo pin as an input
}

void loop() {
  long duration, distance; // Variables to store the duration and distance
  
  digitalWrite(trigPin, LOW); // Set the trig pin low
  delayMicroseconds(50); 
  digitalWrite(trigPin, HIGH); // Set the trig pin high
  delayMicroseconds(15); 
  digitalWrite(trigPin, LOW); // Set the trig pin low
  
  duration = pulseIn(echoPin, HIGH); // Measure the duration of the pulse
  distance = (duration/2) / 29.1; // Calculate the distance in cm
  
  // BTSerial.print("Distance: "); // Print the distance
  if (distance < 20){
    BTSerial.println(distance);
    BTSerial.flush();
  }
  // BTSerial.print(" cm");
  // BTSerial.println();
  delay(50); // Wait for a second
}