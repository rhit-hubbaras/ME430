#include <Servo.h>
#include <Stepper.h>
#include <Keypad.h>

#define LED_DIGIT_0 0b11111100
#define LED_DIGIT_1 0b01100000
#define LED_DIGIT_2 0b11011010
#define LED_DIGIT_3 0b11110010
#define LED_DIGIT_4 0b01100110
#define LED_DIGIT_5 0b10110110
#define LED_DIGIT_6 0b00111110
#define LED_DIGIT_7 0b11100000
#define LED_DIGIT_8 0b11111110
#define LED_DIGIT_9 0b11100110
#define LED_GOOF_0 0b00111001
#define LED_GOOF_1 0b00000001
#define LED_GOOF_2 0b00101010
#define LED_GOOF_3 0b00111010
#define LED_DONE_0 0b01111010
#define LED_DONE_1 0b00111010
#define LED_DONE_2 0b00101010
#define LED_DONE_3 0b10011110

#define INPUT_MAX_VALUE 9999
#define INPUT_PRESET_A 200
#define INPUT_PRESET_B 1500
#define INPUT_PRESET_C 69
#define INPUT_PRESET_D 15

#define SERVO_ANGLE_LOW 115
#define SERVO_ANGLE_HIGH 180

#define STEPPER_SPEED_FAST 90
#define STEPPER_SPEED_SLOW 90

#define PHOTOSENSOR_LOW_THRESHOLD 140
#define PHOTOSENSOR_HIGH_THRESHOLD 200
#define PHOTOSENSOR_READINGS 16

const uint8_t LEDcodes[10] = {
  LED_DIGIT_0, LED_DIGIT_1, LED_DIGIT_2, LED_DIGIT_3, LED_DIGIT_4,
  LED_DIGIT_5, LED_DIGIT_6, LED_DIGIT_7, LED_DIGIT_8, LED_DIGIT_9
};

const uint billValues[7] = {500,100,50,20,10,5,1};
const uint initialDeal[7] = {2,2,2,6,5,5,5};

// 33-23, 32-22
const uint8_t segmentPins[8] = {30,32,27,25,33,26,24,28};
const uint8_t digitPins[4] = {22,31,29,23};

char hexaKeys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Objects setup
byte rowPins[4] = {45,43,41,39}; 
byte colPins[4] = {44,42,40,38}; 
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4); 

Servo servo;
const int stepsPerRevolution = 48; 
Stepper stepper(stepsPerRevolution, 46, 47, 48, 49);

//Global variables
int slotCounter = 0;//???
uint desiredBills[7] = {0,0,0,0,0,0,0};
//uint x = 0;
long wait = 0;

uint photosensorReadings[PHOTOSENSOR_READINGS];
uint8_t nextPhotosensorReading = 0;
bool isPhotosensorHigh = 0;
bool wasPhotosensorHigh = 0;

void setup(){

  for (int i=0; i < 8; i++){
    pinMode(segmentPins[i], OUTPUT); //set segment and DP pins to output
  }
  //sets the digit pins as outputs
  for (int i=0; i < 4; i++){
    pinMode(digitPins[i], OUTPUT);
  }
  
  pinMode(6,OUTPUT); //DC motor
  pinMode(7,OUTPUT); //DC motor??

  stepper.setSpeed(200);

  servo.attach(9);
  servo.write(SERVO_ANGLE_HIGH);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(500);
  //digitalWrite(13,LOW);
}

int counter = 0;

void loop(){
  uint desiredMoney = getNumberInput();
  getBillDivision(desiredMoney);
  retrieveDesiredBills();
  waitUntilButtonPress();

  // for(int i=0;i<7;i++){
  //   long delayStart = millis();
  //   while(millis()-delayStart < 1000){
  //     showNumber(desiredBills[i]);
  //   }
  //   delay(100);
  // }
  
  // waitUntilPhotosensorHigh(20);
  // counter++;
  
  // for(int i=1;i<=3;i++){
  // spinBills(i);
  // delay(1500);
  // }

  // moveToEnd();
  // delay(1000);
  // for(int i=0;i<6;i++){
  //   moveOneTray();
  //   delay(1000);
  // }

  // servoDown();
  // delay(1000);
  // servoUp();
  // delay(1000);
}

void retrieveDesiredBills(){
  for(int i=0;i<7;i++){
    if(i==0){
      moveToEnd();
    }else{
      moveOneTray();
    }
    delay(1000);
    spinBills(desiredBills[i]);
    delay(500);
  }
}

void moveToEnd(){
  carForward();
  for(int i=0;i<6;i++){
    waitUntilPhotosensorHigh(30);
    carBackward();
    delay(50);
    carStop();
    delay(500);
    carForward();
  }
  carBackward();
  delay(50);
  carStop();
}

void moveOneTray(){
  carBackward();
  waitUntilPhotosensorHigh(30);
  carForward();
  delay(50);
  carStop();
}

void spinBills(uint x){
  if(x>0){
    servoDown();
    for(int i=0;i<x;i++){
      spinSingleBill();
    }
    servoUp();
    delay(1000);
  }
}
void spinSingleBill(){
  stepper.setSpeed(STEPPER_SPEED_FAST);
  spinStepper(400);
  delay(100);
  stepper.setSpeed(STEPPER_SPEED_SLOW);
  spinStepper(-400);
  delay(300);
}


void getBillDivision(uint x){
  if(x==1500){
    for(int i = 0;i < 7;i++){
      desiredBills[i] = initialDeal[i];
    }
  }else{
    for(int i = 0;i < 7;i++){
      desiredBills[i] = 0;
      uint billValue = billValues[i];
      while(x>=billValue){
        desiredBills[i] += 1;
        x -= billValue;
      }
    }
  }
}

uint getNumberInput(){
  bool done = 0;
  uint x = 0;
  while(!done){
    uint y = handleButtonPresses(x);
    if(y==-1){
      done = 1;
    }else{
      x=y;
    }
    if(x>INPUT_MAX_VALUE){
      showGoof();
    }else{
      showNumber(x);
    }
  }
  return x;
}

int readPhotosensor(){
  return analogRead(0);
}
bool getPhotosensorDigital(){
  uint v = readPhotosensor();
  photosensorReadings[nextPhotosensorReading] = v;
  nextPhotosensorReading = (nextPhotosensorReading + 1) % PHOTOSENSOR_READINGS;
  long photosensorValueSum = 0;
  for(int i = 0;i < PHOTOSENSOR_READINGS;i++){
    photosensorValueSum += photosensorReadings[i];
  }
  uint photosensorValue = photosensorValueSum / PHOTOSENSOR_READINGS;
  if(photosensorValue > PHOTOSENSOR_HIGH_THRESHOLD){
    isPhotosensorHigh = 1;
  }else if(photosensorValue < PHOTOSENSOR_LOW_THRESHOLD){
    isPhotosensorHigh = 0;
  }
  return isPhotosensorHigh;
}

void waitUntilPhotosensorHigh(){
  waitUntilPhotosensorHigh(1);
}
void waitUntilPhotosensorHigh(long minDelay){
  bool done = 0;
  bool wasHigh = 0;
  bool isHigh = 0;
  long delayStart = millis();
  while(!done){
    wasHigh = isHigh;
    isHigh = getPhotosensorDigital();
    done = (isHigh && !wasHigh) && (millis() - delayStart > minDelay);
    //showNumber(counter,isHigh?0x0f:0x00);
    //showNumber(readPhotosensor(),isHigh?0x0f:0x00);
  }
}


uint handleButtonPresses(uint x){

    char buttonPressed = keypad.getKey();

    if(buttonPressed){
      if(isdigit(buttonPressed)){
        int digitPressed = buttonPressed-48;
        x = x*10+digitPressed;
      }else if(buttonPressed=='*'){
        x=0;
      }else if(buttonPressed=='#'){
        if(x>INPUT_MAX_VALUE){
          x=0;
        }else{
          return -1;
        }
      }else if(buttonPressed=='A'){
        x=INPUT_PRESET_A;
      }else if(buttonPressed=='B'){
        x=INPUT_PRESET_B;
      }else if(buttonPressed=='C'){
        x=INPUT_PRESET_C;
      }else if(buttonPressed=='D'){
        x=INPUT_PRESET_D;
      }
    }
    return x;

}
void waitUntilButtonPress(){
  bool done = 0;
  while(!done){
    char buttonPressed = keypad.getKey();
    if(buttonPressed){
      done = 1;
    }
    showDone();
  }
}

void showNumber (uint number){
  showNumber(number,0);
}
void showNumber (uint number,uint8_t DPs){
  if (number == 0){
    displayDigit(3, LEDcodes[0]); //display 0 in the rightmost digit
    displayDigit(2,0x00);
    displayDigit(1,0x00);
    displayDigit(0,0x00);
  }
  else{
    for (int digit= 3; digit >=0; digit--){
      if (number > 0){
        displayDigit(digit,LEDcodes[number % 10] | ((DPs >> digit) & 0x01));
        number= number/10;
      }else{
        displayDigit(digit,((DPs >> digit) & 0x01));
      }
    }
  }
}

void showGoof (){
  displayDigit(0,LED_GOOF_0);
  displayDigit(1,LED_GOOF_1);
  displayDigit(2,LED_GOOF_2);
  displayDigit(3,LED_GOOF_3);
}
void showDone (){
  displayDigit(0,LED_DONE_0);
  displayDigit(1,LED_DONE_1);
  displayDigit(2,LED_DONE_2);
  displayDigit(3,LED_DONE_3);
}


void displayDigit(uint8_t digitNo, uint8_t ledCode){

  digitalWrite(digitPins[digitNo], LOW);
  for (int segment= 0; segment < 8; segment++){
    boolean isBitSet= bitRead(ledCode, 7-segment);

    //isBitSet= ! isBitSet; //remove this line if common cathode display
    digitalWrite(segmentPins[segment], isBitSet);
  }
  delay(4);
  digitalWrite(digitPins[digitNo], HIGH);
}

void doTheHokeyPokey(){
  // RUN CAR BACKWARDS
  carBackward();
  delay(100);
  //TURN ON SERVO
  servoDown();

  //RUN STEPPER
  spinStepper(360);



 //MOVE SERVO BACK
  servoUp();



 //RUN CAR FORWWARD
  carForward();
  delay(100);
}

void spinStepper(long degrees){
  //moves setpper forward
  long steps = (degrees*stepsPerRevolution) / 360;
  stepper.step(steps);
  delay(10);
  //TURN OFF STEPPER
  digitalWrite(46, LOW);
  digitalWrite(47, LOW);
  digitalWrite(48, LOW);
  digitalWrite(49, LOW);
  
}



void servoDown(){
  digitalWrite(13,HIGH);
  //servo.attach(9);
  delay(50);
  servo.write(SERVO_ANGLE_LOW);
  delay(1000);
  // servo.write(SERVO_ANGLE_HIGH);
  // //delay(20);
  // servo.detach();
  // digitalWrite(13,LOW);
  delay(10);
  // servo.write(SERVO_ANGLE_LOW);
}

void servoUp(){
  digitalWrite(13,HIGH);
  //servo.attach(9);
  delay(50);
  servo.write(SERVO_ANGLE_HIGH);
  delay(1000);
  // servo.detach();
  // digitalWrite(13,LOW);
  delay(10);
}



void carForward(){
  analogWrite(6,150);
  digitalWrite(7,LOW);
}
void carBackward(){
  digitalWrite(6,LOW);
  analogWrite(7,150);
}
void carStop(){
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
}