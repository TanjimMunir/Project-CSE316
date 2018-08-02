char bdata;

void setup() {
  Serial.begin(9600);
  pinMode(0,INPUT);//RXD pin is set for INPUT
  pinMode(1,OUTPUT);

}

void loop() {
  if(Serial.available() > 0)
  {
    bdata = Serial.read();
    Serial.print(bdata);
  }

}
