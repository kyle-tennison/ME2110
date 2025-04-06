import { SerialPort } from "serialport";
import { EventEmitter } from "events";
import { ReadlineParser } from "@serialport/parser-readline";
import dotenv from "dotenv";
import { exit } from "process";

// Load environment variables from the .env file
dotenv.config();

interface Payload {
  val: number;
  tmp: number;
}

export const serialEmitter = new EventEmitter();

// Read the DEVICE_PATH from the environment variables
const devicePath = process.env.DEVICE_PATH || "/dev/cu.usbmodem1101";  // Fallback to a default path if not set

let port: SerialPort;

try {
  port = new SerialPort({
    path: devicePath,
    baudRate: 115200,
    autoOpen: true,
  });
}
catch (error) {
  if (error instanceof Error) {
    console.error('Failed to open port:', error.message);
  } else {
    console.error('An unknown error occurred while opening the port');
  }
  exit(1);
}

port.on("open", () => {
  console.log("Serial port opened");

  setTimeout(()=>{
    console.log("Sending test message to Arduino...")
    sendToArduino("abcd");
  }, 10000)
});

const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));
parser.on("data", (data: string) => {
    console.log('Incoming from Arduino:', data);

    if (data.startsWith("!")){
      return;
    }

    serialEmitter.emit("data", data);
});

export function sendToArduino(data: string) {
  if (port.isOpen) {
    port.write(data, (err) => {
      if (err) {
        console.error("Error writing to serial port:", err);
      }
      else {
        console.debug("Wrote to arduino.");
      }
    });
  } else {
    console.log("Serial port not open.");
  }
}
