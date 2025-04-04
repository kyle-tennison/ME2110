import { WebSocketServer } from "ws";
import { serialEmitter } from "./serial.ts";

export function startWebSocketServer(port = 8080) {
  const wss = new WebSocketServer({ port });

  serialEmitter.on("data", (line) => {
    wss.clients.forEach((client) => {
      if (client.readyState === client.OPEN) {
        client.send(line);
      }
    });
  });

  console.log(`WebSocket server running on ws://localhost:${port}`);
}
