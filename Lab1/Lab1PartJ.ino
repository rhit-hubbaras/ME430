#include <Stepper.h>

const int stepsPerRevolution = 2048;  // 32 steps * 64:1 gearhead
//const int stepsPerRevolution = 120;  // Tell a lie for now!
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);  // Order is important!

void setup() {
  // myStepper.setSpeed(5); // For later
  // set the speed at 1 rpm:
  myStepper.setSpeed(1);
}

void loop() {
  // step one revolution  in one direction:
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
