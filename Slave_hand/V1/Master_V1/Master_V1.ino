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

int servo_ar[5] = {10,9,6,5};
int mode = 0;

char text[32] = "";
char prevText[2] = "00";

LiquidCrystal_I2C lcd(0x27, 16, 2);


int ledOn = 0;
int message = 0;
int winner = 0;
int ledPin = 4;
int ledPinComp = 3;

int butPin = 2;
int butState = 0;
int prevButState = 0;
int lcdOn = 0;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  middle.attach(servo_ar[0]);
  ring.attach(servo_ar[1]);
  pinky.attach(servo_ar[2]);
  index.attach(servo_ar[3]);

  pinMode(ledPin, OUTPUT);
  pinMode(butPin, INPUT);
  pinMode(ledPinComp, OUTPUT);
  lcd.noBacklight();
  lcd.noDisplay();

}

void loop() {
  checkButState();
  history();
  getGameState();
  updateLCD();
  endGame();
  ledFunc();
  prevText[0] = text[0];
  prevButState = butState;
}

//////////////////////////////////////////// Functions

void endGame() {
  if ((text[0] == '4') && (prevText[0] != text[0])){
    delay(1000);
    endLCD();
    int compChoice = random(1,4);
    servoFunc(compChoice);
    endLCDComp(compChoice);
    whoWon(compChoice);
    ledSelect(winner);
  }
  else if (text[0] != '4'){
    ledOn = 0;
    servoFunc(2);
  }
}

void history(){
  if (analogRead(A3) > 800){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Last Winner:");
    lcd.setCursor(0,1);
    if (winner == 1){
      lcd.print("Human");
    }
    else if (winner == 2){
      lcd.print("Computer");
    }
    else if (winner == 3){
      lcd.print("TIE");
    }
    delay(1500);
  }

}

void whoWon(int compChoice){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Winner:");
  lcd.setCursor(0,1);
  if (((text[1] == '1') && (compChoice == 1)) || ((text[1] == '2') && (compChoice == 2 )) || ((text[1] == '3') && (compChoice == 3 ))){
    lcd.print("TIE!");
    winner = 3;
  }
  else if (((text[1] == '1') && (compChoice == 3)) || ((text[1] == '2') && (compChoice == 1)) || ((text[1] == '3') && (compChoice == 2))){
    lcd.print("HUMAN!");
    winner = 1;
  }
  else{
    lcd.print("COMPUTER!");
    winner = 2;
  }
  delay(1500);
}

void servoFunc(int choice){
    if (choice == 2){
    // Nothing happens or paper
    index.write(5);
    middle.write(5);
    ring.write(5);
    pinky.write(5);
  }
  
  else if (choice == 1){
    // Rock
    index.write(160);
    middle.write(160);
    ring.write(160);
    pinky.write(160);
  }

  else if (choice == 3){
    // Scissors
    index.write(5);
    middle.write(5);
    ring.write(160);
    pinky.write(160);
  }
}

void ledFunc(){
  if (winner == 1){
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPinComp, LOW);
  }
  else if (winner == 2){
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPinComp, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPinComp,LOW);
  }
}

void checkButState(){
  butState = digitalRead(butPin);
  if ((butState ==1) && (prevButState == 0)){
    lcdOn = !lcdOn;
      if (lcdOn == 1){
      lcd.backlight();
      lcd.display();
      lcd.setCursor(0,0);
      lcd.print("Let's Play");
      lcd.setCursor(0,1);
      lcd.print("RPS!");
      delay(1500);
    }
    else if (lcdOn == 0){
      lcd.noBacklight();
      lcd.noDisplay();
    }
  }

}

void ledSelect(int winner){
  if (winner == 1){
    // The human won
    ledOn = 1;
  }
  else if (winner == 2){
    ledOn = 0;
  }
}

void getGameState(){
  if (radio.available()) {
  radio.read(&text, sizeof(text));
  Serial.println(text[0]);
  }
}

void updateLCD(){
  if (text[0] != prevText[0]){
    if (text[0] == '1'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ROCK");
    }
    else if (text[0] == '2'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Paper");
    }
    else if (text[0] == '3'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Scissors");
    }
    else if (text[0] == '4'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("SHOOT");
    }
  }
}

void endLCD() {
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Human:");
  lcd.setCursor(0,1);
  
  if (text[1] == '1'){
    lcd.print("ROCK");
  }
  else if (text[1] == '2'){
    lcd.print("PAPER");
  }
  else if (text[1] == '3'){
    lcd.print("SCISSORS");
  }
  delay(1500);
}
void endLCDComp(int choice){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Computer:");
  lcd.setCursor(0,1);

  if (choice == 1){
    lcd.print("ROCK");
  }
  else if (choice == 2){
    lcd.print("PAPER");
  }
  else if (choice == 3){
    lcd.print("SCISSORS");
  }
  delay(1500);
}
