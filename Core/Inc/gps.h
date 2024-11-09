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

	uint8_t _longitude_attitude;
	float _longitude;

	uint8_t _latitude_attitude;
	float _latitude;

} gps;

/*!
 *  parseMessage - parse $GPGGA messages
 *
 *  Parse NMEA GPGGA message and extract location coordinates.
 */
int parseMessage ( gps *gpsStruct, char *msg, size_t length);



#endif /* INC_GPS_H_ */
