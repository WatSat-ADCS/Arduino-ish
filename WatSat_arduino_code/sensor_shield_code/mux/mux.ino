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
  for( int i= 0; i< 4; i++)
    pinMode( controlPin[ i], OUTPUT); 
  for( int i= 0; i< 4; i++)
    digitalWrite( controlPin[ i], LOW); 

  Serial.begin(9600);
}

void loop(){
  //Loop through each of the 3 mux, and read values from each mux channel
  //Reports back Value at channel 6 is: 346
  for( int i= 0; i< 2; i++){
    Serial.print( "MUX "); Serial.print( i); Serial.print( ", ");
    for( int j= 0; j< 16; j++){
      for( int k= 0; k< 4; k++)
        digitalWrite( controlPin[ k], muxChannel[ j][ k]);
      Serial.print( "Channel "); Serial.print( j); Serial.print( ": "); Serial.println( analogRead( sigRead[ i]));
      delay( 250);
    }
  }
}
