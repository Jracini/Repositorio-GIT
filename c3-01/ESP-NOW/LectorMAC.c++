#include <Arduino.h>
#include <WiFi.h>
#include<esp_wifi.h>
#include <Wire.h>

void readMacAddres(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA,baseMac);
  if (ret == ESP_OK)
  {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  } else
  {
    Serial.printf("La lectura de direccion MAC fallo");
  }
  
  
}
void setup(){
Serial.begin(115200);

WiFi.mode(WIFI_STA);
Serial.print("\n[DEFAULT] Direccion MAC ESP32: ");
readMacAddres();
}

void loop(){

}
