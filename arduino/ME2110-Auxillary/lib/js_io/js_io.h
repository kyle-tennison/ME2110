#ifndef JS_IO_H
#define JS_IO_H

enum OpMode {
  BAG = 0,
  PWM = 1,
  BAG_ANIMATE = 2
};


/// @brief Inbound payload to Arduino
struct CommandPayload {
  bool to;
  bool go;
  bool db; 
  bool la; 
};

/// @brief Outbound payload to TS
struct TelemetryPayload {
  bool in1;
  bool in2;
  int16_t pot;
  OpMode op;
  };

void send_payload(TelemetryPayload* data);
CommandPayload read_incoming();

#endif