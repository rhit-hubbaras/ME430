#include <Stepper.h>

const int stepsPerRevolution = 2048;  // 32 steps * 64:1 gearhead
//const int stepsPerRevolution = 120;  // Tell a lie for now!
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);  // Order is important!

void setup() {
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  // myStepper.setSpeed(5); // For later
  // set the speed at 1 rpm:
  myStepper.setSpeed(10);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(4, !digitalRead(2));
  digitalWrite(5, !digitalRead(3));
  analogWrite(6, analogRead(0)/4);
  
  digitalWrite(12, !digitalRead(2));
  digitalWrite(13, !digitalRead(3));
  analogWrite(11, analogRead(0)/4);
  
  if(!digitalRead(2)){
    // step one revolution  in one direction:
    myStepper.step(stepsPerRevolution/4);
    delay(500);
  }

  if(!digitalRead(3)){
    // step one revolution in the other direction:
    myStepper.step(-stepsPerRevolution/4);
    delay(500);
  }
  delay(10);
  Serial.println(!digitalRead(2));
}
