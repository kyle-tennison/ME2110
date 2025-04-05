import { SerialPort } from "serialport";
import { EventEmitter } from "events";
import { ReadlineParser } from "@serialport/parser-readline";

interface Payload {
  val: number;
  tmp: number;
}
export const serialEmitter = new EventEmitter();

const port = new SerialPort({
  path: "/dev/cu.usbmodem1101",
  baudRate: 9600,
  autoOpen: true,
});

port.on("open", () => {
  console.log("Serial port opened");

  setTimeout(() => {
    console.log("Sending to arduino")
    sendToArduino(`{"test": "abcd"}`)
  }, 10000)
});

const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));
parser.on("data", (data: string) => {
  console.log(`Incoming data: ${data}`)
  // const payload: Payload = JSON.parse(data);
  // console.log('Received Payload:', payload);
  // serialEmitter.emit("data", payload);
});

export function sendToArduino(data: string) {
  if (port.isOpen) {
    port.write(data, (err) => {
      if (err) {
        console.error("Error writing to serial port:", err);
      }
    });
  } else {
    console.log("Serial port not open.");
  }
}



