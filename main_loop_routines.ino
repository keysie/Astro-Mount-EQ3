/* =============== SwitchedOn =============== */

/* Return true, if the mechanical switch is 
   closed. */
   
boolean switchedOn()
{
  if (digitalRead(7) == LOW)
    {
      return true;
    }
    else
    {
      return false;
    }
}




/* =========== Get Angular Error ============ */

/* Read encoder-object, determine the current 
   angular position based on that number and
   then return the difference between the 
   desired and the current position in deg. */
   
float getAngularError()
{
  return desPosDeg - 1.40625 * enc.read();
}





/* ================ Do Steps ================ */

/* Trigger the EasyDriver to perform the given
   number of steps. Based on the chosen micro-
   stepping, the number of steps is increased.
   Based on the sign of count, the direction to
   turn is determined. 
   
   !count is assumed to be a multiple of 4! */

void doSteps(int count)
{
  // sanity check
  if (count == 0)
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
    #define MULTI 1      // no multiplier needed
  #elseif defined HALF
    #define MULTI 2  // two microsteps per step
  #elseif defined QUARTER
    #define MULTI 4  // four microsteps per step
  #else
    #define MULTI 8  // eight microsteps per step
  #endif
  
  // multiply count
  // -4 because of acceleration and breaking
  count = (count-4) * MULTI;
  
  // Make sure microstepping is still set
  setMicrostepping();
  
  // Turn off stdby
  digitalWrite(SLP, HIGH);
  
  // Do 2 full steps with increasing speed
  for (int i = 0; i < 2 * MULTI; i++)
  {
    // Trigger one step
    digitalWrite(STEP, HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP, LOW);
    delayMicroseconds(3000 - 125 * i * (9-MULTI));
  }
  
  // Do calculated number of steps
  for (int i = 0; i < count; i++)
  {
    // Trigger one step
    digitalWrite(STEP, HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP, LOW);
    delayMicroseconds(1000);
  }
  
  // Do 2 full steps with decreasing speed
  for (int i = 0; i < 2 * MULTI; i++)
  {
    // Trigger one step
    digitalWrite(STEP, HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP, LOW);
    delayMicroseconds(3000 + 125 * (i+1) * (9-MULTI));
  }
  
  // Turn on stdby
  digitalWrite(SLP, LOW);
  
  // To disable LED on pin 13 and conserve
  // energy, set 13 to low temporarily
  digitalWrite(13, LOW);
  
}






/* ============= Reset Positions ============ */

/* Reset the desired angular position as well as
   the current position of the encoder object */
   
void resetPositions()
{
  desPosDeg = 0;
  enc.write(0); 
}





/* ================= Sleep ================== */

/* Enter SLEEP_MODE_IDLE to conserve energy. A
   Lower power mode is not possible as to not
   deactivate timer1. */

void sleep()
{
  /* Choose sleep mode */
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  sleep_enable();

  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the timer timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */

}




/* ==== Millisecond Delay w/o interrupt ===== */

/* The method delay(XX) uses interrupts, which
   causes problems. delayMicroseconds uses a 
   loop, but accepts max. 16000. This method
   extends this interrupt free delay method. */

void delayms(int ms)
{
  for (int i = 0; i < ms; i++)
  {
    delayMicroseconds(1000);
  }
}
