#include <Servo.h>

Servo servo;

void setup() {
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  servo.attach(9);
}

void loop() {
  digitalWrite(4, !digitalRead(2));
  digitalWrite(5, !digitalRead(3));
  analogWrite(6, analogRead(0)/4);
  
  digitalWrite(12, !digitalRead(2));
  digitalWrite(13, !digitalRead(3));
  analogWrite(11, analogRead(0)/4);
  
  if(!digitalRead(2)){
    servo.write(0);
  }else if(!digitalRead(3)){
    servo.write(180);
  }else{
    int degrees = map(analogRead(0), 0, 1023, 0, 180);
    servo.write(degrees);
  }
  delay(10);
}
