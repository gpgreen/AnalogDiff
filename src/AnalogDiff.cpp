//============================================================================
// Name        : AnalogDiff.cpp
// Author      : Greg Green <gpgreen@gmail.com>
// Version     : 1.0
// Copyright   : GPL v3
// Description : Read differential channel on ADC hardware
//=======================================================================
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
