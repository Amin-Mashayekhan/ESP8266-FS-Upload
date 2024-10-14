#include <ESP8266WiFi.h>
#include <LittleFS.h>  // Include the LittleFS library
#include <ESPAsyncWebServer.h>
 
const char* ssid = "Maad";
const char* password =  "Mm-12345";
 
AsyncWebServer server(80);
 
void setup(){
  Serial.begin(9600);
 
  if(!LittleFS.begin()){
     Serial.println("An Error has occurred while mounting LittleFS");
     return;
  }
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
 
   if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  // Serve a simple HTML page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(200, "text/html", "<h1>Hello, World!</h1>");
  // });
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });
 
  server.on("/bootstrap.rtl.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/bootstrap.rtl.min.css", "text/css");
  });
 
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });
 
  server.on("/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/bootstrap.bundle.min.js", "text/javascript");
  });
 
  server.begin();
}
 
void loop(){}