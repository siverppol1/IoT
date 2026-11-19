/*
 * Cold Chain – ESP8266 + DHT11 → Django REST API
 * Envoie température & humidité en HTTP POST toutes les 10 s.
 * Moniteur Série : 115200 bauds
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

// ── Configuration Wi-Fi ──────────────────────────────────────
const char* WIFI_SSID     = "Et@ehei";       
const char* WIFI_PASSWORD = "eheio2023"; 

// ── Configuration serveur ────────────────────────────────────
const char* SERVER_URL = "http://10.9.9.110:8000/post/";

// ── Configuration capteur DHT ────────────────────────────────
#define DHTPIN  D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ── Intervalle d'envoi (ms) ──────────────────────────────────
const unsigned long SEND_INTERVAL = 20000; // 20 secondes

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== Cold Chain – ESP8266 + DHT11 ===");

  // Initialiser le capteur DHT
  dht.begin();

  // Connexion Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connexion au Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connecté ! IP locale : ");
  Serial.println(WiFi.localIP());
  Serial.print("Serveur cible : ");
  Serial.println(SERVER_URL);
  Serial.println("------------------------------------");
}

void loop() {
  // Lire les données du capteur
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature(); // en °C

  // Vérifier la validité de la lecture
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("[ERREUR] Échec de lecture du DHT11 !");
    delay(SEND_INTERVAL);
    return;
  }

  // Construire le JSON
  String jsonPayload = "{\"temperature\": " + String(temperature, 1)
                     + ", \"humidity\": "    + String(humidity, 1) + "}";

  Serial.print("[ENVOI]  JSON : ");
  Serial.println(jsonPayload);

  // Envoyer en HTTP POST si Wi-Fi connecté
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(jsonPayload);

    Serial.print("[REPONSE] Code HTTP : ");
    Serial.println(httpCode);

    if (httpCode > 0) {
      String response = http.getString();
      Serial.print("[REPONSE] Body : ");
      Serial.println(response);
    } else {
      Serial.print("[ERREUR] Requête échouée : ");
      Serial.println(http.errorToString(httpCode));
    }

    http.end();
  } else {
    Serial.println("[ERREUR] Wi-Fi déconnecté !");
  }

  Serial.println("------------------------------------");
  delay(SEND_INTERVAL);
}

