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
  
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = LittleFS.open("/index.html", "r").readString();

        // Dynamic content
        String title = "Welcome to My ESP8266 Web Server";
        String dynamicContent = "This is dynamic text from ESP8266!";

        // Replace placeholders
        html.replace("{{title}}", title);
        html.replace("{{dynamicContent}}", dynamicContent);

        request->send(200, "text/html", html);
    });

  // Serve CSS and JS files 
  server.on("/assets/bootstrap.rtl.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/assets/bootstrap.rtl.min.css", "text/css");
  });
 
  server.on("/assets/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/assets/style.css", "text/css");
  });
 
  server.on("/assets/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/assets/bootstrap.bundle.min.js", "text/javascript");
  });

  // Serve Font files 
    server.on("/assets/fonts/Vazir-Medium-FD.woff2", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/assets/fonts/Vazir-Medium-FD.woff2", "font/woff2");
    });

    server.on("/assets/fonts/Vazir-Medium-FD.woff", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/assets/fonts/Vazir-Medium-FD.woff", "font/woff");
    });

    server.on("/assets/fonts/Vazir-Medium-FD.ttf", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/assets/fonts/Vazir-Medium-FD.ttf", "font/ttf");
    });

    server.on("/assets/fonts/Vazir-Medium-FD.eot", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/assets/fonts/Vazir-Medium-FD.eot", "font/eot");
    });
 
  server.begin();
}
 
void loop(){}