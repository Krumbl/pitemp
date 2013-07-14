/*
 * Poll temperature and humidity every 2 seconds from a DHT22 sensor
 *
 * see 
 * http://learn.adafruit.com/dht-humidity-sensing-on-raspberry-pi-with-gdocs-logging/overview
 * http://www.jasm.eu/2013/05/28/raspberry-pi-temperature-and-humidity-sensor/
 * http://rpidude.blogspot.de/2012/11/temp-sensor-and-wiringpi.html
 *
 */

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXTIMINGS 85
#define DHTPIN 7

int readDHT22();

int main(int argc, char **argv)
{
	printf("Raspberry Pi wiringPi DHT22 Temperature Test\n");
	
	if (wiringPiSetup() == -1) {
		printf("Need wiringPi installed to run\n");
		exit(1);
		}

	for (int i = 0; i < 5; i++) {
		readDHT22();
		delay(2000);  // sleep between readings
	}

}

int data[5];
int readDHT22() {
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	// flushing pin ???
	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW); 
	delay(18);
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40);

	pinMode(DHTPIN, INPUT);

	// reading
	int lastState = HIGH;
	int counter = 0;
	int j = 0;
	for (int i = 0; i < MAXTIMINGS; i++) {
		counter = 0;

		while (digitalRead(DHTPIN) == lastState) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
				break;
			}
		}

		lastState = digitalRead(DHTPIN);

		if (counter == 255) break; // not sure

		if ((i > 3) && (i%2 == 0)) { // I really need to read the datasheet
			data[j/8] <<= 1;
			if (counter > 16)
				data[j/8] |= 1;
			j++;
		}
	}

	#ifdef DEBUG
		// print data read
		printf("Data (%d): 0x%x 0x%x 0x%x 0x%x 0x%x\n", j, data[0], data[1], data[2], data[3], data[4]);
	#endif
	
	// verify checksum
	if ((j >= 40) &&
			(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
		float h = (data[0] * 256 + data[1]) / 10;
		float c = ((data[2] &0x7F) * 256 + data[3]) / 10.0;
		if (data[2] & 0x80)
			c *= -1;
		float f = c * 1.8f + 32;
		printf("Temp = %.1f *F %.1f C, Hum = %.1f \%\n", f, c, h);
	} else {
		printf("BAD DATA\n");
	}

}
