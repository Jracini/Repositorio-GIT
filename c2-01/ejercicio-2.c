#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "driver/uart.h"
#include "esp_log.h"

#define UART_NUM UART_NUM_0
#define BUF_SIZE 1024

// Verifica si la cadena es un número entero válido
bool es_entero(const char *str) {
    if (*str == '-' || *str == '+') str++;  // acepta signo
    if (*str == '\0') return false;         // vacío no es válido
    while (*str) {
        if (!isdigit((unsigned char)*str)) return false;
        str++;
    }
    return true;
}

// Calcula el cuadrado sumando los primeros N impares
int cuadrado_por_impares(int n) {
    int suma = 0, impar = 1;
    for (int i = 0; i < n; i++) {
        suma += impar;
        impar += 2;
    }
    return suma;
}

void app_main(void) {
    // Configurar UART
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Esperar y procesar datos por UART
    uint8_t data[BUF_SIZE];

    while (1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, 100 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0';  // convierte a string
            char *input = (char *)data;

            // eliminar salto de línea (enter)
            input[strcspn(input, "\r\n")] = '\0';

            // Si es un número entero:
            if (es_entero(input)) {
                int num = atoi(input);
                if (num > 0) {
                    int resultado = cuadrado_por_impares(num);
                    char mensaje[64];
                    snprintf(mensaje, sizeof(mensaje),
                             "Cuadrado de %d es %d\r\n", num, resultado);
                    uart_write_bytes(UART_NUM, mensaje, strlen(mensaje));
                }
            }

            // Si no es un número entero → se ignora automáticamente
        }
    }
}