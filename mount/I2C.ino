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
