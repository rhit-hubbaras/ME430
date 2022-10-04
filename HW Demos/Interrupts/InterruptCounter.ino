#define REG_DDR_BUTTON_PRINT DDRE
#define REG_PORT_BUTTON_PRINT PORTE
#define REG_PIN_BUTTON_PRINT PINE
#define BIT_BUTTON_PRINT 5

#define REG_DDR_BUTTON_COUNT DDRE
#define REG_PORT_BUTTON_COUNT PORTE
#define REG_PIN_BUTTON_COUNT PINE
#define BIT_BUTTON_COUNT 4

volatile uint16_t isrCounter = 0;
volatile byte mainEventFlags = 0x00;

void setup()
{
  REG_DDR_BUTTON_COUNT &= ~_BV(BIT_BUTTON_COUNT);
  REG_DDR_BUTTON_PRINT &= ~_BV(BIT_BUTTON_PRINT);
  
  REG_PORT_BUTTON_COUNT |= _BV(BIT_BUTTON_COUNT);
  REG_PORT_BUTTON_PRINT |= _BV(BIT_BUTTON_PRINT);
  
  Serial.begin(9600);
  
  EIMSK |= _BV(INT4);
  EICRB |= _BV(ISC41);
  EICRB &= ~_BV(ISC40);
  
  sei();
}


void loop()
{
  if(!(REG_PIN_BUTTON_PRINT >> BIT_BUTTON_PRINT & 1)){
    Serial.println(String("ISR Counter: ")+isrCounter);
    delay(500);
  }
  if((mainEventFlags << 0) & 0x01){
    delay(30);
    if(!((REG_PIN_BUTTON_COUNT << BIT_BUTTON_COUNT) & 0x01)){
      isrCounter++;
    }
    mainEventFlags &= ~_BV(0);
  }
}



ISR(INT4_vect){
  mainEventFlags |= _BV(0);
}