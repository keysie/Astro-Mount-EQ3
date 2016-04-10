// Gear motor driver pins
#define STBY 12
#define AIN1 11
#define AIN2 13
#define PWMA 10

// Encoder pins
#define ENCA     2  // green
#define ENCB     3  // blue

// Motor Driver connection
// A01  brown
// A02  red

// I2C Connection (given by wire library)
// A04	SDA
// A05	SCL

// Quarz clock pins
#define FREQ       6

// Step up supply control pin
#define SUPPLY     7

// I2C
#define I2C_TARGET      8
#define OFFSET_HIGH     1
#define OFFSET_LOW      2
#define UPDATE_OFFSET   3

// Undervoltage limit
#define VLIMIT      7.5           //unit: V
#define ADLIMIT     63*VLIMIT


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
battery for a longer period of time.

To increase the accuracy of the position measurement, the 
magnet and the encoder have been moved from the worm-gear
axis to the axis of one of the cogs inside the gear which
connects the dc-motor with the worm-gear axis.

Because the timing could still be off by a few ticks, there
is now a I2C-link between the ardu and the PIC so that the
user can send timing corrections via serial input to the 
external clock board. Those will be saved in the EEPROM
there. The timing should be fine though, as the assembler-
code of the program has been analyzed to ensure the desired
timing is achieved as closely as possible. */


#include <Wire.h>
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
int kp = 9;
int ki = 2;
int integrator = 0;
int output = 0;



void setup()
{
  // very first thing: pull step-down enable pin low
  setupSupply();
  
  // set I2C pins to high for initialization
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  digitalWrite(A4, HIGH);
  digitalWrite(A5, HIGH);
  
  // interrupt on change
  pciSetup(2);
  pciSetup(3);
  pciSetup(6);
  
  // Gear motor
  gearMotorSetup();
  
  // set 10 bit pwm resolution on pin 10
  setupPWM16();
  
  // start serial once and have it running all the time
  // don't attempt to start it earlier, will not work!
  Serial.begin(115200);
  
  // join I2C bus as master
  Wire.begin();
  
  // introduce yourself
  serialIntro();
  
}

void loop()
{
  if (work == true)
  { // do controller work
  
    error = des_pos - encoder;
    
    integrator += error * ki;
    
    output = (error * kp + integrator) + 42*4;
    
    // limit oiutput to 0...1024
    if (output > 1024)
    {
      output = 1024;
    }
    else if (output < -1024)
    {
      output = -1024;
    }
    
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
    
    // integrator limit
    int intlimit = 200;
    if (integrator > intlimit)
    {
      integrator = intlimit;
    }
    else if (integrator < -intlimit)
    {
      integrator = -intlimit;
    }
    
    analogWrite16(10, output);
    
    //Serial.begin(115200);
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
    //Serial.end();
    
    if (analogRead(A0) < ADLIMIT)
    {
      undervolt_count++;
      
      if (undervolt_count > 100) // = ca 18 seconds
      {
        // Output message to serial port
        Serial.begin(115200);
        Serial.println("Undervoltage - Shutting Down NOW!");
        Serial.end();
        
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
  else
  {
    // nothing to do with controlling, so check if serial
    // data is available to read
    if (Serial.available() > 0)
    {
      // read transmitted data as int16
      int16_t newOffset = Serial.parseInt();
      int16_t returnedOffset = 0;
      
      // print out separator
      Serial.println("");
      Serial.println("========================================");
      
      // confirm reception of data
      Serial.print("Int received on Ardu: ");
      Serial.println(newOffset,DEC);
      
      // send data and read back to confirm
      Serial.print("Sending Offset: ");
      Serial.println(newOffset);
      
      I2C_sendOffset(newOffset);
    
      delayMicroseconds(1000);
      
      returnedOffset = I2C_readOffset();
      
      Serial.print("Returned Offset: ");
      Serial.println(returnedOffset);
      
      // check if data was transmitted correctly
      if (newOffset == returnedOffset)
      {
        Serial.println("Success!");
      }
      else
      {
        Serial.println("Something went wrong with I2C transmission!");
      }
      
      // print out separator
      Serial.println("========================================");
      Serial.println("");
      
      // give user time to read
      delay(1500);
    }
  }
}
