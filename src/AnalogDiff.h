//============================================================================
// Name        : AnalogDiff.h
// Author      : Greg Green <gpgreen@gmail.com>
// Version     : 1.0
// Copyright   : GPL v3
// Description : Read differential channel on ADC hardware
//=======================================================================
#ifndef AnalogDiff_h
#define AnalogDiff_h

namespace AnalogDiff {
	void init();
	int read();
}

#endif
