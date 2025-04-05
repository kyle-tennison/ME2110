#ifndef JS_IO_H
#define JS_IO_H

struct Payload {
    int val;
    float tmp;
  };

void send_payload(Payload* data);
void read_incoming();

#endif