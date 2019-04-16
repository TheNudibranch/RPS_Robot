#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int ledPin = 4;
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

  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, ledState);
  if (radio.available()) {
    radio.read(&message, sizeof(message));
    Serial.println(message);
  }

  if (message == 1){
    ledState = !ledState;
    message = 0;
  }
  delay(100);
}
