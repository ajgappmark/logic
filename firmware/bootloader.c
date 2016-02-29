/*
 * Copyright (c) 2016, Alex Taradov <alex@taradov.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*- Includes ----------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <stdint.h>
#include <stdbool.h>
#include "leds.h"

/*- Definitions -------------------------------------------------------------*/
#define APP_ADDR       0x400

/*- Prototypes --------------------------------------------------------------*/
bool uart_sync(void);
uint8_t uart_rx(void);
uint8_t uart_wait_rx(void);
void run_application(void);

/*- Implementations ---------------------------------------------------------*/

//-----------------------------------------------------------------------------
static void error(void)
{
  led_color_set(LED_RED);
  while (1);
}

//-----------------------------------------------------------------------------
static uint8_t hex_to_dec(uint8_t b)
{
  if ('0' <= b && b <= '9')
    return b - '0';
  else if ('a' <= b && b <= 'f')
    return b - 'a' + 10;
  else if ('A' <= b && b <= 'F')
    return b - 'A' + 10;
  else
    error();
  return 0;
}

//-----------------------------------------------------------------------------
static uint8_t read_byte(void)
{
  uint8_t h, l;

  h = hex_to_dec(uart_rx());
  l = hex_to_dec(uart_rx());

  return (h << 4) | l;
}

//-----------------------------------------------------------------------------
static uint16_t update_crc_ccitt(uint16_t crc, uint8_t data)
{
  data ^= crc & 0xff;
  data ^= data << 4;
 
  return ((((uint16_t)data << 8) | ((crc & 0xff00) >> 8)) ^
            (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
}

//-----------------------------------------------------------------------------
int main(void)
{
  bool sync = false;
  bool lock = true;
  uint8_t sync_cnt = 0;
  uint16_t timeout;
  uint16_t crc = 0;
  uint16_t addr;

  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  led_init();
  led_color_set(LED_WHITE);
  led_white_set(1);

  for (timeout = 5000; timeout && !sync; timeout--)
    sync = uart_sync();

  for (; timeout && sync_cnt < 5; timeout--)
  {
    uint8_t byte = uart_wait_rx();

    if ('?' == byte)
      sync_cnt++;
    else if (0 != byte)
      sync_cnt = 0;
  }

  if (0 == timeout)
    run_application();

  led_color_set(LED_MAGENTA);

  while (1)
  {
    uint8_t cmd = uart_rx();

    if ('#' == cmd && !lock)
    {
      if (FLASHEND == addr)
        error();

      for (uint8_t i = 0; i < SPM_PAGESIZE/2; i++)
      {
        uint8_t a, b;

        a = read_byte();
        crc = update_crc_ccitt(crc, a);

        b = read_byte();
        crc = update_crc_ccitt(crc, b);

        boot_page_fill(addr + i*2, ((uint16_t)b << 8) | a);
      }

      if (';' != uart_rx())
        error();

      boot_page_erase(addr);
      boot_spm_busy_wait();

      boot_page_write(addr);
      boot_spm_busy_wait();

      addr += SPM_PAGESIZE;

      sync = false;
    }

    else if ('@' == cmd)
    {
      uint16_t addr;
      uint8_t value;

      addr = read_byte();

      if ('=' != uart_rx())
        error();

      value = read_byte();

      if (';' != uart_rx())
        error();

      eeprom_write_byte((uint8_t *)addr, value);

      sync = false;
    }

    else if ('$' == cmd)
    {
      crc = 0;
      lock = false;
      addr = APP_ADDR;

      if (';' != uart_rx())
        error();

      led_color_set(LED_BLUE);
    }

    else if ('%' == cmd)
    {
      uint8_t crch, crcl;

      crch = read_byte();
      crcl = read_byte();

      if (crc != (((uint16_t)crch << 8) | crcl))
        error();

      if (';' != uart_rx())
        error();

      lock = true;
      led_color_set(LED_GREEN);
    }

    while (!sync)
      sync = uart_sync();
  }

  return 0;
}

