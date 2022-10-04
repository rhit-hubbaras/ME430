// define __AVR_ATmega2560__

#define REG_DDR_BUZZER DDRH
#define REG_PORT_BUZZER PORTH
#define REG_PIN_BUZZER PINH
#define BIT_BUZZER 5

#define REG_DDR_BUTTON_C DDRE
#define REG_PORT_BUTTON_C PORTE
#define REG_PIN_BUTTON_C PINE
#define BIT_BUTTON_C 4

#define REG_DDR_BUTTON_D DDRE
#define REG_PORT_BUTTON_D PORTE
#define REG_PIN_BUTTON_D PINE
#define BIT_BUTTON_D 5

#define REG_DDR_BUTTON_E DDRG
#define REG_PORT_BUTTON_E PORTG
#define REG_PIN_BUTTON_E PING
#define BIT_BUTTON_E 5

#define REG_DDR_BUTTON_G DDRE
#define REG_PORT_BUTTON_G PORTE
#define REG_PIN_BUTTON_G PINE
#define BIT_BUTTON_G 3

#define REG_DDR_BUTTON_ACTIVE DDRL
#define REG_PORT_BUTTON_ACTIVE PORTL
#define REG_PIN_BUTTON_ACTIVE PINL
#define BIT_BUTTON_ACTIVE 7

#define REG_DDR_ACTIVE_BUZZER DDRL
#define REG_PORT_ACTIVE_BUZZER PORTL
#define REG_PIN_ACTIVE_BUZZER PINL
#define BIT_ACTIVE_BUZZER 6


void setup()
{
  REG_DDR_BUZZER |= _BV(BIT_BUZZER);
  REG_DDR_BUTTON_C &= ~_BV(BIT_BUTTON_C);
  REG_DDR_BUTTON_D &= ~_BV(BIT_BUTTON_D);
  REG_DDR_BUTTON_E &= ~_BV(BIT_BUTTON_E);
  REG_DDR_BUTTON_G &= ~_BV(BIT_BUTTON_G);
  REG_DDR_BUTTON_ACTIVE &= ~_BV(BIT_BUTTON_ACTIVE);
  REG_DDR_ACTIVE_BUZZER |= _BV(BIT_ACTIVE_BUZZER);
  
  REG_PORT_BUTTON_C |= _BV(BIT_BUTTON_C);
  REG_PORT_BUTTON_D |= _BV(BIT_BUTTON_D);
  REG_PORT_BUTTON_E |= _BV(BIT_BUTTON_E);
  REG_PORT_BUTTON_G |= _BV(BIT_BUTTON_G);
  REG_PORT_BUTTON_ACTIVE |= _BV(BIT_BUTTON_ACTIVE);
}

void loop()
{
  if (!(REG_PIN_BUTTON_C & _BV(BIT_BUTTON_C))) {
    tone(8, 523, 100); // play tone 60 (C5 = 523 Hz)
  }
  if (!(REG_PIN_BUTTON_D & _BV(BIT_BUTTON_D))) {
    tone(8, 587, 100); // play tone 61 (D5 = 587 Hz)
  }
  if (!(REG_PIN_BUTTON_E & _BV(BIT_BUTTON_E))) {
    tone(8, 659, 100); // play tone 62 (E5 = 659 Hz)
  }
  if (!(REG_PIN_BUTTON_G & _BV(BIT_BUTTON_G))) {
    tone(8, 784, 100); // play tone 62 (E5 = 659 Hz)
  }

  if (!(REG_PIN_BUTTON_ACTIVE & _BV(BIT_BUTTON_ACTIVE))) {
    REG_PORT_ACTIVE_BUZZER |= _BV(BIT_ACTIVE_BUZZER);
  }else{
    REG_PORT_ACTIVE_BUZZER &= ~_BV(BIT_ACTIVE_BUZZER);
  }
  delay(10); // Delay a little bit to improve simulation performance

}