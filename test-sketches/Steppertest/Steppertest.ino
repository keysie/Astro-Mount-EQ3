
#define ENABLE    8    // HIGH -> FETs disabled
#define SLP       9    // LOW -> FETs disabled, power saving
#define MS1      10    // L,L -> Full | H,L -> Half
#define MS2      11    // L,H -> Quarter | H,H -> Eigth
#define STEP     12    // L to H will trigger step
#define DIR      13    // Directions

short serialData = 0;

void setup()
{
  // Init serial for debug
  Serial.begin(9600);
  
  // Set all above pins as output
  pinMode(ENABLE, OUTPUT);
  pinMode(SLP, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  
  // Disable driver first
  digitalWrite(ENABLE, HIGH);
  
  // Set microstepping to eight
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  
  // Hold device in sleep
  digitalWrite(SLP, LOW);
  
  // Set direction
  digitalWrite(DIR, HIGH);
  
  // Reset step
  digitalWrite(STEP, LOW);
  
  // Enable device (still in sleep)
  digitalWrite(ENABLE, LOW);
  
  // disable sleep
  digitalWrite(SLP, HIGH);
  
}


void loop()
{
  if (Serial.available() > 0)
  {
    serialData = Serial.read();
    
    // KEY 1
    if (serialData >= 50 && serialData <= 58)
    {
      for (int i = 0; i<serialData-49; i++)
      {
        // Trigger one step
        digitalWrite(STEP, HIGH);
        digitalWrite(STEP, LOW);
        
        Serial.println("Stepping");
      }
    }
    
    // KEY 0
    else if (serialData == 48)
    {
      // enable sleep
      digitalWrite(SLP, LOW); 
      
      Serial.println("Sleep ON");
    }
    
    // KEY 1
    else if (serialData == 49)
    {
      // disable sleep
      digitalWrite(SLP, HIGH); 
      
      Serial.println("Sleep OFF");
    }
    
    // ALL OTHER KEYS
    else
    {
      Serial.println("INVALID");
    }
  }
}
