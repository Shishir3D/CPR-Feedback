int trigpin = 10;
int echopin = 11;

float pingtime;
float targetdistance;
float speedofsound = 776.5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2000);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigpin, LOW);

  pingtime = pulseIn(echopin,HIGH);
  pingtime = pingtime/1000000.0;
  pingtime = pingtime/3600.0;

  targetdistance = speedofsound * pingtime;
  targetdistance = targetdistance/2;
  targetdistance = targetdistance*160934;

  //Serial.print("The distance to the target is : ");
  if (targetdistance < 10){
    Serial.println(int(targetdistance));
  }
  //Serial.println(" cm");
  delay(100);
}
