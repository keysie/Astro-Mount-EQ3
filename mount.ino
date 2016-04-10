// Gear motor driver pins
#define STBY 12
#define AIN1 11
#define AIN2 13
#define PWMA 10

// Encoder pins
#define ENCA     2
#define ENCB     3

// Quarz clock pins
#define FREQ       6

// Step up supply control pin
#define SUPPLY     7


/*                DESCRIPTION

Even the smallest possible stepping of the stepper-motor 
is still to big for astrophotography, because there is no 
gear in between the motor and the worm-axis. Therefore 
replaced by dc motor with a gear. System is now in full 
feedback control loop with PI controller based on 
experiments and a MATLAB model. Sensor is still 256-tick 
encoder directly mounted to worm-axis.

Arduino now outputs a steady stream of controller info 
such as desired vs measured position etc. on serial on 
115200 baud. It also measures the battery voltage via
an analog in hooked to a voltage divider to determine
when the connected battery is empty. It then stops
automatically and goes to deep sleep.

To conserve more energy, the aruino uno onboard linear
voltage regulator has been removed physically and been
replaced by a separate DC/DC step-down converter module.
This module has been modified to only run for a short 
period of time after power-on, and then shut-down and
inhibit itself if the arduino does not immediately pull
down the module's enable pin. 

This configuration, enables the arduino to push said enable
pin to high in case the battery voltage drops too low. 
The module then shuts down and the entire system is - 
and stays - powered off until the user disconnects the
battery for a longer period of time. */


#include <avr/sleep.h>
#include <avr/power.h>


// Variables
bool state_enc_A;       // old states of all 3 pins
bool state_enc_B;
bool state_freq;

bool new_state_enc_A;   // new states of all 3 pins
bool new_state_enc_B;
bool new_state_freq;

bool work = false;      // tell main loop to do work

byte undervolt_count = 0;  //count how often below 11.4V

long int encoder = 0;   // measured encoder position
long int des_pos = 0;   // desired encoder position

int error = 0;          // controller variables
int kp = 5;
int ki = 1;
int integrator = 0;
int output = 0;



void setup()
{
  // very first thing: pull step-down enable pin low
  setupSupply();
  
  // interrupt on change
  pciSetup(2);
  pciSetup(3);
  pciSetup(6);
  
  // Gear motor
  gearMotorSetup();
  
  // set 10 bit pwm resolution on pin 10
  setupPWM16();
  
}

void loop()
{
  if (work == true)
  { // do controller work
  
    error = des_pos - encoder;
    
    integrator += error * ki;
    
    output = (error * kp + integrator) *10 + 42*4;
    
    // reverse motor-direction if output is negative
    if (output > 0)
    { // normal direction
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
    }
    else
    { // reverse direction
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
    }
    
    // integrator limits
    if (integrator > 10)
    {
      integrator = 10;
    }
    else if (integrator < -10)
    {
      integrator = -10;
    }
    
    analogWrite16(10, output);
    
    Serial.begin(115200);
    Serial.print(des_pos);
    Serial.print('\t');
    Serial.print(encoder);
    Serial.print('\t');
    Serial.print(error);
    Serial.print('\t');
    Serial.print(integrator);
    Serial.print('\t');
    Serial.print(output);
    Serial.print('\t');
    Serial.print(analogRead(A0));
    Serial.print('\n');
    Serial.end();
    
    if (analogRead(A0) < 721)
    {
      undervolt_count++;
      
      if (undervolt_count > 10)
      {
        // set motor driver to sleep
        digitalWrite(STBY, LOW);
        
        // make sure LED on pin 13 is out
        digitalWrite(13, LOW);
        
        // disable step down module
        // this will terminate 5V supply!
        digitalWrite(SUPPLY, LOW);
        
        // disable interrupts
        cli();
        
        // enter deep sleep (won't wake up again from here)
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
      }
    }
    else if (undervolt_count > 0)
    {
      undervolt_count--;
    }
      
    work = false;
  }
}
