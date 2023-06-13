#define BLYNK_TEMPLATE_ID "TMPL6h9YGAJ4W"
#define BLYNK_TEMPLATE_NAME "deteksi serangga"
#define BLYNK_AUTH_TOKEN "b2-0K57-cj65Zx01wuEe_nVFNX2iXaY9"
#define BLYNK_PRINT Serial

#include <ArduinoJson.h>
#include <CTBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>

// Telegram
CTBot mybot;
char token[] = "6115324412:AAFPpNtYi3xQkMnm5JS8f32umDeBJEy62U8";
const int id  = 1868541002;

char auth[] = BLYNK_AUTH_TOKEN; //Auth Token
char ssid[] = "Galaxy A32"; //nama hotspot yang digunakan
char pass[] = ""; //password hotspot yang digunakan

BlynkTimer timer;

#define Echo D8
#define Trig D7
#define merah D0
#define PIR_PIN D4

int buzzerPin = D5;
float distance = 0;
int switchBuzz;
int nilaiPIR;

BLYNK_WRITE(V2){
  int pinData = param.asInt();
  Serial.println(pinData);
  if (pinData == 1){
    switchBuzz = 1;
  }else{
    switchBuzz = 0;
  }

}

void data_blynk() {
  long durasi, jarak;
  digitalWrite(Trig, LOW);
  delay(10);

  digitalWrite(Trig, HIGH);
  delay(10);

  digitalWrite(Trig, LOW);
  durasi = pulseIn(Echo, HIGH);
  jarak = (durasi/2) / 29.1;

  nilaiPIR = digitalRead(PIR_PIN);

  Blynk.virtualWrite(V0, jarak);
  
  if (switchBuzz == 1){
    if (jarak < 200) {
      digitalWrite(merah, HIGH);
      Blynk.virtualWrite(V1, 1);
      tone(buzzerPin, 20, 1000);
      Serial.print("Ada Yang Masuk!!!");
      mybot.sendMessage(id,"Ada Yang Masuk!!!");
      Blynk.virtualWrite(V3,"Ada Yang Masuk!!!");
      delay(500);
    } else {
      digitalWrite(merah, LOW);
      Blynk.virtualWrite(V1, 0);
      noTone(buzzerPin);
      Serial.print("Tidak Ada Yang Masuk");
      mybot.sendMessage(id,"Tidak Ada Yang Masuk");
      Blynk.virtualWrite(V3,"Tidak Ada Yang Masuk");
      delay(500);
    }
  }else {
    Blynk.virtualWrite(V3," ");
    if (jarak < 200) {
      digitalWrite(merah, HIGH);
    } else {
      digitalWrite(merah, LOW);
    }
  }

  Serial.print("Jarak Object : ");
  Serial.print(jarak);
  Serial.println(" cm");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Copyright © Auvarmf");
  Blynk.begin(auth, ssid, pass);
  pinMode(merah, OUTPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  mybot.wifiConnect(ssid,pass);
  mybot.setTelegramToken(token);
  timer.setInterval(2500L, data_blynk);
}

void loop() {
  delay(10);
  data_blynk();
  Blynk.run();
  timer.run();
}
