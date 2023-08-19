#include <Arduino.h>

#include <ESPAsyncWiFiManager.h>
#include <AsyncElegantOTA.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ACS712.h>

// Set web server port number to 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
DNSServer dns;


// Set number of outputs
#define NUM_OUTPUTS  5

// Assign each GPIO to an output
// GPIO 2 is forbidden
int outputGPIOs[NUM_OUTPUTS] = {5, 4, 14, 12, 13};

ACS712 sensor(ACS712_20A, A0);
float current=0;

void initACS () {
  sensor.calibrate();
}

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}

void initWifi() {
  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println ("connected");
  digitalWrite(LED_BUILTIN, LOW);
}



String getOutputStates(){
  StaticJsonDocument<384> myArray;
  for (int i =0; i<NUM_OUTPUTS; i++){
    myArray["gpios"][i]["output"] = String(outputGPIOs[i]);
    myArray["gpios"][i]["state"] = String(digitalRead(outputGPIOs[i]));
  }
  myArray["current"] = current;
  String jsonString;
  serializeJson(myArray,jsonString);
  return jsonString;
}

void notifyClients(String state) {
  ws.textAll(state);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "states") == 0) {
      notifyClients(getOutputStates());
    }
    else{
      int gpio = atoi((char*)data);
      digitalWrite(gpio, !digitalRead(gpio));
      notifyClients(getOutputStates());
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
 
  Serial.begin(115200);

  // Set GPIOs as outputs
  for (int i =0; i<NUM_OUTPUTS; i++){
    pinMode(outputGPIOs[i], OUTPUT);
    digitalWrite(outputGPIOs[i], HIGH);
  }

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  initACS();
  initLittleFS();
  initWifi();
  initWebSocket();


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html",false);
  });

  server.serveStatic("/", LittleFS, "/");

  AsyncElegantOTA.begin(&server);

  server.begin();
}

void loop() {
    Serial.println("reading current..");
    current = sensor.getCurrentAC(50);
    Serial.printf("got current: %f\n", current);
    notifyClients(getOutputStates());
    ws.cleanupClients();
    delay(1000);
}