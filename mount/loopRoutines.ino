/* 16-bit version of analogWrite(). Works only on pins 9 and 10. */
// EDIT: Only working on Pin 10
void analogWrite16(uint8_t pin, uint16_t val)
{
    switch (pin) {
        //case  9: OCR1A = val; break;
        case 10: OCR1B = val; break;
    }
}





