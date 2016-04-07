#include <avr/sleep.h>
#include <avr/power.h>
#include <Encoder.h>



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




/* ============== Description ================ */

/* Software now incorporates encoder feedback.
   Based on the same timer1 interrupt timing as
   v0.1, the interrupt now only increases a
   counter. The rest is done in the main loop in
   order to allow encoder- and other interrupts to
   work propperly. They are not detected while the
   ardu is handling an interrupt vector.
   
   STEPPER PINNING CHANGED!
   
   Every 17.2s the desired angular position is
   increased by 7.2°. Using the encoder data, the
   current position is obtained and the error
   between the two is calculated. This error is 
   then used to determine how many 4-step-groups
   the motor has to advance.
   
   If the desired position can not be reached, the
   software will enter an indefinite error loop that
   will flash the on-board LED. Camera-control and 
   battery-checking are not yet implemented.
   
   To minimize vibrations in the mount, the stepper
   increased speed for two full steps and then breaks
   for two steps at the end. In between it moves fast.
   This makes doSteps only viable for a count that is
   a multiple of 4.





/* =============== Variables ================= */

short counter = 0;    // counting timer-interrupts
Encoder enc(2,3);     // encoder connected to pins
                      // 2 and 3
                      
float desPosDeg = 0;  // Desired angular position [deg]
float errPosDeg = 0;  // Error in ang. position [deg]

int stepGrps = 0;     // Number of 4-step groups required
int retryCounter = 0; // Number of times reaching the
                      // desired position has been tried
                      
                      
                      
/* ================= Setup ================== */

/* Prepare the EasyDriver for the stepper, set
   up timer1 to generate the required interrupts
   and configure pins for the mechanical switch */

void setup()
{
  stepperSetup();
  
  timerSetup();
  
  switchSetup();
}



/* =========== Timer1 Interrupt ============= */

/* Every 5th time the interrupt occurs, the
   interrupt counter has to be increased by
   one. This interrupt will wake the device
   from sleep and cause the main loop to be
   run. There, the rest of the program is
   handled. */
   
ISR(TIMER1_COMPA_vect)
{
  counter++;
}





/* =============== Main Loop ================ */

/* First two forks are switched in order compared
   to the paper layout. This is to allow the
   switch to be checked every 3.4s instead of 
   every 17s. */

void loop()
{
  if (switchedOn())
  { // SWITCH IS ON
  
    if (counter >= 5)
    { // COUNTER IS >= 5
    
      // Reset counter
      counter = 0;
      
      // Increase desired angular position
      desPosDeg += 7.2;
      
      // Calc angular error based on current position
      errPosDeg = getAngularError();
      
      // Try to reach the desired position 3 times
      while (1==1)
      {
        
        // Calc how many groups of four full steps
        // are required to reach the desired pos-
        // ition.
        stepGrps = round(errPosDeg / 7.2);
        
        // Advance the calculated steps
        doSteps(stepGrps * 4);
        
        // Calc angular error again
        errPosDeg = getAngularError();
        
        // Check if desired position is reached
        if (errPosDeg < 7.2)
        { // YES
          
          // Reset retry counter
          retryCounter = 0;
          
          // End while loop
          break;
        }
        
        // Position is NOT reached, to increment
        // retry counter
        retryCounter++;
        
        // Check if this was already the third 
        // attempt to reach the position.
        if (retryCounter >=3)
        { // YES
          
          // Disable stepper driver
          digitalWrite(ENABLE, HIGH);
          
          // Enter indefinite error loop
          while (1==1)
          {
            // Blink LED on pin 13
            digitalWrite(13, HIGH);
            delayms(200);
            digitalWrite(13, LOW);
            delayms(200);
          }
        } // NO: Try again
       
      } // End of retry loop
    } // End of counter if
  } // End of switchedOn if
  
  else
  { // SWITCH IS OFF
  
    resetPositions();  
  }
  
  sleep();
}
