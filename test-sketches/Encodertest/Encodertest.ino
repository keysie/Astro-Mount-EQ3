#include <Encoder.h>

// Set up encoder object
Encoder enc(2,3);


// Init serial comm
void setup()
{
 Serial.begin(9600); 
}

// Main loop
void loop()
{
  
  // Output current enc reading
  Serial.println(enc.read());
  // Pause 200ms
  delay(200);
}

// -> Läuft!
