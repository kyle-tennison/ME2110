import { SerialPort } from "serialport";
import { EventEmitter } from "events";
import { ReadlineParser } from "@serialport/parser-readline";

export const serialEmitter = new EventEmitter();
// serialEmitter.emit("data", data.toString());


const port = new SerialPort({
  path: "/dev/cu.usbmodem1101",
  baudRate: 9600,
  autoOpen: true,
});

port.on("open", () => {
  console.log("Serial port opened");
});

const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));
parser.on("data", (data) => {
  console.log(`Got incoming data: ${data}`);
});
