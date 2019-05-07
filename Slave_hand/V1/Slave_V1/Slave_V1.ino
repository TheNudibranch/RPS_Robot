#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <nRF24L01.h>
#include <RF24.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

// hardware SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

int gameArray[2] = {0,0};
char gameString[2] = "00";

long prevAccelTime = 0; // Last time accelerometer was triggered
float accelMin[2] = {0,0};
float accelMax[2] = {0,0};
float compX;
float compY;
float currentAccel[2] = {0,0};
int armState = 0;
int prevArmState = 0;

int middlePos = 0;
int middleMin = 0;
int middleMax = 0;
int ringMin = 0;
int ringMax = 0;
int ringPos = 0;
float compMiddle;
float compRing;



void setup() {
  rfSet();
  #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(9600);
  accelSet();
  setAccelRange();
  setFlex();
}

void loop() {
  accelRead(200);
  armFunc();
  updateGameArray();
  prevArmState = armState;
//  radioTest();
}


//////////////////////////////// Functions


void updateGameArray(){
  if ((armState != prevArmState) && armState == 0){
    Serial.println("yo");
    gameArray[0] += 1;
    Serial.println(gameArray[0]);
    if (gameArray[0] == 4){
      updateHandPos();
      updateGameString(gameArray[0]);
      Serial.println(gameString);
      radio.write(&gameString, sizeof(gameString));
      gameArray[0] = 0;
      gameString[1] = '0';
    }
    else{
      updateGameString(gameArray[0]);
      Serial.println(gameString);
      radio.write(&gameString, sizeof(gameString));
    }
  }
}

// Change the state of the led
// Basically, if the hand is up then the led should be on
void armFunc(){
  if ((currentAccel[0] > compX) && (currentAccel[1] > compY)){
    armState = 1;
  }
  else{
    armState = 0;
  }
}

// Set the max and min of the acceleromter for use
void setAccelRange(){
  Serial.println("Lower Arm");
  delay(5000);
  sensors_event_t event; 
  lis.getEvent(&event);
  accelMin[0] = event.acceleration.x;
  accelMin[1] = event.acceleration.y;
  Serial.print("Minimum ");
  Serial.print(accelMin[0]);
  Serial.print("      ");
  Serial.println(accelMin[1]);

  Serial.println("Raise Arm");
  delay(5000);
  lis.getEvent(&event);
  accelMax[0] = event.acceleration.x;
  accelMax[1] = event.acceleration.y;
  Serial.print("Maximum ");
  Serial.print(accelMax[0]);
  Serial.print("      ");
  Serial.println(accelMax[1]);

  compX = (accelMax[0] - accelMin[0]) / 2;
  compY = (accelMax[1] - accelMin[1]) / 2;
}

// Read the acceleromter and output the event every "delayTime" amount of time
void accelRead(int delayTime){
  if (millis() - prevAccelTime >= delayTime){
    sensors_event_t event; 
    lis.getEvent(&event);
//
//    Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
//    Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
//    Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
//    Serial.println(" m/s^2 ");
//
//    Serial.println();
    prevAccelTime = millis();
    currentAccel[0] = event.acceleration.x;
    currentAccel[1] = event.acceleration.y;
  }
}

// Set up code for the radio mod.
void rfSet(){
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

// Set up code for the accelerometer
void accelSet(){
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");
}

void setFlex(){
  Serial.println("Close Hand");
  delay(5000);
  middleMin = analogRead(A4);
  ringMin = analogRead(A5);
  Serial.println("Open Hand");
  delay(5000);
  middleMax = analogRead(A4);
  ringMax = analogRead(A5);
  compMiddle = (middleMax - middleMin) / 2;
  compRing = (ringMax - ringMin) / 2;
  Serial.println(compMiddle);
  Serial.println(compRing);
  
}

void updateHandPos(){
  // Let 1 be rock, 2 be paper, and 3 be scicorss
  middlePos = analogRead(A4);
  ringPos = analogRead(A5);
  int normMiddle = middlePos - middleMin;
  int normRing = ringPos - ringMin;

  if ((normMiddle < compMiddle) && (normRing < compRing)){
    Serial.println("Rock");
    gameString[1] = '1';
  }
  else if ((normMiddle > compMiddle) && (normRing > compRing)){
    Serial.println("Paper");
    gameString[1] = '2';
  }
  else if ((normMiddle > compMiddle) && (normRing < compRing)){
    Serial.println("Scicorss");
    gameString[1] = '3';
  }
}

void updateGameString(int gState){
  switch (gState){
    case 0:
    gameString[0] = '0';
    break;
    
    case 1:
    gameString[0] = '1';
    break;

    case 2:
    gameString[0] = '2';
    break;

    case 3:
    gameString[0] = '3';
    break;

    case 4:
    gameString[0] = '4';
    break;
  }
}
