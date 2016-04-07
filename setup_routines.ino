#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <Encoder.h>


/* ============= Stepper-Setup ============== */

/* Configure the pins as outputs and configure
   the EasyDriver according to the configuration */
   
void setMicrostepping()
{
  // Set microstepping
  #if defined FULL
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, LOW);
  #elseif defined HALF
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, LOW);
  #elseif defined QUARTER
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, HIGH);
  #else
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, HIGH);
  #endif 
}

void stepperSetup()
{
  // Set all above pins as output
  pinMode(ENABLE, OUTPUT);
  pinMode(SLP, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  
  // Disable driver first
  digitalWrite(ENABLE, HIGH);
  
  // Hold device in sleep
  digitalWrite(SLP, LOW);
  
  // Set microstepping
  setMicrostepping();
  
  // Reset step
  digitalWrite(STEP, LOW);
  
  // Enable device (still in sleep)
  digitalWrite(ENABLE, LOW);
}





/* ============== Timer-Setup =============== */

/* Set up timer1 to create an interrupt every
   3.446592s to achieve 17.2s after 5 iterations
   
   This means for timer1: Prescaler:      1024
                          Compare-Value: 53852
                          
   Additionally, all unused peripherals are
   deactivated to conserve energy */   

void timerSetup()
{
  cli();//stop interrupts

  //set timer1 interrupt at every 3.44s
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register
  OCR1A = 53852;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);


  sei();//allow interrupts
  
  
   /* Disable all of the unused peripherals. This will reduce power
   * consumption further and, more importantly, some of these
   * peripherals may generate interrupts that will wake our Arduino from
   * sleep!
   */
   
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer2_disable();
  power_twi_disable();  

}




/* ============= Switch-Setup =============== */

/* A manual switch is placed over pins 5 and 7
   to enable the user to turn the device on and
   off without disconnecting power. For this,
   pin 5 is used as output and driven to LOW,
   while pin 7 is used as input and pulled up
   internally. When the switch is closed, pin 7
   will be pulled to low. */ 
   
 void switchSetup()
 {
   pinMode(5, OUTPUT);
   pinMode(7, INPUT_PULLUP);
   digitalWrite(5, LOW);
 }
