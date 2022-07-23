#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  
  if(mySerial.available() > 1){//Read from  OSOYOO UART LoRa wireless module and send to serial monitor
    String input = mySerial.readString();
    Serial.println(input);    
  }
}
