void setup() {
  Serial.begin(9600);
}
 
void loop() {
  while(Serial.available()) {
   char incoming = Serial.read();
   if(incoming == 'i'){
    Serial.print("works");
   }
   Serial.print(incoming);
  }
}

/*
type in the following into terminal to get remote communication to work:
ls /dev/tty.*
screen /dev/tty.usbmodem1421 9600 
*/
