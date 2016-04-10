#include <Wire.h>

#define I2C_TARGET      8

#define OFFSET_HIGH     1
#define OFFSET_LOW      2
#define UPDATE_OFFSET   3

void setup()
{
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
}

void I2C_sendByte(uint8_t device, uint8_t address, uint8_t data)
{
  Wire.beginTransmission(device);  // transmit to device X
  
  Wire.write(address);    // send address or command first
  Wire.write(data);       // send data after that
  
  Wire.endTransmission();
}

uint8_t I2C_readByte(uint8_t device, uint8_t address)
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission();
  
  delayMicroseconds(1000); // wait 1 ms
  
  Wire.requestFrom(device, (byte)1);  // request 1 byte
  
  while (Wire.available())
  {
    return Wire.read();
  }
}

void I2C_sendOffset(int16_t value)
{
  // send offset MSBs
  I2C_sendByte(I2C_TARGET, OFFSET_HIGH, value >> 8);
  
  // send offset LSBs
  I2C_sendByte(I2C_TARGET, OFFSET_LOW, value & 0xFF);
  
  // send order to update the offset
  I2C_sendByte(I2C_TARGET, UPDATE_OFFSET, 0x00);
}

int16_t I2C_readOffset()
{
  uint8_t OffsetH = I2C_readByte(I2C_TARGET, OFFSET_HIGH);
  uint8_t OffsetL = I2C_readByte(I2C_TARGET, OFFSET_LOW);
  
  return (OffsetH << 8) + OffsetL;
}


void loop()
{
  int Offset = 517;
  int OffsetReturn = 0;
  
  Serial.println("");
  
  Serial.println("Reading stored offset...");
  OffsetReturn = I2C_readOffset();
  
  Serial.print("Returned Offset: ");
  Serial.println(OffsetReturn);
  
  Serial.println("Enter any integer to send now...");
  Serial.setTimeout(10);

  // wait for user to enter a value
  while (1)
  {
    if (Serial.available() > 0)
    {
      Offset = Serial.parseInt();
      break;
    }
  }
  
  Serial.setTimeout(1000);
  
  Serial.print("Sending Offset: ");
  Serial.println(Offset);
  
  I2C_sendOffset(Offset);

  delayMicroseconds(1000);
  
  OffsetReturn = I2C_readOffset();
  
  Serial.print("Returned Offset: ");
  Serial.println(OffsetReturn);
  
 
  while(1==1)
  {}
}
