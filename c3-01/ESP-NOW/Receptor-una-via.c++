#include <esp_now.h>
#include <WiFi.h>

#define LED 2 //Pin del led del Esp32


const int frecuencia = 5000;
const int canal = 0;
const int resolucion = 12;

// Define a data structure
typedef struct struct_message {
  char a[32];
  int raw;
  float Porcentaje;
  bool d;
} struct_message;
 
// Create a structured object
struct_message Datos;
 
 
// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&Datos, incomingData, sizeof(Datos));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(Datos.a);
  Serial.print("Integer Value: ");
  Serial.println(Datos.raw);
  Serial.print("Float Value: ");
  Serial.println(Datos.Porcentaje);
  Serial.print("Boolean Value: ");
  Serial.println(Datos.d);
  Serial.println();

  // Validar y aplicar PWM
  int duty = constrain(Datos.raw, 0, 4095);  // limitar a rango válido
  ledcWrite(canal, duty); // aplicar PWM al LED según el valor recibido 
}
 /*---------------------------------------------------------------------------------------------------------------------------*/
 void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // poner ESP32 en modo estación
  
  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  // Registrar función de recepción
  esp_now_register_recv_cb(OnDataRecv);

  // Configuración del canal PWM
  ledcSetup(canal, frecuencia, resolucion);
  ledcAttachPin(LED, canal); // conectar canal PWM al pin del LED
}

void loop() {
  // Nada aquí, el LED se controla cuando llegan datos
}