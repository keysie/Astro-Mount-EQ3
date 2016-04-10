// Timer test

// Running continuously on one second intervalls
// to determine accuracy of internal clock

int seconds = 0;
int minutes = 0;
int hours = 0;

void setup()
{
  Serial.begin(9600);
}


void loop()
{
  Serial.print(hours);
  Serial.print("  ");
  Serial.print(minutes);
  Serial.print("  ");
  Serial.print(seconds);
  Serial.print("\n");
  
  seconds++;
  
  if (seconds >= 60)
  {
    minutes++;
    seconds = seconds - 60;
  }
  
  if (minutes >= 60)
  {
    hours++;
    minutes = minutes - 60;
  }
  
  delay(1000);
}

// Works within 1 sec accuracy over 40 minutes test.
