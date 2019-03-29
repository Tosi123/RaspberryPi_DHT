#include <wiringPi.h>
#include <stdint.h>
#include "util.h"

float temp;
float hum;

void read_dht_data(int DHT_PIN)
{
	int MAX_TIMINGS = 85;
	int data[5] = { 0, 0, 0, 0, 0 };

	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	/* pull pin down for 18 milliseconds */
	pinMode(DHT_PIN, OUTPUT);
	digitalWrite(DHT_PIN, LOW);
	delay(18);

	/* prepare to read the pin */
	digitalWrite(DHT_PIN, HIGH);
	delayMicroseconds(40);
	pinMode(DHT_PIN, INPUT);

	/* detect change and read data */
	for (i = 0; i < MAX_TIMINGS; i++)
	{
		counter = 0;
		while (digitalRead(DHT_PIN) == laststate)
		{
			counter++;
			delayMicroseconds(2);
			if (counter == 255)
			{
				break;
			}
		}
		laststate = digitalRead(DHT_PIN);
		if (counter == 255)
			break;
		/* ignore first 3 transitions */
		if ((i >= 4) && (i % 2 == 0))
		{
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if (counter > 16)
				data[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ((j >= 40) &&
		(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
	{
		float h = (float)((data[0] << 8) + data[1]) / 10;
		if (h > 100)
		{
			h = (float)data[0];    // for DHT11
		}
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if (c > 125)
		{
			c = (float)data[2];    // for DHT11
		}
		if (data[2] & 0x80)
		{
			c = -c;
		}
		sprintf(log_tmp, "Successful Temperature And Humidity Extraction (Humidity=%.1f%%/Temperature=%.1f*C)\n", h, c);
		log_write("INFO", __FILE__, __func__, __LINE__, log_tmp);

		temp = c;
		hum = h;
	}
	else {
		sprintf(log_tmp, "Failure Of Temperature And Humidity Extraction\n");
		log_write("WARN", __FILE__, __func__, __LINE__, log_tmp);
		temp = -1;
		hum = -1;
	}
}