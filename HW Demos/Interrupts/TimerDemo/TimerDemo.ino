#include <LiquidCrystal.h>

#define DDR_BUTTON_GREEN DDRD
#define PORT_BUTTON_GREEN PORTD
#define PIN_BUTTON_GREEN PIND
#define BIT_BUTTON_GREEN 2
#define EICR_BUTTON_GREEN EICRA
#define ISC0_BUTTON_GREEN ISC20
#define ISC1_BUTTON_GREEN ISC21
#define INT_BUTTON_GREEN INT2
#define INTvect_BUTTON_GREEN INT2_vect

#define DDR_BUTTON_YELLOW DDRD
#define PORT_BUTTON_YELLOW PORTD
#define PIN_BUTTON_YELLOW PIND
#define BIT_BUTTON_YELLOW 3
#define EICR_BUTTON_YELLOW EICRA
#define ISC0_BUTTON_YELLOW ISC30
#define ISC1_BUTTON_YELLOW ISC31
#define INT_BUTTON_YELLOW INT3
#define INTvect_BUTTON_YELLOW INT3_vect

#define DDR_BUTTON_RESET DDRD
#define PORT_BUTTON_RESET PORTD
#define PIN_BUTTON_RESET PIND
#define BIT_BUTTON_RESET 0
#define EICR_BUTTON_RESET EICRA
#define ISC0_BUTTON_RESET ISC00
#define ISC1_BUTTON_RESET ISC01
#define INT_BUTTON_RESET INT0
#define INTvect_BUTTON_RESET INT0_vect

#define DDR_LED_GREEN DDRH
#define PORT_LED_GREEN PORTH
#define PIN_LED_GREEN PINH
#define BIT_LED_GREEN 3

#define DDR_LED_YELLOW DDRE
#define PORT_LED_YELLOW PORTE
#define PIN_LED_YELLOW PINE
#define BIT_LED_YELLOW 3

LiquidCrystal lcd(8,9,10,11,12,13);

bool isGreenTimerRunning = false;
uint16_t greenCounter = 0; //timer 2 with 0.001s interrupts
#define TIMER_2_START 0
#define TIMER_2_END 250

bool isYellowTimerRunning = false;
uint16_t yellowCounter = 0; //timer 1 with 0.1s interrupts
#define TIMER_1_START 0
#define TIMER_1_END 25000

#define DEBOUNCE_TIME 20
volatile uint16_t delayCounter = 0;

volatile byte mainFlags = 0x00;

void setup()
{
  DDR_BUTTON_GREEN &= ~_BV(BIT_BUTTON_GREEN);
  DDR_BUTTON_YELLOW &= ~_BV(BIT_BUTTON_YELLOW);
  DDR_BUTTON_RESET &= ~_BV(BIT_BUTTON_RESET);
  PORT_BUTTON_GREEN |= _BV(BIT_BUTTON_GREEN);
  PORT_BUTTON_YELLOW |= _BV(BIT_BUTTON_YELLOW);
  PORT_BUTTON_RESET |= _BV(BIT_BUTTON_RESET);
  DDR_LED_GREEN |= _BV(BIT_BUTTON_GREEN);
  DDR_LED_YELLOW |= _BV(BIT_BUTTON_YELLOW);
  
  
  EICR_BUTTON_GREEN &= ~_BV(ISC0_BUTTON_GREEN);
  EICR_BUTTON_GREEN |= _BV(ISC1_BUTTON_GREEN);
  EICR_BUTTON_YELLOW &= ~_BV(ISC0_BUTTON_YELLOW);
  EICR_BUTTON_YELLOW |= _BV(ISC1_BUTTON_YELLOW);
  EICR_BUTTON_RESET &= ~_BV(ISC0_BUTTON_RESET);
  EICR_BUTTON_RESET |= _BV(ISC1_BUTTON_RESET);
  EIMSK |= _BV(INT_BUTTON_GREEN);
  EIMSK |= _BV(INT_BUTTON_YELLOW);
  EIMSK |= _BV(INT_BUTTON_RESET);

  TIMSK2 |= _BV(OCIE2A);
  TCCR2A = 0x00;
  TCCR2B |= _BV(CS22);
  TCCR2B &= ~_BV(CS21);
  TCCR2B &= ~_BV(CS20);
  TCNT2 = TIMER_2_START;
  OCR2A = TIMER_2_END;
  
  TIMSK1 |= _BV(OCIE2A);
  TCCR1A = 0x00;
  TCCR1B &= ~_BV(CS12);
  TCCR1B |= _BV(CS11);
  TCCR1B |= _BV(CS10);
  TCNT1 = TIMER_1_START;
  OCR1A = TIMER_1_END;
  
  sei();
  
  lcd.begin(16,2);
  lcd.print("Andrew Hubbard");
  lcd.setCursor(0,1);
  lcd.print("G=   Y=   T=");
}

void loop()
{
  if((mainFlags >> 0) & 0x01){
    blockingDelay(DEBOUNCE_TIME);
    if(!((PIN_BUTTON_GREEN >> BIT_BUTTON_GREEN) & 0x01)){
      PORT_LED_GREEN ^= _BV(BIT_LED_GREEN);
      isGreenTimerRunning=!isGreenTimerRunning;
    }
    mainFlags &= ~_BV(0);
  }
  if((mainFlags >> 1) & 0x01){
    blockingDelay(DEBOUNCE_TIME);
    if(!((PIN_BUTTON_YELLOW >> BIT_BUTTON_YELLOW) & 0x01)){
      PORT_LED_YELLOW ^= _BV(BIT_LED_YELLOW);
      isYellowTimerRunning=!isYellowTimerRunning;
    }
    mainFlags &= ~_BV(1);
  }
  if((mainFlags >> 2) & 0x01){
    blockingDelay(DEBOUNCE_TIME);
    if(!((PIN_BUTTON_RESET >> BIT_BUTTON_RESET) & 0x01)){
      PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
      PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
      isGreenTimerRunning = false;
      isYellowTimerRunning = false;
      greenCounter = 0;
      yellowCounter = 0;
    }
    mainFlags &= ~_BV(2);
  }
  
  updateLCD();
}

void updateLCD(){
  
  lcd.setCursor(0,0);
  uint8_t gTimerSec = greenCounter / 1000;
  uint8_t gTimerTenth = (greenCounter / 100) % 10;
  lcd.print(gTimerSec);
  lcd.print(".");
  lcd.print(gTimerTenth);
  lcd.print("            ");
  
  lcd.setCursor(0,1);
  if(isYellowTimerRunning){
    lcd.print("            ");
  }else{
    uint8_t yTimerSec = yellowCounter / 10;
    uint8_t yTimerTenth = yellowCounter % 10;
    lcd.print(yTimerSec);
    lcd.print(".");
    lcd.print(yTimerTenth);
  	lcd.print("            ");
  }
}

void blockingDelay(uint16_t delayms){
  delayCounter = delayms;
  while (delayCounter > 0){}
}

ISR(INTvect_BUTTON_GREEN){
  mainFlags |= _BV(0);
}
ISR(INTvect_BUTTON_YELLOW){
  mainFlags |= _BV(1);
}
ISR(INTvect_BUTTON_RESET){
  mainFlags |= _BV(2);
}
ISR(TIMER2_COMPA_vect){
  TCNT2 = TIMER_2_START;
  if(isGreenTimerRunning){
    greenCounter++;
  }
  if(delayCounter > 0){
    delayCounter--;
  }
}
ISR(TIMER1_COMPA_vect){
  TCNT1 = TIMER_1_START;
  if(isYellowTimerRunning){
    yellowCounter++;
  }
}