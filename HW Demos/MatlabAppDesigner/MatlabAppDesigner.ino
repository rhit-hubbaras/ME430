// C++ code
//
#include <Servo.h>

Servo servo;

String inputString;
bool stringComplete;

bool buttonWasPressed;
bool buttonIsPressed;

int joystickX;
int joystickY;
int lastJoystickX;
int lastJoystickY;

const int JOYSTICK_THRESHOLD = 10;

void setup()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  inputString.reserve(200);
  servo.attach(9);
}

void loop()
{
  buttonIsPressed = !digitalRead(2);
  if(buttonIsPressed && !buttonWasPressed){
    Serial.println("B");
  }
  buttonWasPressed = buttonIsPressed;
  
  joystickX = analogRead(0);
  if(abs(joystickX-lastJoystickX)>=JOYSTICK_THRESHOLD){
    Serial.println("X "+String(joystickX));
    lastJoystickX = joystickX;
  }
  
  joystickY = analogRead(1);
  if(abs(joystickY-lastJoystickY)>=JOYSTICK_THRESHOLD){
    Serial.println("Y "+String(joystickY));
    lastJoystickY = joystickY;
  }
  
  if (stringComplete) {
    handleMessage();
      
    inputString = "";
    stringComplete = false;
  }
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == 10) {
      inputString.toUpperCase();
      stringComplete = true;
    }else if(inChar == 13){
      //pass
    }else{
      inputString += inChar;
    }
  }
}

void handleMessage() {
  Serial.println(inputString);

  if(inputString.startsWith("LED ")){
    String input = inputString.substring(4);
    if(input.equals("ON")){
      digitalWrite(3,HIGH);
    }
    if(input.equals("OFF")){
      digitalWrite(3,LOW);
    }
  }
  if(inputString.startsWith("SERVO ")){
    int deg = inputString.substring(6).toInt();
    servo.write(deg);
  }
}