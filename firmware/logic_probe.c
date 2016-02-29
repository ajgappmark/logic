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
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include "leds.h"

/*- Definitions -------------------------------------------------------------*/

/*- Types -------------------------------------------------------------------*/
typedef enum
{
  PROBE_STATE_LOW,
  PROBE_STATE_MID,
  PROBE_STATE_HIGH,
} probe_state_t;

/*- Implementations ---------------------------------------------------------*/

//-----------------------------------------------------------------------------
static void sys_init(void)
{
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;
  MCUCR = (1 << PUD);
  PCMSK = (1 << PCINT4);
  sei();
}

//-----------------------------------------------------------------------------
static void adc_init(void)
{
  ADMUX = (0 << REFS0) | (2 << MUX0);
  ADCSRB = (0 << ADTS0);
  ADCSRA = (1 << ADEN) | (3 << ADPS0);
}

//-----------------------------------------------------------------------------
static uint16_t adc_read(void)
{
  uint16_t adc;

  while (ADCSRA & (1 << ADSC));
  adc = ADC;
  ADCSRA |= (1 << ADSC);

  return adc;
}

//-----------------------------------------------------------------------------
static bool probe_changed(void)
{
  bool res = (GIFR & (1 << PCIF)) ? true : false;
  GIFR = (1 << PCIF);
  return res;
}

//-----------------------------------------------------------------------------
int main(void)
{
  probe_state_t probe_state = PROBE_STATE_MID;
  bool changed = false;
  uint16_t cnt = 0;

  sys_init();
  adc_init();
  led_init();

  led_color_set(LED_YELLOW);
  led_white_set(0);

  while (1)
  {
    probe_state_t state;
    uint16_t adc = adc_read();

    if (adc < 340)
    {
      state = PROBE_STATE_LOW;
      led_color_set(LED_RED);
    }
    else if (adc > /*670*/600)
    {
      state = PROBE_STATE_HIGH;
      led_color_set(LED_GREEN);
    }
    else
    {
      state = PROBE_STATE_MID;
      led_color_set(LED_YELLOW);
    }

    if (probe_state != state)
    {
      probe_state = state;
      //changed = false;
      changed = true;
    }

#if 1
    if (changed)
    {
      changed = false;
      cnt = 6000;
      //led_white_set(1);
      //led_white_set(0);
      //led_blue_set(1);
    }
#endif

#if 1
    if (probe_changed())
    {
      cnt = 6000;
      //led_white_set(1);
      //led_white_set(0);
//      led_blue_set(1);
    }
#endif

#if 1
    if (cnt > 0)
    {
      led_white_set(1);
      //led_blue_set(1);
      cnt--;
    }
    else
    {
      led_white_set(0);
      //led_blue_set(0);
    }
#endif
  }

  return 0;
}

