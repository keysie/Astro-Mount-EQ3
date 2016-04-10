
#define STBY 13
#define AIN1 12
#define AIN2 11
#define PWMA 10

void setup()
{
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN1, HIGH);
  digitalWrite(STBY, HIGH);
  analogWrite(PWMA, 0);
}

void loop()
{
  
}

