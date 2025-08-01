#include <TM1637Display.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// === PINS TM1637 ===
#define CLK D1  // CLK du module
#define DIO D2  // DIO du module
#define BUZZER D5

TM1637Display display(CLK, DIO);

// === WiFi credentials ===
const char* ssid     = "youpilab_fibre";
const char* password = "i_l@v3_yl2021Fibre";

// === NTP Setup ===
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000); // 3600 pour UTC+1 (ex: Benin)

// === SETUP ===
void setup() {
  Serial.begin(115200);
  display.setBrightness(5); // Luminosité entre 0 et 7
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  // Connexion WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connexion WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté au WiFi");

  // Démarrage du client NTP
  timeClient.begin();
}

// === LOOP ===
void loop() {
  timeClient.update();

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  // Format HHMM pour l'afficheur
  int displayTime = hours * 100 + minutes;

  // Affichage avec deux-points allumés
  display.showNumberDecEx(displayTime, 0b01000000, true);

  // Bip pour chaque seconde
  tone(BUZZER, 1000); // 1000 Hz
  delay(100);         // Bip court
  noTone(BUZZER);     // Désactiver le buzzer

  delay(900); //Reste du temps pour la seconde
}
