#include <Arduino.h>
#include "MIDI.h"
#include "diyDrumPadsLib.h"


int main(void) {
	init();
	setAnalogReadPrescale(6);

//  Serial.begin(9600);
	MIDI.begin(midiChannel);

	setupAnalogPads();

	while(true) { loop(); }
}

void loop() {

	processAnalogPads();

}
