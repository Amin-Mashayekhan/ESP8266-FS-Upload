#ifndef WEBSOCKETMODULE_H
#define WEBSOCKETMODULE_H

#include <ESPAsyncWebServer.h>

class WebSocketModule {
public:
  WebSocketModule(const char* url);
  void onEvent(AwsEventHandler handler);
  bool broadcastMessage(const String& message);  // Return a boolean
  void addToServer(AsyncWebServer* server);
 size_t clientCount(); // Add this method to get the client count
 
private:
  AsyncWebSocket ws;
};

#endif