#include <ArduinoJson.h>
#include <js_io.h>
#include <common.h>

#define BUFFER_SIZE 512


void send_payload(TelemetryPayload* data) {
  JsonDocument doc;

  doc["in1"] = data->in1;
  doc["in2"] = data->in2;
  doc["pot"] = data->pot;
  doc["op"] = data->op;

  serializeJson(doc, Serial);
  Serial.println(); // Send \r\n to signal break
}


CommandPayload read_incoming() {
  debug_println("!Reading incoming message");

  char inputBuffer[BUFFER_SIZE];
  int bufferIndex = 0;

  while (Serial.available() > 0) {
    const char incomingByte = Serial.read();
    if (incomingByte == '\n' || incomingByte == '\r') {
      inputBuffer[bufferIndex] = '\0'; // Null-terminate the string
      break; // Stop reading further bytes.
    } else {
      if (bufferIndex < BUFFER_SIZE - 1) {
        inputBuffer[bufferIndex] = incomingByte;
        bufferIndex++;
      } else {
        bufferIndex = 0;
        debug_println("!Buffer overflow! Resetting...");
      }
    }
  }

  debug_print("!Final buffer is: ");
  debug_println(inputBuffer);

  char* json_string = &inputBuffer[0];


  JsonDocument doc;
  deserializeJson(doc, json_string, bufferIndex);

  CommandPayload payload;

  payload.to = doc["to"];
  payload.go = doc["go"];
  payload.db = doc["db"];
  payload.la = doc["la"];

  debug_println("!Sucessfully deserialized inbound command");

  return payload;
}