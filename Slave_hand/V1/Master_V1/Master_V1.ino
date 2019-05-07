#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo middle;
Servo ring;
Servo pinky;
Servo index;
Servo thumb;

int servo_ar[5] = {10,9,6,5,3};
int mode = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);


int ledState = 0;
int message = 0;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void loop() {
  getGameState();
}

//////////////////////////////////////////// Functions

void getGameState(){
  if (radio.available()) {
  char text[32] = "";
  radio.read(&text, sizeof(text));
  Serial.println(text);
  }
}
