#ifndef JS_IO_H
#define JS_IO_H

enum OpMode {
  BAG = 0,
};


/// @brief Inbound payload to Arduino
struct CommandPayload {
  bool top_ore_collector;
  bool ground_ore_collector;
  bool dog_bone; 
};

/// @brief Outbound payload to TS
struct TelemetryPayload {
  bool in1;
  bool in2;
  uint16_t pot;
  OpMode op;
  };

void send_payload(TelemetryPayload* data);
CommandPayload read_incoming();

#endif