#include <stdio.h>
#include <stdlib.h>
#include "driver/touch_pad.h"
#include "driver/adc.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define touch TOUCH_PAD_NUM0 // GIOP-4 o pin 26
#define potenciometro ADC_CHANNEL_0  //GPIO-36 o pin 3
#define ADC_WIDTH ADC_WIDTH_BIT_12  // Resolución de 12 bits (0-4095)
#define ADC_ATTEN ADC_ATTEN_DB_11   // Rango 3.3V

float ADC_VOLT_MAX = 3.3;
uint16_t touch_value;

void app_main(){
    // Configurar ADC
    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(potenciometro, ADC_ATTEN);
     
     char carriage_return = 0;
     while (1)
     {
        carriage_return=getchar();
        if (carriage_return == '\r' || carriage_return == '\n')
        {
            printf("Recibi un enter. \n");
            break;  
        }
    }        vTaskDelay(1000/portTICK_PERIOD_MS);

    //Iniciar pin tactil
    touch_pad_init();
    touch_pad_config(touch, 0);

    //Bucle principal
    while (1)
    {
        touch_pad_read(touch, &touch_value);

        if (touch_value < 500) {
            int raw = adc1_get_raw(potenciometro);
            float porcentaje = (raw / 4095.0) * 100.0;
            printf("ADC: %d - %.2f%%\n", raw, porcentaje);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
       
    }
}
