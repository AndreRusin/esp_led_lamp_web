#include <Arduino.h>
#include "Types.h"
#include <FastLED.h>
#include "Constants.h"
#include <GyverDBFile.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <SettingsAsyncWS.h>
#include <WiFiConnector.h>
#include "TimerManager.h"
#ifdef ESP_USE_IR
  #include <IRremoteESP8266.h>
  #include <IRrecv.h>
  #include <IRutils.h>
#endif
#ifdef ESP_USE_BUTTON
  #include <GyverButton.h>
#endif

#ifdef ESP_USE_IR
IRrecv irrecv(BTN_IR);
decode_results results;
#endif

#ifdef ESP_USE_BUTTON
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN); // для физической (не сенсорной) кнопки нужно поменять LOW_PULL на HIGH_PULL. ну и кнопку нужно ставить без резистора между находящимися рядом пинами D2 и GND
#endif

GyverDBFile db(&LittleFS, "/data.db");
SettingsAsyncWS sett("Лампуля", &db);

DB_KEYS(
    kk,
    wifi_ssid,
    wifi_pass,
    current_eff
);

CRGB leds[NUM_LEDS];
sets::Logger logger(150);

bool TimerManager::TimerRunning = false;
bool TimerManager::TimerHasFired = false;
uint8_t TimerManager::TimerOption = 1U;
uint64_t TimerManager::TimeToFire = 0ULL;

EffectState modes[MODE_AMOUNT];
EffectState currentState;
void initDB(EffectState modes[]) {
  db.init(kk::wifi_ssid, "");
  db.init(kk::wifi_pass, "");
  char key[12];
  for (size_t id = 0; id < EFFECT_COUNT; ++id) {
        const auto& effect = effects[id];

        uint16_t avg_speed = (effect.min_speed + effect.max_speed) / 2;
        uint16_t avg_scale = (effect.min_scale + effect.max_scale) / 2;

        snprintf(key, sizeof(key), "eff_%u", id);
        if ( db.has(SH(key)) == false ) {
          EffectState state = {100, avg_speed, avg_scale};
          db[SH(key)] = state;
        }
        db[SH(key)].writeTo(modes[id]);
    }
    if (db.has(kk::current_eff) == false) {
      db[kk::current_eff] = 0;
    }
    snprintf(key, sizeof(key), "eff_%u", db[kk::current_eff].toInt());
    db[SH(key)].writeTo(currentState);
}
const char* getCurrentEffectKey() {
  static char key[12];
  snprintf(key, sizeof(key), "eff_%u", db[kk::current_eff].toInt());
  return key;
}

void build(sets::Builder& b) {
    static uint8_t tab;

    if (b.Tabs("Управление;Настройки;Логи", &tab)) {
        // при нажатии перезагружаемся и выходим
        b.reload();
        return;
    }

    if (tab == 0) {
      if (b.beginGroup("Таймер выключения")) {
          if (b.Slider("Минуты", 10, 180, 5, "")) {
            
          }
          if (b.Button("Включить")) {

          }

          b.endGroup();  // закрыть группу
      }
      if (b.beginGroup("Эффекты")) {
          // Выпадающий список эффектов
          if (b.Select(kk::current_eff, "Эффект", getEffectsNames())) {
            db[SH(getCurrentEffectKey())].writeTo(currentState);
            b.reload();
          }

          if (b.Slider("Яркость", 1, 255, 1, "", &currentState.bright)) {
            db[SH(getCurrentEffectKey())] = currentState;
            modes[db[kk::current_eff]] = currentState;
          }

          if (b.Slider(
              "Скорость",
              effects[db[kk::current_eff].toInt()].min_speed,
              effects[db[kk::current_eff].toInt()].max_speed,
              1, "", &currentState.speed)) {
                db[SH(getCurrentEffectKey())] = currentState;
                modes[db[kk::current_eff]] = currentState;
              }

          if (b.Slider(
              "Масштаб",
              effects[db[kk::current_eff].toInt()].min_scale,
              effects[db[kk::current_eff].toInt()].max_scale,
              1, "", &currentState.scale)) {
                db[SH(getCurrentEffectKey())] = currentState;
                modes[db[kk::current_eff]] = currentState;
              }
          b.endGroup();  // закрыть группу
      }
    } else if (tab == 1) {
      sets::Group g(b, "WiFi");
      b.Input(kk::wifi_ssid, "SSID");
      b.Pass(kk::wifi_pass, "Password", "***");

      if (b.Button("Подключиться")) {
          db.update();
          WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
      }
    } else if (tab == 2) {
      b.Log(logger);
    }
}

bool loadingFlag = true;
bool ONflag = false;
unsigned char matrixValue[8][16];
static const uint8_t maxDim = max(WIDTH, HEIGHT);

void setup() {
    Serial.begin(115200);
    Serial.println();

  #ifdef ESP32
    LittleFS.begin(true);
  #else
    LittleFS.begin();
  #endif

    db.begin();
    initDB(modes);

  #if defined(ESP_USE_IR)
    irrecv.enableIRIn();
  #endif
  #if defined(ESP_USE_BUTTON)
    touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
    touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
  #endif

    FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection(TypicalLEDStrip)*/;
    FastLED.setBrightness(BRIGHTNESS);
    if (CURRENT_LIMIT > 0)
    {
      FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    }
    FastLED.clear();
    FastLED.show();

    WiFiConnector.setName(AP_NAME);
    WiFiConnector.setPass(AP_PASS);
    // подключение и реакция на подключение или ошибку
    WiFiConnector.onConnect([]() {
        Serial.print("Connected! ");
        Serial.println(WiFi.localIP());
    });
    WiFiConnector.onError([]() {
        Serial.print("Error! start AP ");
        Serial.println(WiFi.softAPIP());
    });

    WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
    sett.begin();
    sett.onBuild(build);

    memset(matrixValue, 0, sizeof(matrixValue));
    randomSeed(micros());
    changePower();
    loadingFlag = true;
}

void loop() {
  effectsTick();
  WiFiConnector.tick();
  sett.tick();

  #ifdef ESP_USE_IR
    irTick();
  #endif

  #ifdef ESP_USE_BUTTON
    buttonTick();
  #endif

  TimerManager::HandleTimer(&ONflag, &changePower);
}
