// This is the code for the arduino that possses the button

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int button  = 4;
int butState = 0;
int prevButState = 0;

const int message = 1;
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  butState = digitalRead(button);
  const char text[] = "Hello World";
  if ((prevButState == LOW) && (butState == HIGH)){
    radio.write(&message, sizeof(message));
  }
  prevButState = butState;
  delay(100);
}
