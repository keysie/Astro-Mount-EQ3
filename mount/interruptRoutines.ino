// handle pin change interrupt for D0 to D7 here
ISR (PCINT2_vect) 
 {
   // Read all three pins
   new_state_enc_A = digitalRead(ENCA);
   new_state_enc_B = digitalRead(ENCB);
   new_state_freq = digitalRead(FREQ);
   
   // Encoder interrupt?
   if (  state_enc_A != new_state_enc_A || 
         state_enc_B != new_state_enc_B    )
   {
     
     // Handle interrupt
     handleEncInterrupt();
   }
   
   // Clock interrupt?
   if (state_freq != new_state_freq)
   {
     // Update state
     state_freq = new_state_freq;
     
     // Handle Interrupt
     handleClkInterrupt();
   }
} 

void handleEncInterrupt()
{
  if (new_state_enc_A != state_enc_A &&
      new_state_enc_B == state_enc_B)
  { // A changed
    
    if (new_state_enc_A == HIGH && state_enc_A == LOW)
    { // LOW to HIGH on A
      
      if (state_enc_B == HIGH)
      { // Movement CW -> decrease
        encoder--;
      }
      else
      { // Movement CCW -> increase
        encoder++;
      }
      
    }
    else
    { // HIGH to LOW on A
    
      if (state_enc_B == HIGH)
      { // Movement CCW -> increase
        encoder++;
      }
      else
      { // Movement CW -> decrease
        encoder--;
      }
 
    }
  }
  
  else if (new_state_enc_B != state_enc_B &&
           new_state_enc_A == state_enc_A)
  { // B changed
    
    if (new_state_enc_B == HIGH && state_enc_B == LOW)
    { // LOW to HIGH on B
      
      if (state_enc_A == HIGH)
      { // Movement CCW -> increase
        encoder++;
      }
      else
      { // Movement CW -> decrease
        encoder--;
      }
      
    }
    else
    { // HIGH to LOW on B
    
      if (state_enc_A == HIGH)
      { // Movement CW -> decrease
        encoder--;
      }
      else
      { // Movement CCW -> increase
        encoder++;
      }
      
    }
  }
  
  // Update states
  state_enc_A = new_state_enc_A;
  state_enc_B = new_state_enc_B;
}
  

void handleClkInterrupt()
{
  // increase desired encoder position
  des_pos++;
  
  // tell main loop to do controlling work
  work = true;
}
