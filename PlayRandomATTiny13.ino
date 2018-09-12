#include <Arduino.h>
#include <avr/sleep.h>
#include "DFPlayerMini.h"
#include "uart.h"

DFPlayerMini player;

void setup(void)
{
	pinMode(PB1, OUTPUT); // Gives power to the mp3 module
	pinMode(PB2, INPUT);  // Read mp3: On when idle, off when playing

	digitalWrite(PB1, HIGH); // Power on mp3
	reset();
	play();
	waitForIdle();
	digitalWrite(PB1, LOW); // Cut down the module power

	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Power down, only wake up by reset
}

void loop(void)
{
	sleep_mode(); // Good night
}

void play()
{
	player.sendCommand(VOLUME_COMMAND, 0);	// Hack to play random track
	delay(100);								  // Minimal delay before two commands
	player.sendCommand(RANDOM_TRACK_COMMAND); // Ask the module to play a random track
	delay(100);
	player.sendCommand(NEXT_COMMAND); // But first track is NEVER random so play next
	delay(100);
	player.sendCommand(VOLUME_COMMAND, 30); // Set the volume back on
	delay(100);								// Wait a little bit just in case
}

void reset()
{
	// Wait for initialization sequence
	char c;
	int size = 0;
	while (size < 8) // No idea how many ms that represents
	{
		c = uart_getc();
		if (c != NULL)
		{
			size++;
		}
	}
	delay(200); // Minimal delay for initialization
}

void waitForIdle()
{
	bool isFinished = false;
	do
	{
		isFinished = digitalRead(PB2);
		delay(100);
	} while (!isFinished); // Wait for the track to end
	delay(500);
}
