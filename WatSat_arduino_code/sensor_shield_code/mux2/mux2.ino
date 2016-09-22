// 74HC4067 demultiplexer demonstration (1 to 16)
 
// control pins output table in array form
// see truth table on page 2 of TI 74HC4067 data sheet
// connect 74HC4067 S0~S3 to Arduino D7~D4 respectively
// 5V to 74HC4067 pin 1 to power the LEDs :)
byte controlPins[] = {B00000000, 
                      B10000000,
                      B01000000,
                      B11000000,
                      B00100000,
                      B10100000,
                      B01100000,
                      B11100000,
                      B00010000,
                      B10010000,
                      B01010000,
                      B11010000,
                      B00110000,
                      B10110000,
                      B01110000,
                      B11110000 }; 
 
void setup()
{
  DDRD = B11111111; // set PORTD (digital 7~0) to outputs
}
 
void setPin(int outputPin)
// function to select pin on 74HC4067
{
  PORTD = controlPins[outputPin];
}
 
void loop()
{
  for (int i = 0; i < 16; i++)
  {
    setPin(i);
    delay(250);
  }
}
