/*
 * gps.h
 *
 *  Created on: Nov 3, 2024
 *      Author: tomasz
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "gpiob.h"
#include "stm32f429xx.h"

#define GPS_BUFFER						256

#define GPGGA_HEADER					"$GPGGA"

typedef struct gps {

	char gps_buffer [GPS_BUFFER];
	uint8_t _head;
	uint8_t _tail;

	uint8_t _received_char;

	uint8_t _longitude_attitude;
	float _longitude;

	uint8_t _latitude_attitude;
	float _latitude;

	uint8_t _hours;
	uint8_t _minute;
	uint8_t _seconds;

	uint8_t _gpsEnd;

	uint8_t _usart2_idle;

} gps;

int parseMessage ( gps *gpsStruct, char *msg, size_t length);

/*
void store_char ( gps *_gpsStruct, char ch);

int get_char ( gps *_gpsStruct );

int get_line ( gps *_gpsStruct );

void parse_GPGAA ( gps *_gpsStruct, char *_ptr );

void parse_message ( gps *_gpsStruct );*/



#endif /* INC_GPS_H_ */
