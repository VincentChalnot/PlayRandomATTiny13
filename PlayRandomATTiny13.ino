#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include "DFPlayerMini.h"
#include "uart.h"

#define wordSize 9
#define MIN_DELAY 100
#define EEPROM_SEED_ADDRESS 0x00

DFPlayerMini player;
uint8_t numberFiles = 123;
uint8_t uartInput[wordSize];
uint16_t seed = 0;

void setup(void)
{
	pinMode(PB1, OUTPUT); // Gives power to the mp3 module
	pinMode(PB2, INPUT);  // Read mp3: On when idle, off when playing
	

	digitalWrite(PB1, HIGH); // Power on mp3
	readUart();				 // Wait for init word

	seed = lfsr16_next(eeprom_read_word((uint16_t *)EEPROM_SEED_ADDRESS) ^ seed);
	eeprom_write_word((uint16_t *)EEPROM_SEED_ADDRESS, seed);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Power down, only wake up by reset
	
	delay(MIN_DELAY);
}

void loop(void)
{
	// updateNumberFiles(); // NOT WORKING; receiving 0x7F instead of 0xD8
	play();
	waitForIdle();

	digitalWrite(PB1, LOW);				 // Cut down the module power
	sleep_mode();						 // Good night
}

void play()
{
	bool playing = false;
	do
	{
		delay(MIN_DELAY);
		player.sendCommand(PLAY_TRACK_COMMAND, (seed % (numberFiles - 1) + 1));
		delay(MIN_DELAY);
		playing = !digitalRead(PB2);
	} while (!playing); // Wait for the track to end
	player.sendCommand(SET_VOLUME_COMMAND, 30); // Set the volume to maximum
	delay(MIN_DELAY);
	player.sendCommand(SET_VOLUME_ADJUST_COMMAND, 31); // Set the volume adjustement to maximum
	delay(MIN_DELAY);
}

void waitForIdle()
{
	delay(MIN_DELAY);
	bool isFinished = false;
	do
	{
		isFinished = digitalRead(PB2);
		delay(MIN_DELAY);
	} while (!isFinished); // Wait for the track to end
	delay(MIN_DELAY);
}

void updateNumberFiles(void)
{
	delay(MIN_DELAY);
	player.sendCommand(U_DISK_NUMBER_FILES_QUERY);
	readUart();

	numberFiles = uartInput[6];
}

void readUart(void)
{
	uint8_t *p;
	p = uartInput;
	while ((*(p++) = uart_getc()) != 0xEF && (p - uartInput) < wordSize);
	*p = 0;
}

uint16_t lfsr16_next(uint16_t n)
{
	return (n >> 0x01U) ^ (-(n & 0x01U) & 0xB400U);
}