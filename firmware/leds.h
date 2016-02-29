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
#include <stdint.h>

/*- Definitions -------------------------------------------------------------*/
#define PIN_BLUE       (1 << 0)
#define PIN_GREEN      (1 << 1)
#define PIN_RED        (1 << 2)
#define PIN_WHITE      (1 << 3)

#define LED_OFF        0
#define LED_RED        1
#define LED_GREEN      2
#define LED_BLUE       3
#define LED_YELLOW     4
#define LED_MAGENTA    5
#define LED_CYAN       6
#define LED_WHITE      7

/*- Implementations ---------------------------------------------------------*/

//-----------------------------------------------------------------------------
static inline void led_init(void)
{
  DDRB = PIN_RED | PIN_GREEN | PIN_BLUE | PIN_WHITE;
  PORTB = PIN_RED | PIN_GREEN | PIN_BLUE | PIN_WHITE;
}

//-----------------------------------------------------------------------------
static inline void led_red_set(uint8_t state)
{
  if (state)
    PORTB &= ~PIN_RED;
  else
    PORTB |= PIN_RED;
}

//-----------------------------------------------------------------------------
static inline void led_green_set(uint8_t state)
{
  if (state)
    PORTB &= ~PIN_GREEN;
  else
    PORTB |= PIN_GREEN;
}

//-----------------------------------------------------------------------------
static inline void led_blue_set(uint8_t state)
{
  if (state)
    PORTB &= ~PIN_BLUE;
  else
    PORTB |= PIN_BLUE;
}

//-----------------------------------------------------------------------------
static inline void led_white_set(uint8_t state)
{
  if (state)
    PORTB &= ~PIN_WHITE;
  else
    PORTB |= PIN_WHITE;
}

//-----------------------------------------------------------------------------
static inline void led_red_toggle(void)
{
  PINB = PIN_RED;
}

//-----------------------------------------------------------------------------
static inline void led_green_toggle(void)
{
  PINB = PIN_GREEN;
}

//-----------------------------------------------------------------------------
static inline void led_blue_toggle(void)
{
  PINB = PIN_BLUE;
}

//-----------------------------------------------------------------------------
static inline void led_white_toggle(void)
{
  PINB = PIN_WHITE;
}

//-----------------------------------------------------------------------------
static inline void led_color_set(uint8_t color)
{
  uint8_t p = PORTB & PIN_WHITE;

  if (LED_OFF == color)
    PORTB = p | PIN_RED | PIN_GREEN | PIN_BLUE;
  else if (LED_RED == color)
    PORTB = p | PIN_GREEN | PIN_BLUE;
  else if (LED_GREEN == color)
    PORTB = p | PIN_RED | PIN_BLUE;
  else if (LED_BLUE == color)
    PORTB = p | PIN_RED | PIN_GREEN;
  else if (LED_YELLOW == color)
    PORTB = p | PIN_BLUE;
  else if (LED_MAGENTA == color)
    PORTB = p | PIN_GREEN;
  else if (LED_CYAN == color)
    PORTB = p | PIN_RED;
  else if (LED_WHITE == color)
    PORTB = p;
}

