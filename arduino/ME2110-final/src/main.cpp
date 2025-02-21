#include "Arduino.h"
#include "myDuino.h"


myDuino* client;


void setup(){
  Serial.begin(9600);
  client = new myDuino(1);
}


void loop(){
  if (client->readButton(2) == 1){
    client->digital(2, 1);
    Serial.println("on");
  }
  else{
    client->digital(2, 0);
    Serial.println("off");
  }
}