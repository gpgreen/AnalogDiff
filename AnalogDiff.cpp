/*
  AnalogDiff.cpp - Using ADC differential conversion hardware
  Greg Green <gpgreen@gmail.com>

  History:
	23/July/18 - V0.1 released working on ATMEGA644 only
	

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <stdlib.h>

#include <AnalogDiff.h>

void AnalogDiff::init() {

#if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__)
    // datasheet recommends turning ADC off then on for differential
    // channels
    ADCSRA &= ~(1<<ADEN);
    ADCSRA = (1<<ADEN);
    ADCSRB = 0;
    // select positive differential input on ADC3
    // negative differential input on ADC2
    // gain is 10x    
    ADMUX = 0b01001101;
#else
#error Unsupported CPU type
#endif
}

int AnalogDiff::read() {

    init();

#if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__)
    ADCSRA |= (1<<ADSC);

    // ADSC is cleared when the conversion finishes
    while (bit_is_set(ADCSRA, ADSC));

	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	byte low  = ADCL;
	byte high = ADCH;

    // combine the two bytes
    return (high << 8) | low;
    
#endif
}
