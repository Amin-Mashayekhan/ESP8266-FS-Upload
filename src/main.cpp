#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h> // Include the LittleFS library

const char *ssid = "Maad";
const char *password = "Mm-12345";
int wifi_timer = 0;

AsyncWebServer server(80);

// Dynamic content
String title = "Welcome to My ESP8266 Web Server";
String dynamicContent = "This is dynamic text from ESP8266!";
unsigned long long serverStartSecond = 0;

// Create WebSocket object
AsyncWebSocket ws("/ws");

// Function to handle WebSocket events
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.println("WebSocket client connected");
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.println("WebSocket client disconnected");
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        String html = LittleFS.open("/index.html", "r").readString();
        // Replace placeholders
        html.replace("{{title}}", title);
        html.replace("{{dynamicContent}}", dynamicContent);
        request->send(200, "text/html", html); });

  // Serve CSS and JS files
  server.on("/assets/bootstrap.rtl.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/assets/bootstrap.rtl.min.css", "text/css"); });

  server.on("/assets/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/assets/style.css", "text/css"); });

  server.on("/assets/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/assets/bootstrap.bundle.min.js", "text/javascript"); });

  // Serve Font files
  server.on("/assets/fonts/vazir-font/Vazir-Medium-FD.woff2", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/assets/fonts/vazir-font/Vazir-Medium-FD.woff2", "font/woff2"); });

  server.on("/assets/fonts/vazir-font/Vazir-Medium-FD.woff", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/assets/fonts/vazir-font/Vazir-Medium-FD.woff", "font/woff"); });

  server.on("/assets/fonts/vazir-font/Vazir-Medium-FD.ttf", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/assets/fonts/vazir-font/Vazir-Medium-FD.ttf", "font/ttf"); });

  server.on("/assets/fonts/vazir-font/Vazir-Medium-FD.eot", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/assets/fonts/vazir-font/Vazir-Medium-FD.eot", "font/eot"); });

  // Initialize WebSocket
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.begin();
}

unsigned long long lastSendTime = 0;
unsigned long sendInterval = 1500; // Initial interval of 1.5 second
unsigned int successfulSends = 0;
unsigned int failedSends = 0;
unsigned long long fastCounter = 0;
unsigned long long currentMillis = 0;
// Function to send real-time data over WebSocket
void notifyClients()
{
  // Create a JSON string to send over WebSocket
  String json = "{\"sendInterval\": \"" + String(sendInterval) + "\",";
  json += "\"freeHeap\": \"" + String(ESP.getFreeHeap()) + "\",";
  json += "\"successfulSends\": \"" + String(successfulSends) + "\",";
  json += "\"failedSends\": \"" + String(failedSends) + "\",";
  json += "\"preparationMoment\": \"" + String(currentMillis) + "\",";
  json += "\"serverStartSecond\": \"" + String(serverStartSecond) + "\", \"fastCounter\": \"" + String(fastCounter) + "\"}";
  bool sent = ws.textAll(json); // Send data to all connected clients
  if (ws.count() > 0)
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
    // Send real-time updates to connected WebSocket clients
  }

  if (currentMillis - lastSendTime >= sendInterval)
  {
    // Serial.println(" * f-c: " + String(fastCounter) + " \ c-m: " + String(currentMillis) + " * ");
    Serial.println(currentMillis);
    lastSendTime = currentMillis;
    notifyClients();
  }
}