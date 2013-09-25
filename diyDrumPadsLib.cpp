#include <Arduino.h>
#include <avr/eeprom.h>
#include <MIDI.h>
#include <Event.h>
#include <Timer.h>
#include "diyDrumPadsLib.h"

AnalogPadSettings EEMEM analogPadsSettings[] = {
		{ 40, A0, 50, 100 },
		{ 41, A1, 50, 100 },
		{ 42, A2, 50, 100 },
		{ 43, A3, 50, 100 },
//		{ 0x2A, A4, 0x01, 0x32 },
//		{ 0x2B, A5, 0x01, 0x32 },
//		{ 0x2C, A6, 0x01, 0x32 },
//		{ 0x2D, A7, 0x01, 0x32 },
};

byte midiChannel = 10;
const byte analogPadsCount = sizeof(analogPadsSettings) / sizeof(AnalogPadSettings);
AnalogPad analogPads[analogPadsCount];

void setupAnalogPads(void) {
  for (byte i = 0; i < analogPadsCount; i++) {
  	eeprom_read_block(&analogPads[i].settings, &analogPadsSettings[i], sizeof(AnalogPadSettings));
  	analogPads[i].duration = 0;
  	analogPads[i].maxValue = 0;
  	analogPads[i].count = 0;
  }
}

void processAnalogPads(void) {
	for (byte i = 0; i < analogPadsCount; i++) {
		processAnalogPad(analogPads[i]);
	}
}

void processAnalogPad(AnalogPad &pad) {
	uint16_t value;

	value = (uint16_t)analogRead(pad.settings.pin);

	if (value > pad.settings.threshold && pad.duration == 0 && pad.count == 0) {
		pad.duration = 0;
		pad.maxValue = value;
		pad.count = pad.settings.cutoff;
	}

	if (pad.count > 0) {
		if (value > pad.maxValue) {
			pad.maxValue = value;
			pad.count = pad.settings.cutoff;
		}
		pad.duration++;
		pad.count--;
		if (pad.count == 0) {
			MIDI.sendNoteOn(pad.settings.note, map(pad.maxValue, 0, 1023, 0, 127), midiChannel);
		}
	} else {
		if (pad.duration > 0) {
			pad.duration--;
			if (pad.duration == 0) {
				MIDI.sendNoteOff(pad.settings.note, 0, midiChannel);
			}
		}
	}

}

// Sets ADC prescale factor, so ADC frequency will be CLK / 2^factor, factor value should be in range [1-7]
void setAnalogReadPrescale(byte factor) {
	ADCSRA = (ADCSRA & 0xF8) | factor;
}
