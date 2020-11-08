void setup() {
  pinMode(2, OUTPUT);
  pinMode(16, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);   
  digitalWrite(16, HIGH);
  delay(1000);                       
  digitalWrite(2, HIGH);  
  digitalWrite(16, HIGH);  
  delay(1000);                       
}
