// This is the code for the slave arduino
// The code here reads accel and transmits LED state
// This is just a test of multiple SPI devices

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


const int butPin  = 4; // Button pin
int butState = 0; // What is the current state of the button
int prevButState = 0; // What was the last state of the button?
int transmitBool = 0; // Can the radio transmit?
long prevAccelTime = 0; // Last time accelerometer was triggered
long prevRadioTime = 0; // Last time radio was triggered

const int message = 1;
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  rfSet();
  #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(9600);
  accelSet();
}

void loop() {
  buttUpdate();
  transmitMessage(transmitBool, 500);
  accelRead(2000);
}
////////////////////////////////////// Functions

// Main function to read the accelerometer, takes the time between each reading
void accelRead(int delayTime){
  if (millis() - prevAccelTime >= delayTime){
    sensors_event_t event; 
    lis.getEvent(&event);

    Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
    Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
    Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
    Serial.println(" m/s^2 ");

    Serial.println();
    prevAccelTime = millis();
  }
}

// Update the state of the button and whether the radio can transmit
void buttUpdate(){
  butState = digitalRead(butPin);
  if ((prevButState == LOW) && (butState == HIGH)){
    transmitBool = 1;
  }
  else{
    transmitBool = 0;
  }
  prevButState = butState;
}

// Transmission message for the RF mod.
void transmitMessage(int transmit, int delayTime){
  if ((transmit == 1) && (millis() - prevRadioTime >= delayTime)){
    radio.write(&message, sizeof(message));
    prevRadioTime = millis();
    transmitBool = 0;
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
