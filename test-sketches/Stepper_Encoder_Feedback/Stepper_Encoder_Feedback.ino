#include <Encoder.h>

// CONFIGURATION


// Stepping (uncomment only one)

//#define FULL
//#define HALF
//#define QUARTER
#define EIGTH


// Error margin allowed 
// (measured in encoder steps)

#define MARGIN 5


// Define Pins of Stepper

#define ENABLE    8    // HIGH -> FETs disabled
#define SLP       9    // LOW -> FETs disabled, power saving
#define MS1      10    // L,L -> Full | H,L -> Half
#define MS2      11    // L,H -> Quarter | H,H -> Eigth
#define STEP     12    // L to H will trigger step
#define DIR      13    // Directions






// ============== BEGIN OF PROGRAM ================


// store for desired position
int desPos = 0;

// counter for serial communication
int n = 0;

// error of position
int error = 0;

// Set up encoder object
Encoder enc(2,3);



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



void setup()
{
  // Setup pins for stepper
  stepperSetup();
  
  // Reset Encoder Position
  enc.write(0);
  
  // Init serial comm
  Serial.begin(9600); 
  
}


//void loop()
//{
//  
//  if (Serial.available() > 0)
//  {
//    serialData = Serial.read();
//    
//    // STEPPING
//    if (serialData >= 49 && serialData <= 100)
//    {
//      // disable sleep and wait a moment
//      digitalWrite(SLP, HIGH);
//      delay(20);
//      
//      
//      for (int i = 0; i<serialData-48; i++)
//      {
//        // Trigger one step
//        digitalWrite(STEP, HIGH);
//        delay(20);
//        digitalWrite(STEP, LOW);
//        
//        Serial.println("Stepping");
//      }
//      
//      // Reenable sleep
//      digitalWrite(SLP, LOW);
//    }
//  }
//  
//  Serial.println(enc.read());
//  
//  delay(100);
//}

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
    digitalWrite(STEP, LOW);
  }
  
  // Turn on stdby
  digitalWrite(SLP, LOW);
  
}

void loop()
{
  // every n-th time check for new desired
  // position via serial
  if (n == 1000)
  {
    if (Serial.available() > 0)
    {
      desPos = Serial.read() - 48;
    }
    
    // Print error
    Serial.println(error);
    
    // Reset counter
    n = 0;
  }
  
  // Calculate error
  error = desPos - enc.read();
  
  // Loop until error is small enough
  if (error > MARGIN || -error > MARGIN)
  {
    
    // one step
    if (error > 0)
    {
      doStep(1);
    }
    else
    {
      doStep(-1);
    }
    
  }
  
  // increase counter for serial
  n++;
  
}
