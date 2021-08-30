#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "data.h" //only for privacy reasons

#define STEPPER_PIN_1 14
#define STEPPER_PIN_2 12
#define STEPPER_PIN_3 13
#define STEPPER_PIN_4 15
int step_number = 0;

const char* ssid = ssid_private;
const char* password = password_private;

AsyncWebServer server(80);

int positionState = 0;
int currentPosition = 0;
int targetPosition = 0;

void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi ..");
    Serial.println(WiFi.macAddress());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

void setup(){
    pinMode(STEPPER_PIN_1, OUTPUT);
    pinMode(STEPPER_PIN_2, OUTPUT);
    pinMode(STEPPER_PIN_3, OUTPUT);
    pinMode(STEPPER_PIN_4, OUTPUT);

    Serial.begin(57600);
    initWiFi();

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"positionState\": "+String(positionState)+",\"currentPosition\": "+String(currentPosition)+",\"targetPosition\": "+String(targetPosition)+"}");
    });

    server.on("/targetPosition", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncWebParameter* p = request->getParam("value");
        toTargetPosition(p->value().toInt());
        request->send(200);
    });
    server.on("/currentPosition", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncWebParameter* p = request->getParam("value");
        // currentPosition(p);
        request->send(200);
    });
    server.on("/positionState", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncWebParameter* p = request->getParam("value");
        // positionState(p);
        request->send(200);
    });
    server.on("/obstructionDetected", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncWebParameter* p = request->getParam("value");
        request->send(200);
    });

    server.begin();
}

void toTargetPosition(int p){
    targetPosition = p;
    if(p<currentPosition){
        for(p*250; p > 0; p--){
            OneStep(true);
            delay(2);
        }
    }else{
        for(p*250; p > 0; p--){
            OneStep(false);
            delay(3);
        }
    }
}

void OneStep(bool dir){
    if(dir){
        switch(step_number){
            case 0:
                digitalWrite(STEPPER_PIN_1, HIGH);
                digitalWrite(STEPPER_PIN_2, LOW);
                digitalWrite(STEPPER_PIN_3, LOW);
                digitalWrite(STEPPER_PIN_4, LOW);
                break;
            case 1:
                digitalWrite(STEPPER_PIN_1, LOW);
                digitalWrite(STEPPER_PIN_2, HIGH);
                digitalWrite(STEPPER_PIN_3, LOW);
                digitalWrite(STEPPER_PIN_4, LOW);
                break;
            case 2:
                digitalWrite(STEPPER_PIN_1, LOW);
                digitalWrite(STEPPER_PIN_2, LOW);
                digitalWrite(STEPPER_PIN_3, HIGH);
                digitalWrite(STEPPER_PIN_4, LOW);
                break;
            case 3:
                digitalWrite(STEPPER_PIN_1, LOW);
                digitalWrite(STEPPER_PIN_2, LOW);
                digitalWrite(STEPPER_PIN_3, LOW);
                digitalWrite(STEPPER_PIN_4, HIGH);
                break;
        } 
    }else{
        switch(step_number){
            case 0:
                digitalWrite(STEPPER_PIN_1, LOW);
                digitalWrite(STEPPER_PIN_2, LOW);
                digitalWrite(STEPPER_PIN_3, LOW);
                digitalWrite(STEPPER_PIN_4, HIGH);
                break;
            case 1:
                digitalWrite(STEPPER_PIN_1, LOW);
                digitalWrite(STEPPER_PIN_2, LOW);
                digitalWrite(STEPPER_PIN_3, HIGH);
                digitalWrite(STEPPER_PIN_4, LOW);
                break;
            case 2:
                digitalWrite(STEPPER_PIN_1, LOW);
                digitalWrite(STEPPER_PIN_2, HIGH);
                digitalWrite(STEPPER_PIN_3, LOW);
                digitalWrite(STEPPER_PIN_4, LOW);
                break;
            case 3:
                digitalWrite(STEPPER_PIN_1, HIGH);
                digitalWrite(STEPPER_PIN_2, LOW);
                digitalWrite(STEPPER_PIN_3, LOW);
                digitalWrite(STEPPER_PIN_4, LOW);
                break;
        }
    }
    step_number++;
    if(step_number > 3){
        step_number = 0;
    }
}

void loop(){}