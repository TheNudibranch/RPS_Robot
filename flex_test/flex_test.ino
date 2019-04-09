// Code to test the flex sensor

int flexVal = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  flexVal = analogRead(A0);
  Serial.println(flexVal);
  delay(200);
}
