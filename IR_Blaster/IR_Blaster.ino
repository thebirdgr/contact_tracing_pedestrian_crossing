void setup()
{
  Serial.begin(9600);

  pinMode(13, OUTPUT);
}

void loop()
{
  char sense = digitalRead(2);
  Serial.println(sense);
  
  if(sense == HIGH){
    digitalWrite(13, sense);
    Serial.println("Motion Detected");
  }
  else{
    digitalWrite(13, LOW);
  } 
  delay(1000); // Wait for 1000 millisecond(s)
}
