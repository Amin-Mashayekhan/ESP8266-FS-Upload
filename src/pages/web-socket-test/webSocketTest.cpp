#include "webSocketTest.h"
#include "AsyncWebSocket.h"
#include <ESPAsyncWebServer.h>
#include "../../modules/web-socket/WebSocketModule.h"
#include "../../modules/web-socket/webSocketShared.h"
#include <LittleFS.h>

// Create WebSocket object
WebSocketModule testWebSocket("/ws");

// Function to handle WebSocket events
void onWebSocketEventWST(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("Client connected to web socket test page: %u\n", client->id());
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("Client disconnected from web socket test page: %u\n", client->id());
  }
  // else if (type == WS_EVT_DATA)
  // {
  //   // Handle received message from client
  //   String message = "";
  //   for (size_t i = 0; i < len; i++)
  //   {
  //     message += (char)data[i];
  //   }
  //   Serial.printf("Message from client %u: %s\n", client->id(), message.c_str());

  //   // Example: echo message back to client
  //   client->text("Echo: " + message);
  // }
}

// Function to send real-time data over WebSocket
void setupWebSocketTestPage(AsyncWebServer *server)
{
  testWebSocket.onEvent(onWebSocketEventWST); // Register the event handler
  testWebSocket.addToServer(server);
  // Serve the HTML file for the WebSocket test page
  server->on("/web-socket-test/", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(LittleFS, "/web-socket-test.html", "text/html"); });
}


unsigned long lastSendTimeWST = 0;
unsigned long sendInterval = 1000;
unsigned long successfulSends = 0;
unsigned int failedSends = 0;
void notifyClients()
{
  if (currentMillis - lastSendTimeWST >= sendInterval)
  {
    // Serial.println(" * f-c: " + String(fastCounter) + " \ c-m: " + String(currentMillis) + " * ");
    // Serial.println(currentMillis);
    lastSendTimeWST = currentMillis;

    // Create a JSON string to send over WebSocket
    String jsonDataWST = "{\"sendInterval\": \"" + String(sendInterval) + "\",";
    jsonDataWST += "\"freeHeap\": \"" + String(ESP.getFreeHeap()) + "\",";
    jsonDataWST += "\"successfulSends\": \"" + String(successfulSends) + "\",";
    jsonDataWST += "\"failedSends\": \"" + String(failedSends) + "\",";
    jsonDataWST += "\"preparationMoment\": \"" + String(currentMillis) + "\",";
    jsonDataWST += "\"serverStartSecond\": \"" + String(serverStartSecond) + "\", \"fastCounter\": \"" + String(fastCounter) + "\"}";
    bool sent = testWebSocket.broadcastMessage(jsonDataWST); // Send data to all connected
    if (testWebSocket.clientCount() > 0)              // Check if clients are connected
    {
      if (sent)
      {
        successfulSends++;
        if (sendInterval > 100)
        {
          sendInterval -= 50;
        }
        else if (sendInterval > 34)
        {
          sendInterval -= 1;
        }
      }
      else
      {
        failedSends++;
        sendInterval += 100;
      }
    }
  }
}