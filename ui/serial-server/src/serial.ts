import { SerialPort } from "serialport";
import { EventEmitter } from "events";
import { ReadlineParser } from "@serialport/parser-readline";
import dotenv from "dotenv";

// Load environment variables from the .env file
dotenv.config();

interface Payload {
  val: number;
  tmp: number;
}

export const serialEmitter = new EventEmitter();

// Read the DEVICE_PATH from the environment variables
const devicePath = process.env.DEVICE_PATH || "/dev/cu.usbmodem1101";  // Fallback to a default path if not set

const port = new SerialPort({
  path: devicePath,
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
  const payload: Payload = JSON.parse(data);
  console.log('Received Payload:', payload);
  serialEmitter.emit("data", payload);
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
