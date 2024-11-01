#include "WebSocketModule.h"

WebSocketModule::WebSocketModule(const char *url) : ws(url) {}

void WebSocketModule::onEvent(AwsEventHandler handler)
{
    ws.onEvent(handler);
}

bool WebSocketModule::broadcastMessage(const String &message)
{
    if (ws.count() > 0)
    {                        // Check if there are any connected clients
        ws.textAll(message); // Send the message to all connected clients
        return true;         // Message was sent
    }
    return false; // No clients connected, message not sent
}

void WebSocketModule::addToServer(AsyncWebServer *server)
{
    server->addHandler(&ws);
}

size_t WebSocketModule::clientCount()
{
    return ws.count(); // Return the number of connected clients
}