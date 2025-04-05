import { WebSocketServer } from "ws";
import { sendToArduino, serialEmitter } from "./serial.ts";

export function startWebSocketServer(port = 8080) {
  const wss = new WebSocketServer({ port });

  serialEmitter.on("data", (line) => {
    wss.clients.forEach((client) => {
      if (client.readyState === client.OPEN) {
        client.send(line);
      }
    });
  });

    // Listen for incoming messages from clients
    wss.on('connection', (client) => {

      console.log(`Made connection on websocket.`)

      client.on('message', (message) => {
        console.log('Received message from client:', message);
        sendToArduino(message.toString());
      });
    });


  console.log(`WebSocket server running on ws://localhost:${port}`);
}
