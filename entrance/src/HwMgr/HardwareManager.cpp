
#include "HardwareManager.h"

#include <iostream>
#include <string.h>
//#include "../../../common/wiringPi/wiringPi.h"

// pin where the button is wired to
#define BELL_BUTTON_PIN 0 // i.e. physical pin 11

HardwareManager::HardwareManager(const ringBellFun_t call_back) : ringBellFun(call_back) {}

bool HardwareManager::initHardware() const {
	// initialize wiringPi library

	if(wiringPiSetup() < 0){
		std::cerr << "\"Unable to setup wiringPi: " << strerror(errno) << std::endl;
		return false;
	}

	// ----- BELL BUTTON -----
	// specify pin as input
	pinMode(BELL_BUTTON_PIN, INPUT);
	// connect the pull up resistor
	pullUpDnControl(BELL_BUTTON_PIN, PUD_UP);
	// and attach myInterrupt() to the interrupt
	if(wiringPiISR(BELL_BUTTON_PIN, INT_EDGE_FALLING, HardwareManager::call_back) < 0){
		std::cerr << "Unable to setup ISR: " << strerror(errno) << std::endl;
		return false;
	}
	return true;
}

void HardwareManager::call_back() {
	if(!HardwareManager::already_handled.test_and_set()){
		delayMicroseconds(50 * 1000);
		const int value = digitalRead(BELL_BUTTON_PIN);
		if(value == HIGH){
			this->ringBellFun();
		}
		HardwareManager::already_handled.clear();
	}
}

