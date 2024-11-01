#ifndef WEBSOCKETTEST_H
#define WEBSOCKETTEST_H

#include <ESPAsyncWebServer.h>

void setupWebSocketTestPage(AsyncWebServer *server);
void notifyClients();

#endif