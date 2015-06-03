#include <Wire.h>
#include <SBoard.h>
/*
high pwm means low voltage
low pwm means high voltage
*/

int NUM_BOARDS = 6;

// output pins for controlling the mux
int control0 = 2;
int control1 = 3;
int control2 = 4;
int control3 = 5;

// input pins for reading the mux
int in0 = 0;
int in1 = 1;

// output pins for controlling the magnetorquers
int torx = 6;
int tory = 7;
int torz = 8;
int torred = 9;

SBoard boards[6];
Data d0;
Data master [6];

char req;

void setup() {
  Serial.begin(9600);
  
  pinMode(control0, OUTPUT); 
  pinMode(control1, OUTPUT); 
  pinMode(control2, OUTPUT); 
  pinMode(control3, OUTPUT); 
  
  pinMode(in0, INPUT);
  pinMode(in1, INPUT);
  
  pinMode(torx, OUTPUT); 
  pinMode(tory, OUTPUT); 
  pinMode(torz, OUTPUT); 
  pinMode(torred, OUTPUT); 
  
  Serial.println("Started serial");
  init_stuff();
}

void loop() {
  while (Serial.available() > 0){
    listenForRequest();
  }
  
}

void listenForRequest(){
  req = Serial.read();
  
  if (req == 'g'){
    getRequest();
  }else if (req == 'p'){
    postRequest();
  }
}

void postRequest(){
  /*
  What to do when a "post" request is sent.
  For example, will post the voltage required to apply to magentorquers
  */
    delay(100);
    int arguments[4];
    String input = Serial.readString();
    //verify valid string here (?)
    String dp = "";
    int argind = 0;
    // start at 1 b/c string always starts and ends with ( and ) respectively 
    for (int i=1; i<input.length()-1; i++) {
      if (input[i] != ','){
        dp += input[i];
      }
      if (input[i+1] == ',' || input[i+1] == ')') {
        Serial.println(dp);
        arguments[argind] = dp.toInt();
        dp = "";
        argind ++;
      }
    }

    for (int i = 0; i<4; i++) {
      Serial.print(arguments[i]);
    }
    
  // process input in the form '[1,1,1,1]'
  adjust(arguments[0], arguments[1], arguments[2], arguments[3]);
}

void getRequest(){
  /*
  What to do when a "get" request is sent.
  For example, will gather from all the sensors and pass that to comp using serial por
  */
 readall();
 outputall();
}

void init_stuff()
{
  // TODO: define all the sensors/boards
  Sensor p0 = {0, 0000};
  Sensor p1 = {0, 0001};
  Sensor tmp = {0, 0010};
  Sensor mxm = {0, 0011};
  Sensor mxd = {0, 0100};
  Sensor mym = {0, 0101};
  Sensor myd = {0, 0110};
  boards[0] = SBoard(p0, p1, tmp, mxm, mxd, mym, myd, 
    control0, control1, control2, control3);
  boards[1] = SBoard(p0, p1, tmp, mxm, mxd, mym, myd, 
    control0, control1, control2, control3);
  boards[2] = SBoard(p0, p1, tmp, mxm, mxd, mym, myd, 
    control0, control1, control2, control3);
  boards[3] = SBoard(p0, p1, tmp, mxm, mxd, mym, myd, 
    control0, control1, control2, control3);
  boards[4] = SBoard(p0, p1, tmp, mxm, mxd, mym, myd, 
    control0, control1, control2, control3);
  boards[5] = SBoard(p0, p1, tmp, mxm, mxd, mym, myd, 
    control0, control1, control2, control3);
    
}

void readall(){
  // SOLDERING NOTE: MAKE SURE TO LINE UP OUTPUTS!!
  // can fix this by giving a board another var that describes it's input pin
  for (int i=0; i<(NUM_BOARDS/2); i++){
    master[i] = boards[i].readData();
    master[i+1] = boards[i+1].readData();
  }
}

void outputall(){
  for (int i=0; i<NUM_BOARDS; i++){
    Serial.print(i);
    Serial.print("{photodiodes:");
    Serial.print(master[i].pda);
    Serial.print("; mag_x:");
    Serial.print(master[i].ax);
    Serial.print("; mag_y:");
    Serial.print(master[i].ay);
    Serial.print(";}"); 
  }
  Serial.print("\n");
}

void adjust(float magx, float magy, float magz, float magred){
  analogWrite(torred, magred);
  analogWrite(torz, magz);
  analogWrite(tory, magy);
  analogWrite(torx, magx);
}
