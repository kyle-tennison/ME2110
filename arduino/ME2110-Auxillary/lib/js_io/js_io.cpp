#include <ArduinoJson.h>
#include <js_io.h>
#include <common.h>

#define BUFFER_SIZE 128


void send_payload(Payload* data) {
  JsonDocument doc;

  doc["val"] = data->val;
  doc["tmp"] = data->tmp;

  serializeJson(doc, Serial);
}


void read_incoming() {
  Serial.println("Reading incoming message");

  char inputBuffer[BUFFER_SIZE];
  int bufferIndex = 0;

  while (Serial.available() > 0){

    const char incomingByte = Serial.read();

    if (incomingByte == '\n' || incomingByte == '\r') {
      inputBuffer[bufferIndex] = '\0'; // Null-terminate the string
    }
    else {
      // Store the byte in the buffer if there's space
      if (bufferIndex < BUFFER_SIZE - 1) {
        inputBuffer[bufferIndex] = incomingByte;
        bufferIndex++;
      } else {
        // If buffer is full, reset to start again (optional)
        bufferIndex = 0;
        debug_println("Buffer overflow! Resetting...");
      }
    }
  }

  debug_print("Final buffer is: ");
  debug_println(inputBuffer);

  char* json_string = &inputBuffer[0];


  JsonDocument doc;
  deserializeJson(doc, json_string, bufferIndex);



}