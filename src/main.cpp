#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "pages/analog-graph/analogGraph.h"
#include "pages/web-socket-test/webSocketTest.h"
#include "modules/web-socket/webSocketShared.h"
#include "modules/staic-files/StaticFileHandler.h"

const char *ssid = "Maad";
const char *password = "Mm-12345";
int wifi_timer = 0;

AsyncWebServer server(80);

// Dynamic content
String title = "Welcome to My ESP8266 Web Server";
String dynamicContent = "This is dynamic text from ESP8266!";

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  Serial.println("LittleFS mounted successfully");

  // Handle the analog Graph WebSocket page
  setupAnalogGraphWebSocketPage(&server);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(999.9);
    wifi_timer++;
    Serial.println("Connecting to WiFi.. \\ " + String(wifi_timer));
    digitalWrite(LED_BUILTIN, LOW);
    delay(0.1);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  // WiFi successfully connected
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(WiFi.localIP());

  if (!LittleFS.begin())
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }

  // Serve a simple HTML page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(200, "text/html", "<h1>Hello, World!</h1>");
  // });

  setupStaticFileRoutes(&server);

  setupWebSocketTestPage(&server);

  server.begin();
}

unsigned long serverStartPreviousMillis = 0;

void loop()
{
  fastCounter++;
  currentMillis = millis();
  if (currentMillis - serverStartPreviousMillis >= 1000)
  {
    serverStartPreviousMillis = currentMillis;
    serverStartSecond++;
    // Serial.println("\n Server Start Seconds" + String(serverStartSecond));
  }
  notifyAnalogGraph();
  notifyClients();
}