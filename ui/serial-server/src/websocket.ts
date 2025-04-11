import { WebSocketServer } from "ws";
import { sendToArduino, serialEmitter } from "./serial.ts";

export function startWebSocketServer(port = 8080) {
  const wss = new WebSocketServer({ port });

  serialEmitter.on("data", (line) => {
    let count = 0;
    wss.clients.forEach((client) => {
      if (client.readyState === client.OPEN) {
        client.send(line);
        count += 1;
      }
    });
    // console.log(`Relayed message to ${count} clients`)
  });

    // Listen for incoming messages from clients
    wss.on('connection', (client) => {

      console.log(`Made connection on websocket.`)

      client.on('message', (message) => {
        const data = message.toString();
        console.log('Received message from client: ', data);
        sendToArduino(data);
      });
    });


  console.log(`WebSocket server running on ws://localhost:${port}`);
}
