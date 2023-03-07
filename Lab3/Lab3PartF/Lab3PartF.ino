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

volatile uint16_t greenCounter = 0;
volatile uint16_t yellowCounter = 0;

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
  
  sei();
  
  lcd.begin(16,2);
  lcd.print("Andrew Hubbard");
  lcd.setCursor(0,1);
  lcd.print("G=   Y=   T=");
}

void loop()
{
  if((mainFlags >> 0) & 0x01){
    delay(30);
    if(!((PIN_BUTTON_GREEN >> BIT_BUTTON_GREEN) & 0x01)){
      greenCounter++;
      PORT_LED_GREEN |= _BV(BIT_LED_GREEN);
      PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
    }
    mainFlags &= ~_BV(0);
  }
  if((mainFlags >> 1) & 0x01){
    delay(30);
    if(!((PIN_BUTTON_YELLOW >> BIT_BUTTON_YELLOW) & 0x01)){
      yellowCounter++;
      PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
      PORT_LED_YELLOW |= _BV(BIT_LED_YELLOW);
    }
    mainFlags &= ~_BV(1);
  }
  if((mainFlags >> 2) & 0x01){
    delay(30);
    if(!((PIN_BUTTON_RESET >> BIT_BUTTON_RESET) & 0x01)){
      greenCounter = 0;
      yellowCounter = 0;
      PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
      PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
      
      lcd.setCursor(0,1);
      lcd.print("G=0  Y=0  T=");

    }
    mainFlags &= ~_BV(2);
  }
  
  updateLCD();
  
  delay(10);
}

void updateLCD(){
  
  lcd.setCursor(2,1);
  lcd.print(greenCounter);
  
  lcd.setCursor(7,1);
  lcd.print(yellowCounter);
  
  lcd.setCursor(12,1);
  lcd.print(millis()/1000);
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