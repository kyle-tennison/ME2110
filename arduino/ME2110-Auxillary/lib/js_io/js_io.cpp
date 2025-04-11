#include <ArduinoJson.h>
#include <js_io.h>
#include <common.h>

#define INPUT_BUFFER_SIZE 128

void send_payload(TelemetryPayload* data) {
  StaticJsonDocument<128> doc;  // Adjust capacity as needed.
  doc["in1"] = data->in1;
  doc["in2"] = data->in2;
  doc["pot"] = data->pot;
  doc["op"] = data->op;
  
  serializeJson(doc, Serial);
  Serial.println(); // Send terminator.
}

CommandPayload read_incoming() {
  debug_println("!Reading incoming message");

  char inputBuffer[INPUT_BUFFER_SIZE];
  memset(inputBuffer, 0, sizeof(inputBuffer));
  int bufferIndex = 0;

  // Wait for data.
  while (Serial.available() == 0) {}

  // Read input until newline or the buffer is nearly full.
  while (Serial.available() > 0 && bufferIndex < INPUT_BUFFER_SIZE - 1) {
    char ch = Serial.read();
    if (ch == '\n' || ch == '\r') {
      if (bufferIndex == 0) continue;  // Skip leading line breaks.
      else break;
    }
    inputBuffer[bufferIndex++] = ch;
  }
  inputBuffer[bufferIndex] = '\0';

  debug_print("!Final buffer is: ");
  debug_println(inputBuffer);

  // Create a document with a capacity appropriate for 4 keys.
  StaticJsonDocument<JSON_OBJECT_SIZE(4) + 40> doc;
  DeserializationError error = deserializeJson(doc, inputBuffer);
  if (error) {
    debug_print("!deserializeJson() failed: ");
    debug_println(error.c_str());
  }

  CommandPayload payload;
  payload.to = doc["to"] | false; // Defaults to false if missing.
  payload.go = doc["go"] | false;
  payload.db = doc["db"] | false;
  payload.la = doc["la"] | false;
  payload.dbl = doc["dbl"] | false;

  debug_println("!Successfully deserialized inbound command:");
  debug_print("!to: ");
  debug_println(payload.to);
  debug_print("!go: ");
  debug_println(payload.go);
  debug_print("!db: ");
  debug_println(payload.db);
  debug_print("!la: ");
  debug_println(payload.la);
  debug_print("!dbl:");
  debug_println(payload.dbl);

  return payload;
}
