#include <avr/sleep.h>
#include <avr/power.h>

/* ============== Description ================ */

/* Stepper can only be put to sleep mode every
   four full steps, otherwise it will fall back
   the next time sleep is deactivated.
   
   The mount-gear has 100 teeth, so a rate of
   1:100 is applied to the angular velocity.
   
   To achieve the desired angular velocity of
   7.292115E-5 rad/s or 0.004178 deg/s the motor
   has to turn 100 times faster, with 
   w_m = 0.4178 deg/s.
   
   Each step of the motor is 1.8°, so four steps
   are equal to phi = 7.2°.
   
   phi / w_m = 17.232820127s is the time, after
   which the stepper has to move four steps forward.
   
   Because timer1 can only reach a delay of about
   8 seconds, several timer interrupts have to occur
   and a counter is required. The chosen solution is
   to have an interrupt every 3.446592s, which
   results in 4.9999 iterations for 17.2... seconds.
   
   This means for timer1: Prescaler:      1024
                          Compare-Value: 53852
                          
   To conserve power, the stepper is put to sleep
   whenever it is not moving, and the arduino is
   put in a power-saving mode (IDLE). Lower modes
   are not possible because then timer1 would stop.
   
   A manual switch is incorporated to allow the user
   to stop the motor from turning.
   
   The entire code is interrupt-driven. The main loop
   is only used to put the device to sleep once there
   is no more interrupt-code to be computed.





/* ============= Configuration =============== */


// Microstepping (FULL|HALF|QUARTER|EIGTH)
#define EIGTH


// Define Pins of Stepper
#define ENABLE   12    // HIGH -> FETs disabled
#define SLP       8    // LOW -> FETs disabled, power saving
#define MS1       9    // L,L -> Full | H,L -> Half
#define MS2      13    // L,H -> Quarter | H,H -> Eigth
#define STEP     10    // L to H will trigger step
#define DIR      11    // Directions





/* =============== Variables ================= */

short counter = 0;    // counting timer-interrupts





/* ================ Program ================= */

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
  
  // Reset step
  digitalWrite(STEP, LOW);
  
  // Enable device (still in sleep)
  digitalWrite(ENABLE, LOW);
}


void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void powerSetup()
{
  
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


void setup()
{
  stepperSetup();
  
  powerSetup();
  
  /* Setup Pin 6 as interrupt on change
     pin, so that the external clock 
     board can be used. */
  pinMode(6, INPUT);
  pciSetup(6);
  
  /* Setup for manual switch; Pin 5 is
    driven to LOW, pin 7 is pulled up
    internally. The switch will connect
    pins 5 and 7 and thus pull 7 to LOW
    when active. */
  pinMode(5, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  digitalWrite(5, LOW);
}



void doStep(int count)
{
  // sanity check
  if (count == 0 || count > 200)
  {
    return;
  }
  
  // Set turn direction depending on count sign
  if (count > 0)
  {
    digitalWrite(DIR, HIGH);
  }
  else
  {
    count = count * (-1);
    digitalWrite(DIR, LOW);
  }
  
  // Calculate number of steps required based on
  // chosen microstepping setting
  #if defined FULL
    count = count;      // no multiplier needed
  #elseif defined HALF
    count = count * 2;  // two microsteps per step
  #elseif defined QUARTER
    count = count * 4;  // four microsteps per step
  #else
    count = count * 8;  // eight microsteps per step
  #endif
  
  // Turn off stdby
  digitalWrite(SLP, HIGH);
 
  // Do calculated number of steps
  for (int i = 0; i < count; i++)
  {
    // Trigger one step
    digitalWrite(STEP, HIGH);
    delayMicroseconds(5000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(5000);
  }
  
  // Turn on stdby
  digitalWrite(SLP, LOW);
  
}


   
ISR(PCINT2_vect)
{
  counter++;
  
  if (counter == 1)
  {
    
    doStep(4);
    
    counter = 0;
  }
}


void loop()
{
  /* Choose sleep mode */
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  sleep_enable();

  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the timer timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */

}
