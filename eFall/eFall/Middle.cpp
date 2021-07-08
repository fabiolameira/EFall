#include "Middle.h"

Middle::Middle() {
	this->index = 0;
	this->state = IDDLE;
}

char Middle::getState() {
	return this->state;
}

int Middle::begin() {
	return IMU.begin();
}

int Middle::end() {
	IMU.end();
}

void Middle::readValues() {
	float x, y, z;
	if (IMU.accelerationAvailable()) {
		IMU.readAcceleration(x, y, z);
		this->x = x;
		this->y = y;
		this->z = z;
	}
}

void Middle::feedFifos(char flag2Seconds) {
	if (index == FIFO_SIZE) index = 0;
	
	float lastA = fifoA[index];
	float a = sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2)));
	fifoA[index] = a;
	
	float lastM = fifoM[index - 1];
	float newM = lastM - (lastA / FIFO_SIZE) + (fifoA[index] / FIFO_SIZE);
	fifoM[index] = newM;
	
	this->handleState(newM, flag2Seconds);
	// Serial.println((String) newM + ";");
	
	index++;
}

void Middle::handleState(float m, char flag2Seconds) {
	if (state == IDDLE && m < 0.5) {
		state = PRE_FALL;
		Serial.println("STATE: PRE_FALL");
	}
	if (state == PRE_FALL) {
		if (m > 1.8) {
			state = FALL;
			Serial.println("STATE: FALL");
		} 
		else if (flag2Seconds == 1) {
			state = IDDLE;
			Serial.println("STATE: IDDLE");
		}
	}
	if (state == FALL) {
		state = IDDLE;
		Serial.println("STATE: IDDLE");
	}
}

void Middle::printFifoA() {
	for (int i = 0; i < FIFO_SIZE; i++) {
		Serial.println(" A: " + (String) fifoA[i]);
	}
}

void Middle::printFifoM() {
	for (int i = 0; i < FIFO_SIZE; i++) {
		Serial.println(" M: " + (String) fifoM[i]);
	}
}