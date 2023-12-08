#ifndef __GERAL__H__
#define __GERAL__H__
#include <stdio.h>
#include "includes.h"
#include "wifiSta.h"
#include "mqtt.h"

// -- Inicializando e limpando as portas do teclado --;
void devboardInit(void)
{
  // -- Resetando os pinos dos registradores --;
  // -- Saídas digitais --;
  gpio_reset_pin(IO_SH);                      // -- Resetando todos os pinos a serem usados --;
  gpio_reset_pin(IO_CLK);                     // -- Resetando todos os pinos a serem usados --;
  gpio_reset_pin(IO_WR);                      // -- Resetando todos os pinos a serem usados --;

  // -- Saídas digitais --;
  gpio_set_level(IO_SH, 0);                   // -- Zerando as portas --;
  gpio_set_level(IO_CLK, 0);                  // -- Zerando as portas --;
  gpio_set_level(IO_WR, 0);                   // -- Zerando as portas --;

  // -- Configurandos as portas --;
  // -- Saídas digitais --;
  esp_rom_gpio_pad_select_gpio(IO_SH);
  gpio_set_direction(IO_SH, GPIO_MODE_OUTPUT);
  esp_rom_gpio_pad_select_gpio(IO_CLK);
  gpio_set_direction(IO_CLK, GPIO_MODE_OUTPUT);
  esp_rom_gpio_pad_select_gpio(IO_WR);
  gpio_set_direction(IO_WR, GPIO_MODE_OUTPUT);
  gpio_set_direction(IO_DT_RD, GPIO_MODE_INPUT);

  int resetSaida;
  gpio_set_level(IO_WR, 0);
  for(resetSaida = 0; resetSaida < 8; resetSaida++){
      gpio_set_level(IO_CLK, 1);
      gpio_set_level(IO_CLK, 0);
  }
  gpio_set_level(IO_SH, 1);
  gpio_set_level(IO_SH, 0);
}

uint8_t io_le_escreve(uint8_t saidas)
{
    int j;
    uint8_t entradas = 0;
    gpio_set_level(IO_SH,1);
    for (j = 7; j >= 0; j--)
    {
        entradas <<= 1;
        entradas += gpio_get_level(IO_DT_RD);
        gpio_set_level(IO_WR, ( saidas >> j ) & 1);
        gpio_set_level(IO_CLK,1);
        gpio_set_level(IO_CLK,0);
    } 
    gpio_set_level(IO_SH,0);

    return (entradas);
}


#endif  //!__GERAL__H__