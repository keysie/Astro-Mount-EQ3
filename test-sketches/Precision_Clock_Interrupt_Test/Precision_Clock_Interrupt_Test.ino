boolean toggle = true;

void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

//ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
// {    
//     digitalWrite(13,digitalRead(8) and digitalRead(9));
// }
// 
//ISR (PCINT1_vect) // handle pin change interrupt for A0 to A5 here
// {
//     digitalWrite(13,digitalRead(A0));
// }  
 
ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here
 {
   if (toggle == true)
   {
     digitalWrite(13,HIGH);
     toggle = false;
   }
   else
   {
     digitalWrite(13,LOW);
     toggle = true;
   }
 } 
 
 void setup()
 {
   pinMode(6,INPUT);
   
   pciSetup(6);
   
   pinMode(13,OUTPUT);
 }
 
 void loop()
 {
   
 }
