#include <Arduino.h>
#include "WiFi.h"
#include <Servo.h>

const char* ssid = "";
const char* password = "";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 1;
const int   daylightOffset_sec = 3600;

const int servoOpen = 80;
const int servoClose = 180;

#define BUTTON_PIN 14

Servo servo;

void manger_time(int nbr = 4) {
  Serial.println("C'est manger time !");
  for (int i = 0; i < nbr; i++) {
    servo.write(servoOpen);
    delay(1000);
    servo.write(servoClose);
    delay(1000);
  }
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  servo.attach(13);
  servo.write(servoClose);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer);
}

void loop() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  if (
    (timeinfo.tm_hour == 10 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0)
    || (timeinfo.tm_hour == 16 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0)
    || (timeinfo.tm_hour == 20 && timeinfo.tm_min == 30 && timeinfo.tm_sec == 0)
  ) {
    manger_time(2);
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    manger_time(4);
  }
  // Serial.println(buttonState);
  delay(1000);
}