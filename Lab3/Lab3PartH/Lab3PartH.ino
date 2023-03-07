#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,10,11,12,13);

void setup()
{
  ADMUX &= ~_BV(REFS1);
  ADMUX |= _BV(REFS0);
  ADMUX &= ~_BV(ADLAR);
  ADCSRB &= ~_BV(ADTS2);
  ADCSRB &= ~_BV(ADTS1);
  ADCSRB &= ~_BV(ADTS0);
  
  lcd.begin(16,2);
}

void loop()
{
  
  updateLCD();
  
}

void updateLCD(){
  lcd.setCursor(0,0);
  lcd.print("ADC0 = ");
  uint16_t adc0 = readADC0();
  lcd.print(adc0);
  lcd.print("            ");
  
  lcd.setCursor(0,1);
  lcd.print("ADC15 = ");
  uint16_t adc15 = readADC15();
  lcd.print(adc15);
  lcd.print("            ");
}

uint16_t readADC0(){
  ADMUX &= ~_BV(REFS1);
  ADMUX |= _BV(REFS0);
  ADCSRB &= ~_BV(MUX5);
  ADMUX &= ~_BV(MUX4);
  ADMUX &= ~_BV(MUX3);
  ADMUX &= ~_BV(MUX2);
  ADMUX &= ~_BV(MUX1);
  ADMUX &= ~_BV(MUX0);
  
  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADSC);
  
  while( (ADCSRA >> ADSC) & 0x01 ){delay(1);}
  
  return ADC;
}

uint16_t readADC15(){
  ADMUX &= ~_BV(REFS1);
  ADMUX |= _BV(REFS0);
  ADCSRB |= _BV(MUX5);
  ADMUX &= ~_BV(MUX4);
  ADMUX &= ~_BV(MUX3);
  ADMUX |= _BV(MUX2);
  ADMUX |= _BV(MUX1);
  ADMUX |= _BV(MUX0);
  
  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADSC);
  
  while( (ADCSRA >> ADSC) & 0x01 ){delay(1);}
  
  return ADC;
}