#include <Arduino.h>
#include <WiFi.h>
#include "AudioSystem.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include <time.h>


#define ENC_DT    D7
#define ENC_CLK   D6
#define ENC_SW    D5

#define TFT_SCK   D0
#define TFT_MOSI  D1
#define TFT_DC    D2
#define TFT_RST   D3
#define TFT_CS    D4

#define PIN_I2S_LRC  D8
#define PIN_I2S_BCLK D9
#define PIN_I2S_DOUT D10

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;      // Polska to GMT+1 (3600 sekund)
const int   daylightOffset_sec = 3600;

volatile uint32_t holdStartMs = 0;
volatile bool buttonHolding = false;

AudioSystem audioSystem;
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas(160, 128);
bool refreshScreen = true;


int lastMinute = -1;

enum EncEvent { 
  EV_CW,
  EV_CCW,
  EV_PRESS,
  EV_LONG 
};

QueueHandle_t encQueue;



void taskRotary(void* p);

void handleEvent(uint8_t ev);

String toAscii(String text);

String getLocalTime();

void draw();

void setup() {
  Serial.begin(115200);
  delay(2000); 

  Serial.println("\n===== START =====");
  Serial.print("Total PSRAM: ");
  Serial.println(ESP.getPsramSize());
  Serial.print("Free PSRAM: ");
  Serial.println(ESP.getFreePsram());


  Serial.println("Initialize lcd");
  SPI.begin(TFT_SCK, -1, TFT_MOSI, TFT_CS);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);


  Serial.println("Create encoder queue...");
  encQueue = xQueueCreate(16, 1);
  xTaskCreatePinnedToCore(taskRotary, "rotary", 4096, nullptr, 1, nullptr, 1);

  Serial.println("Connecting to wifi");
  WiFi.disconnect();
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Initialize time from network");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  audioSystem.begin(PIN_I2S_BCLK, PIN_I2S_LRC, PIN_I2S_DOUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  audioSystem.loop();

  if (audioSystem.titleChanged()) {
    refreshScreen = true;
  }

  uint8_t ev;
  while(xQueueReceive(encQueue, &ev, 0) == pdTRUE) {
    handleEvent(ev);
  }

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    // Sprawdzamy, czy minuta się zmieniła
    if (timeinfo.tm_min != lastMinute) {
      lastMinute = timeinfo.tm_min;
      refreshScreen = true; // Wymuszamy odświeżenie ekranu!
    }
  }

  draw();
}



void taskRotary(void* p) {
  pinMode(ENC_CLK, INPUT_PULLUP); 
  pinMode(ENC_DT, INPUT_PULLUP); 
  pinMode(ENC_SW, INPUT_PULLUP); 

  int lastClk = digitalRead(ENC_CLK);
  bool lastBtn = HIGH;
  uint32_t pressAt = 0;

  for (;;) {
    int clk = digitalRead(ENC_CLK);
    if (clk != lastClk && clk == LOW) {
      uint8_t e = digitalRead(ENC_DT) ? EV_CW : EV_CCW;
      xQueueSend(encQueue, &e, 0);
    }

    lastClk = clk;

    bool btn = digitalRead(ENC_SW);
    if (btn == LOW && lastBtn == HIGH) {
      pressAt = millis();
      holdStartMs = pressAt;
      buttonHolding = true;
    }
    if (btn == HIGH && lastBtn == LOW) {
      buttonHolding = false;
      uint8_t e = (millis() - pressAt > 700) ? EV_LONG : EV_PRESS;
      xQueueSend(encQueue, &e, 0);
    }
    lastBtn = btn;
    vTaskDelay(pdMS_TO_TICKS(2));
  }
}

void handleEvent(uint8_t ev) {
  if(ev == EV_CW) {
    Serial.println("Encoder EV_CW");
    audioSystem.volumeUp();
    refreshScreen = true;
  }
  else if(ev == EV_CCW) {
    Serial.println("Encoder EV_CCW");
    audioSystem.volumeDown();
    refreshScreen = true;
  }
  else if(ev == EV_PRESS) {
    Serial.println("Encoder EV_PRESS");
  }
  else if(ev == EV_LONG) {
    Serial.println("Encoder EV_LONG");
  }
}

String toAscii(String text) {
  String from = "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ";
  String to   = "acelnoszzACELNOSZZ";
  for (int i = 0; i < from.length(); i++) {
    text.replace(from.substring(i, i+1), to.substring(i, i+1));
  }
  return text;
}

String getLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "00:00";
  }
  char timeStringBuff[6]; // HH:MM + terminator
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M", &timeinfo);
  return String(timeStringBuff);
}

void draw() {
  if(!refreshScreen) {
    return;
  }

  Serial.println("Refresh screen");

  // 6 to font size
  int streamTitleX = (160 - (int)audioSystem.getStreamTitle().length() * 6) / 2;
  
  canvas.fillScreen(0x0000);

  canvas.setTextColor(ST77XX_WHITE);

  canvas.setCursor(2, 2); // Padding 5 pikseli od lewej i góry
  canvas.print(getLocalTime());

  int volPercent = audioSystem.getVolumePercent();
  canvas.drawRect(54, 2, 102, 12, ST77XX_WHITE);
  canvas.fillRect(55, 3, volPercent, 10, ST77XX_GREEN);

  // todo dodac scrolla
  canvas.setCursor(streamTitleX, 48);
  canvas.print(toAscii(audioSystem.getStreamTitle()));

  // double buffering rysujemy na canvie i wrzucamy na lcd
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), 160, 128);
  refreshScreen = false;
}