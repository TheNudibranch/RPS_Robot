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

long prevAccelTime = 0; // Last time accelerometer was triggered
float accelMin[2] = {0,0};
float accelMax[2] = {0,0};
float compX;
float compY;
float currentAccel[2] = {0,0};
int ledOn = 0;
int pastLedOn = 0;

void setup() {
  rfSet();
  #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(9600);
  accelSet();
  setAccelRange();
}

void loop() {
  accelRead(200);
  ledFunc();
  transmitLed();
}


//////////////////////////////// Functions

void transmitLed(){
  if (ledOn != prevLedOn){
    radio.write(&ledOn, sizeof(ledOn));
  }
}

// Change the state of the led
// Basically, if the hand is up then the led should be on
void ledFunc(){
  if ((currentAccel[0] > compX) && (currentAccel[1] > compY)){
    ledOn = 1;
  }
  else{
    ledOn = 0;
  }
  Serial.println(ledOn);
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
