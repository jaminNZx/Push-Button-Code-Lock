//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include "settings.h"

//testing ESP32
//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>

int codeLocked, codeTimer1, codeTimer2, buttonState, buttonStatePrev, codeMode, buttonCounter;
SimpleTimer timer;
WidgetTerminal terminal(vPIN_TERMINAL);

/*
   OUTPUT TO CONSOLE FUNCTION
*/
void printTask(String a) {
#if defined(OUTPUT_SERIAL)
  Serial.println(a);
#endif
#if defined(OUTPUT_TERMINAL)
  terminal.println(a);
  terminal.flush();
#endif
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
#ifdef SERVER
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS, SERVER);
#else
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS);
#endif
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.begin();
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  timer.setInterval(10, checkButton);
  codeLocked = 1;
  digitalWrite(vPIN_LOCKED, HIGH);
}

void unlock() {
  printTask("UNLOCKED");
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
}

BLYNK_WRITE(vPIN_UNLOCK) {
  if (param.asInt()) unlock();
}

BLYNK_WRITE(vPIN_LOCKED) {
  codeLocked = param.asInt();
  (codeLocked) ? printTask("SECURE") : printTask("INSECURE");
}

void checkButton() {
  buttonState = !digitalRead(BUTTON);
  if (codeLocked) {
    if (!codeMode) {
      if (buttonState == HIGH && buttonStatePrev == LOW) {
        codeTimer1 = timer.setTimeout(CODE_START, []() {
          printTask("ENTERING CODE MODE");
          codeMode = 1;
          codeTimer2 = timer.setTimeout(CODE_TIMEOUT, []() {
            printTask("EXITING CODE MODE");
            codeMode = 0;
            buttonCounter = 0;
          });
        });
      }
      if (buttonState == LOW && buttonStatePrev == HIGH) timer.disable(codeTimer1);
    } else {
      if (buttonCounter >= BUTTON_CODE) {
        unlock();
        codeMode = 0;
        buttonCounter = 0;
        timer.disable(codeTimer2);
      } else {
        if (buttonState == HIGH && buttonStatePrev == LOW) buttonCounter++;
      }
    }
  } else {
    if (buttonState == HIGH && buttonStatePrev == LOW) unlock();
  }
  buttonStatePrev = buttonState;
}

void loop() {
  Blynk.run();
  ArduinoOTA.handle();
  timer.run();
}
