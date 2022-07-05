#include <stdio.h>
#include "pico/stdlib.h"

/* Elijo un pin de Trigger y Echo */
const uint TRIGGER = 27;
const uint ECHO = 26;

float ultrasonic_get_distance_cm(void) {
      /* Escribo un 1 en el Trigger */
      gpio_put(TRIGGER, true);
      /* Espero 10 us con el pulso encendido */
      sleep_us(10);
      /* Escribo un 0 en el Trigger */
      gpio_put(TRIGGER, false);
      /* Espero a que el pulso llegue al Echo */
      while(!gpio_get(ECHO));
      /* Mido el tiempo de cuando llega el pulso */
      absolute_time_t from = get_absolute_time();
      /* Espero a que el pulso del Echo baje a cero */
      while(gpio_get(ECHO));
      /* Mido el tiempo cuando termina el pulso */
      absolute_time_t to = get_absolute_time();
      /* Saco la diferencia de tiempo (el ancho del pulso) en us */
      int64_t pulse_width = absolute_time_diff_us(from, to);
      /* Calculo la distancia */
      float distance = pulse_width / 59.0;
      return distance;
}

int main() {
    /* Habilito el USB */
    stdio_init_all();
    // Declaro los pines de los LEDS
    gpio_init (2);
    gpio_set_dir (2, true);
    gpio_put (2, false);
    
    gpio_init (3);
    gpio_set_dir (3,true);
    gpio_put (3,false);
    
    gpio_init (4);
    gpio_set_dir (4,true);
    gpio_put (4,false);
    
    sleep_ms(1000);
    printf("Listo!\r\n");
    /* Habilito el pin de Trigger */
    gpio_init(TRIGGER);  
    /* Configuro el Trigger como salida */
    gpio_set_dir(TRIGGER, true);
    /* Pongo un 0 en el pin de Trigger */
    gpio_put(TRIGGER, false);
    /* Habilito el pin de Echo */
    gpio_init(ECHO);
    /* Configuro el Echo como entrada */
    gpio_set_dir(ECHO, false);


    while(1){
      //Doy la orden de cuando se deben encender los LEDS
          if(ultrasonic_get_distance_cm() < 10){
            gpio_put (2, true);
            gpio_put (3, false);
            gpio_put (4, false);

          }
          else if(ultrasonic_get_distance_cm() < 50){
            gpio_put (3, true);
            gpio_put (2, false);
            gpio_put (4, false);
          }
          else if(ultrasonic_get_distance_cm() < 100){
            gpio_put (4, true);
            gpio_put (2, false);
            gpio_put (3,false);
          }
        /* Espero medio segundo */
        sleep_ms(500);
    }
}