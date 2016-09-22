#include <Wire.h>
char WHO_AM_I = 0x00;
char SMPLRT_DIV= 0x15;
char DLPF_FS = 0x16;
char GYRO_XOUT_H = 0x1D;
char GYRO_XOUT_L = 0x1E;
char GYRO_YOUT_H = 0x1F;
char GYRO_YOUT_L = 0x20;
char GYRO_ZOUT_H = 0x21;
char GYRO_ZOUT_L = 0x22;
char DLPF_CFG_0 = (1<<0);
char DLPF_CFG_1 = (1<<1);
char DLPF_CFG_2 = (1<<2);
char DLPF_FS_SEL_0 = (1<<3);
char DLPF_FS_SEL_1 = (1<<4);

char itgAddress = 0x69;
#define magAddress 0x1E 
//Arduino D8, D9, D10 and D11 pins send a 1/0 to the mux s0, s1, s3 and s4 pins to select the binary channel on the mux
int controlPin[ 4] ={ 8, 9, 10, 11};

//The binary values used to select specific channel
int muxChannel[16][4]={
  {0,0,0,0}, //channel 0
  {1,0,0,0}, //channel 1
  {0,1,0,0}, //channel 2
  {1,1,0,0}, //channel 3
  {0,0,1,0}, //channel 4
  {1,0,1,0}, //channel 5
  {0,1,1,0}, //channel 6
  {1,1,1,0}, //channel 7
  {0,0,0,1}, //channel 8
  {1,0,0,1}, //channel 9
  {0,1,0,1}, //channel 10
  {1,1,0,1}, //channel 11
  {0,0,1,1}, //channel 12
  {1,0,1,1}, //channel 13
  {0,1,1,1}, //channel 14
  {1,1,1,1}  //channel 15
};

//signal "SIG" pin present on the mux is read by the Arduino on A0, A1 or A2; each analogue input is unique to a specific mux
//3 mux are used on the sensor shield
int sigRead[3] = {0, 1, 2};


void setup(){
  Serial.begin(9600);
  
  //Initialize the I2C communication. This will set the Arduino up as the 'Master' device.
  Wire.begin();
  
  //Configure the gyroscope
  //Set the gyroscope scale for the outputs to +/-2000 degrees per second
  itgWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));
  //Set the sample rate to 100 hz
  itgWrite(itgAddress, SMPLRT_DIV, 9);
  
  //Put the HMC5883 magnetometer IC into the correct operating mode
 Wire.beginTransmission(magAddress); //open communication with HMC5883
 Wire.write(0x02); //select mode register
 Wire.write(0x00); //continuous measurement mode
 Wire.endTransmission();
}

void loop(){
  //Loop through each of the 3 mux, and read values from each mux channel
  for( int i= 0; i< 1; i++){
    Serial.print( "MUX "); Serial.print( i); Serial.print( ", ");
    for( int j= 0; j< 2; j++){
      for( int k= 0; k< 4; k++)
        digitalWrite( controlPin[ k], muxChannel[ j][ k]);
      if( j== 0)
      {
        //Create variables to hold the output rates.
        int xRate, yRate, zRate;
      
        //Read the x,y and z output rates from the gyroscope.
        xRate = readX();
        yRate = readY();
        zRate = readZ();
      
        //Print the output rates to the terminal, seperated by a TAB character.
        Serial.println( "GYRO:");
        Serial.print(xRate);
        Serial.print('\t');
        Serial.print(yRate);
        Serial.print('\t');
        Serial.println(zRate);
        delay( 250);
      }
      else if( j==1)
      {
          int x,y,z; //triple axis data

          //Tell the HMC5883L where to begin reading data
          Wire.beginTransmission(magAddress);
          Wire.write(0x03); //select register 3, X MSB register
          Wire.endTransmission();
          
         
          //Read data from each axis, 2 registers per axis
          Wire.requestFrom(magAddress, 6);
          if(6<=Wire.available()){
            x = Wire.read()<<8; //X msb
            x |= Wire.read(); //X lsb
            z = Wire.read()<<8; //Z msb
            z |= Wire.read(); //Z lsb
            y = Wire.read()<<8; //Y msb
            y |= Wire.read(); //Y lsb
          }
          
          //Print out values of each axis
          Serial.println( "MAGNETOMETER:");
          Serial.print("x: ");
          Serial.print(x);
          Serial.print("  y: ");
          Serial.print(y);
          Serial.print("  z: ");
          Serial.println(z);
          delay( 250);
      }
      
      Serial.print( "Channel "); Serial.print( j); Serial.print( ": "); Serial.println( analogRead( sigRead[ 4]));
      delay( 250);
    }
  }
}

//This function will write a value to a register on the itg-3200.
//Parameters:
//  char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
//  char registerAddress: The address of the register on the sensor that should be written to.
//  char data: The value to be written to the specified register.
void itgWrite(char address, char registerAddress, char data)
{
  //Initiate a communication sequence with the desired i2c device
  Wire.beginTransmission(address);
  //Tell the I2C address which register we are writing to
  Wire.write(registerAddress);
  //Send the value to write to the specified register
  Wire.write(data);
  //End the communication sequence
  Wire.endTransmission();
}

//This function will read the data from a specified register on the ITG-3200 and return the value.
//Parameters:
//  char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
//  char registerAddress: The address of the register on the sensor that should be read
//Return:
//  unsigned char: The value currently residing in the specified register
unsigned char itgRead(char address, char registerAddress)
{
  //This variable will hold the contents read from the i2c device.
  unsigned char data=0;
  
  //Send the register address to be read.
  Wire.beginTransmission(address);
  //Send the Register Address
  Wire.write(registerAddress);
  //End the communication sequence.
  Wire.endTransmission();
  
  //Ask the I2C device for data
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 1);
  
  //Wait for a response from the I2C device
  if(Wire.available()){
    //Save the data sent from the I2C device
    data = Wire.read();
  }
  
  //End the communication sequence.
  Wire.endTransmission();
  
  //Return the data read during the operation
  return data;
}

//This function is used to read the X-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second. 
//Usage: int xRate = readX();
int readX(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_XOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_XOUT_L);  
  
  return data;
}

//This function is used to read the Y-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second. 
//Usage: int yRate = readY();
int readY(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_YOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_YOUT_L);  
  
  return data;
}

//This function is used to read the Z-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second. 
//Usage: int zRate = readZ();
int readZ(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_ZOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_ZOUT_L);  
  
  return data;
}
