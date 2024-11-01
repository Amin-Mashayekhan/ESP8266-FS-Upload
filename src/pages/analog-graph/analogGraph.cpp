#include "analogGraph.h"
#include "../../modules/web-socket/WebSocketModule.h"
#include "../../modules/web-socket/webSocketShared.h"

// Create WebSocket object
WebSocketModule analogGraphWebSocket("/ws");

// Function to handle WebSocket events
void onWebSocketEventAG(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("Client connected to analog graph: %u\n", client->id(), client->remoteIP().toString().c_str());
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("Client disconnected from analog graph: %u\n", client->id(), client->remoteIP().toString().c_str());
  }
}

// Function to send real-time data over WebSocket
void setupAnalogGraphWebSocketPage(AsyncWebServer *server)
{
  analogGraphWebSocket.onEvent(onWebSocketEventAG); // Register the event handler
  analogGraphWebSocket.addToServer(server);

  // Serve the HTML page stored in LittleFS
  server->on("/analog/", HTTP_GET, [](AsyncWebServerRequest *request)
             {
    if (LittleFS.exists("/analog.html")) {
      request->send(LittleFS, "/analog.html", "text/html");
    } else {
      request->send(404, "text/plain", "File Not Found");
    } });
}

unsigned long lastSendTimeAGP = 0;
void notifyAnalogGraph()
{
  if (currentMillis - lastSendTimeAGP >= 56)
  {
    lastSendTimeAGP = currentMillis;
    // Serve the analog reading data as JSON
    int analogValue = analogRead(A0);
    String jsonDataAG = "{\"analog\":" + String(analogValue) + "}";
    analogGraphWebSocket.broadcastMessage(jsonDataAG); // Send data to all connected
  }
}