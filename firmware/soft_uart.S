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

/*- Definitions -------------------------------------------------------------*/
#define PROBE_PORT     _SFR_IO_ADDR(PINB)
#define PROBE_PIN      4
#define BAUDRATE       38400
#define HALF_BIT       (((F_CPU / BAUDRATE) / (4 * 2)) - 2)

#define TMP            r18
#define CNT            r19
#define BIT            r20
#define BYTE           r24
#define RETURN         r24

/*- Implementations ---------------------------------------------------------*/

	.text

//-----------------------------------------------------------------------------
	.type	delay_half_bit, @function
delay_half_bit:
	ldi	TMP, HALF_BIT
1:	dec	TMP
	nop
	brne	1b
	ret
	.size	delay_half_bit, .-delay_half_bit

//-----------------------------------------------------------------------------
	.type	sample_bit, @function
sample_bit:
	clc
	ror	BYTE
	sbic	PROBE_PORT, PROBE_PIN
	ori	BYTE, 0x80
	ret
	.size	sample_bit, .-sample_bit

//-----------------------------------------------------------------------------
	.global	uart_sync
	.type	uart_sync, @function
uart_sync:
	ldi	RETURN, 0
	ldi	CNT, 0xff
10:
	sbic	PROBE_PORT, PROBE_PIN
	rjmp	20f
	dec	CNT
	brne	10b
	ret
20:
	sbis	PROBE_PORT, PROBE_PIN
	rjmp	30f
	dec	CNT
	brne	20b
	ret
30:
	rcall	delay_half_bit
	sbic	PROBE_PORT, PROBE_PIN
	rjmp	40f
	rcall	delay_half_bit
	sbic	PROBE_PORT, PROBE_PIN
	rjmp	40f
	rcall	delay_half_bit
	sbic	PROBE_PORT, PROBE_PIN
	rjmp	40f
	rcall	delay_half_bit
	rcall	delay_half_bit
	sbis	PROBE_PORT, PROBE_PIN
	rjmp	40f

	//sbi	_SFR_IO_ADDR(PORTB), 3
	//cbi	_SFR_IO_ADDR(PORTB), 3

	ldi	RETURN, 1
40:
	ret
	.size	uart_sync, .-uart_sync

//-----------------------------------------------------------------------------
	.global	uart_rx
	.type	uart_rx, @function
uart_rx:
10:
	sbis	PROBE_PORT, PROBE_PIN
	rjmp	10b
20:
	sbic	PROBE_PORT, PROBE_PIN
	rjmp	20b

	//sbi	_SFR_IO_ADDR(PORTB), 3
	ldi	BIT, 8
	rcall	delay_half_bit
30:
	rcall	delay_half_bit
	rcall	delay_half_bit
	rcall	sample_bit
	dec	BIT
	brne	30b

	//cbi	_SFR_IO_ADDR(PORTB), 3
	ret
	.size	uart_rx, .-uart_rx

//-----------------------------------------------------------------------------
	.global	uart_wait_rx
	.type	uart_wait_rx, @function
uart_wait_rx:
	ldi	RETURN, 0
	ldi	CNT, 0xff
10:
	sbis	PROBE_PORT, PROBE_PIN
	rjmp	20f
	dec	CNT
	brne	10b
	ret
20:
	ldi	BIT, 8
	rcall	delay_half_bit
30:
	rcall	delay_half_bit
	rcall	delay_half_bit
	rcall	sample_bit
	dec	BIT
	brne	30b
40:
	sbis	PROBE_PORT, PROBE_PIN
	rjmp	40b

	ret
	.size	uart_wait_rx, .-uart_wait_rx

