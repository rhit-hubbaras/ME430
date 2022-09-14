const uint8_t codes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                         0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

void setup()
{
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
  for(int i=0;i<=15;i++){
    set7SegmentValue(i);
    delay(500); // Wait for 1000 millisecond(s)
  }
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