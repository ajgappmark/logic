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
#define APP_ADDR       0x400

/*- Implementations ---------------------------------------------------------*/

//-----------------------------------------------------------------------------
	.section .vectors, "ax", @progbits
	.global	__vectors
__vectors:
	rjmp	__init
	rjmp	APP_ADDR + 0x02		// INT0
	rjmp	APP_ADDR + 0x04		// PCINT0
	rjmp	APP_ADDR + 0x06		// TIMER1_COMPA
	rjmp	APP_ADDR + 0x08		// TIMER1_OVF
	rjmp	APP_ADDR + 0x0a		// TIMER0_OVF
	rjmp	APP_ADDR + 0x0c		// EE_RDY
	rjmp	APP_ADDR + 0x0e		// ANA_COMP
	rjmp	APP_ADDR + 0x10		// ADC
	rjmp	APP_ADDR + 0x12		// TIMER1_COMPB
	rjmp	APP_ADDR + 0x14		// TIMER0_COMPA
	rjmp	APP_ADDR + 0x16		// TIMER0_COMPB
	rjmp	APP_ADDR + 0x18		// WDT
	rjmp	APP_ADDR + 0x1a		// USI_START
	rjmp	APP_ADDR + 0x1c		// USI_OVF

//-----------------------------------------------------------------------------
	.section .init0, "ax", @progbits
__init:
	clr	r1
	rjmp	main

//-----------------------------------------------------------------------------
	.text
	.type	run_application, @function
	.global	run_application
run_application:
	out	_SFR_IO_ADDR(DDRB), r0
	out	_SFR_IO_ADDR(PORTB), r0
	rjmp	app_addr
	.set	app_addr, APP_ADDR
	.size	run_application, .-run_application


