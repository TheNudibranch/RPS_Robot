// Test for the three different modes of the hand
// Adding LCD to test I2c

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
void setup() {
  lcd.backlight();
  lcd.init();
  middle.attach(servo_ar[0]);
  ring.attach(servo_ar[1]);
  pinky.attach(servo_ar[2]);
  index.attach(servo_ar[3]);
  thumb.attach(servo_ar[4]);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()){
    int inter = Serial.parseInt();
    if (inter != 0){
      mode = inter;
    }
  } 
  lcd.setCursor(0,0);
  lcd.print("Halp");
  changeFunctions(mode);
  Serial.println(mode);
}

//////////////////// Functions

void changeFunctions(int mode){
  if (mode == 1){
    // Nothing happens or paper
    index.write(5);
    middle.write(5);
    ring.write(5);
    pinky.write(5);
    thumb.write(5);
  }
  
  else if (mode == 2){
    index.write(160);
    middle.write(160);
    ring.write(160);
    pinky.write(160);
    thumb.write(120);
  }

  else if (mode == 3){
    index.write(5);
    middle.write(5);
    ring.write(160);
    pinky.write(160);
    thumb.write(90);
  }
}
