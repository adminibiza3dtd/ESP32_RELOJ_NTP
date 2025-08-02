#include <WiFi.h>
#include <TFT_eSPI.h>
#include <time.h>

// Cambia por tus credenciales WiFi:
const char* ssid = "Nombre_de_tu_WiFi";
const char* password = "Password_WiFi";

// Configuración de zona horaria: UTC+2 (España en verano), puedes cambiarlo según tu país/zona
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 2 * 3600;
const int   daylightOffset_sec = 0; // Si tienes horario de invierno/verano, ajusta aquí

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Conectar a WiFi
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Conectando WiFi...", tft.width()/2, tft.height()/2, 2);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  tft.fillScreen(TFT_BLACK);
  tft.drawString("WiFi Conectado!", tft.width()/2, tft.height()/2, 2);
  delay(1000);
  tft.fillScreen(TFT_BLACK);

  // Configurar hora desde NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    tft.setTextDatum(MC_DATUM);
    tft.drawString("No hay hora NTP!", tft.width()/2, tft.height()/2, 2);
    delay(2000);
    return;
  }

  // Ajusta la posición aquí:
  int x = tft.width() - 4;      // Más a la derecha
  int y = (tft.height() / 3);    // Más arriba

  // Borra solo la zona donde va la hora (ajusta el área al tamaño de la fuente 7)
  tft.fillRect(x - 110, y - 32, 120, 60, TFT_BLACK);

  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  tft.setTextDatum(TR_DATUM); // Top Right para alinear desde arriba a la derecha
  tft.drawString(timeStr, x, y, 7); // Fuente 7, grande

  delay(1000);
}
