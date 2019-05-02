// Code to test the flex sensor

int flexValMiddle = 0;
int flexValRing = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  flexValMiddle = analogRead(A0);
  flexValRing = analogRead(A1);
  Serial.print("Middle Finger Value:");
  Serial.print(flexValMiddle);
  Serial.print("\t Ring Finger Value:");
  Serial.println(flexValRing);
  delay(200);
}
