
void setup(){
Serial.begin(9600);
}

void loop() {
  int analogread=0;
  int digread=0;
  analogread=analogRead(A0); 
  digread=digitalRead(5);

  analogread=(analogread-264)/5.9;
  Serial.println(digread);   
  Serial.println(analogread); 
  delay(150);
}
