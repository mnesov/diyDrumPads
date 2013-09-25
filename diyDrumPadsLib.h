#ifndef DRUMMERPADSLIB_H_
#define DRUMMERPADSLIB_H_

struct AnalogPadSettings {
	byte note;
	byte pin;
	byte threshold;
	uint16_t cutoff;
};

struct AnalogPad {
	AnalogPadSettings settings;
	uint16_t duration = 0;
	uint16_t maxValue = 0;
	uint16_t count = 0;
};

extern AnalogPad analogPads[];
extern byte midiChannel;
extern const byte analogPadsCount;

// Sets ADC prescale factor, so ADC frequency will be CLK / 2^factor, factor value should be in range [1-7]
void setAnalogReadPrescale(byte factor);
void note(int pitch, int velocity);
void processAnalogPad(AnalogPad &pad);
void processAnalogPads(void);
void setupAnalogPads(void);


#endif /* DRUMMERPADSLIB_H_ */
