// blink with SFRs

#define DDR_LED DDRB
#define PORT_LED PORTB
#define BIT_LED 7


void setup()
{
  DDR_LED |= _BV(BIT_LED);
}

void loop()
{
  PORT_LED |= _BV(BIT_LED);
  delay(1000); // Wait for 1000 millisecond(s)
  PORT_LED &= ~_BV(BIT_LED);
  delay(1000); // Wait for 1000 millisecond(s)
}