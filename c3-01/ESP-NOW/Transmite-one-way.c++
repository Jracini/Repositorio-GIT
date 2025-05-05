#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "driver/adc.h"

#define Potenciometro ADC1_CHANNEL_0
#define ADC_WIDTH ADC_WIDTH_BIT_12  // Resolución de 12 bits (0-4095)
#define ADC_ATTEN ADC_ATTEN_DB_11   // Rango 3.3V

const float ADC_VOLT_MAX = 3.3;

/*ESP Transmisor 
MAC:e4:65:b8:47:fc:e8*/

/*ESP RECEPTOR 
MAC:fc:b4:67:78:c2:d4*/

//Linea para configurar la direccion MAC a enviar en este caso 
uint8_t broadcastAddress []= {0xfc, 0xb4, 0x67, 0x78, 0xc2, 0xd4};

//Estructura de los datos 
typedef struct estructura_del_mensaje
{
    char a[32]; /*Son 31 caracteres, porque 1 es nulo*/
    int b; //Entero
    float c; //Decimal 
    bool d; // Booleano (true or false)

} estructura_del_mensaje;

estructura_del_mensaje Datos; /*Se usa esta variable (Datos) para almacenar todos los datos que se enviaran*/

esp_now_peer_info_t peerInfo; //Se guardara los datos del dispositivo receptor que se usara con esp now

/*Verifica si se conecto y envio el mensaje correctamente*/
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
    Serial.print("\r\nEstatus del envio del mensaje o paquete: \t");
    Serial.println( status==ESP_NOW_SEND_SUCCESS ? "Envio completado" : "Envio fallido");
}

void setup(){
    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(Potenciometro, ADC_ATTEN);
    //Convierte el Esp32 en una estacion WiFi
    WiFi.mode(WIFI_STA);
    //Iniciar ESP-NOW
    if (esp_now_init() != ESP_OK){
        Serial.println("Error al iniciar ESP NOW");
        return;
    }
     // Registrar el envio del callback 
     esp_now_register_send_cb(OnDataSent);
     // Registrar semilla
     memcpy(peerInfo.peer_addr, broadcastAddress, 6);
     peerInfo.channel = 0;  
     peerInfo.encrypt = false;
     // Agregar semilla 
     if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Error al agregar semilla");
        return;
    }
}
void loop(){
    int raw = adc1_get_raw(Potenciometro);
    float porcentaje = (raw / 4095.0) * 100.0;;
    strcpy(Datos.a, "Valor ADC");
    Datos.b = raw;
    Datos.c = porcentaje;
    Datos.d = true;

     // Send message via ESP-NOW
     esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Datos, sizeof(Datos));
     if (result == ESP_OK) {
        Serial.println("Sending confirmed");
    }
    else {
        Serial.println("Sending error");
    }
    delay(500);
}