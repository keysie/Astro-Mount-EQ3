// Set up interrupt on change for pin-group
void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}




void gearMotorSetup()
{
  // Pins
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  
  // Direction and Standby
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(STBY, HIGH);
  
  // Initial speed
  analogWrite(PWMA, 42*4);
}



/* Comments about the setup
Changing ICR1 will effect the amount of bits of resolution.
ICR1 = 0xffff; (65535) 16-bit resolution
ICR1 = 0x7FFF; (32767) 15-bit resolution
ICR1 = 0x3FFF; (16383) 14-bit resolution
ICR1 = 0x03FF; (1023)  10-bit resolution etc....

PB1 and COM1A1 are used for pin 9 which is not used in this sketch.

Changing the prescaler will effect the frequency of the PWM signal.
Frequency[Hz}=CPU/(ICR1+1) where in this case CPU=16 MHz
16-bit PWM will be>>> 16000000/(65535+1)=244,14Hz
*/
void setupPWM16() {
    DDRB |= _BV(PB2);                       /* set pins as outputs */
    TCCR1A = _BV(COM1B1)                    /* non-inverting PWM */
        | _BV(WGM11);                       /* mode 14: fast PWM, TOP=ICR1 */
    TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS11);                        /* prescaler 1 */
    ICR1 = 0x03ff;                          /* TOP counter value (freeing OCR1A*/
}





