const uint8_t codes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                         0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

bool oldState[] = {0,0};
bool currState[] = {0,0};
int speedCounter = 0;

void setup()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop()
{  
  int brightnessR = 1023-analogRead(0);
  bool isWhiteR = brightnessR > 512;
  int brightnessL = 1023-analogRead(1);
  bool isWhiteL = brightnessL > 512;
//   digitalWrite7Segment(codes[brightnessR/100] | (isWhiteR <<7));

  currState[0] = !digitalRead(2);
  currState[1] = !digitalRead(4);

  if(currState[0] && !oldState[0]){
    if(speedCounter<5){
      speedCounter++;
    }
  }
  if(currState[1] && !oldState[1]){
    speedCounter = 0;
  }
  set7SegmentValue(speedCounter);
  analogWrite(3, isWhiteR?speedCounter*50:0);
  analogWrite(5, isWhiteL?speedCounter*50:0);

  oldState[0] = currState[0];
  oldState[1] = currState[1];
  delay(10);
}

void set7SegmentValue(uint8_t value){
  digitalWrite7Segment(codes[value]);
}

void digitalWrite7Segment(uint8_t code){
  digitalWrite(6,code & 0x01);
  digitalWrite(7,code & 0x02);
  digitalWrite(8,code & 0x04);
  digitalWrite(9,code & 0x08);
  digitalWrite(10,code & 0x10);
  digitalWrite(11,code & 0x20);
  digitalWrite(12,code & 0x40);
  digitalWrite(13,code & 0x80);
}