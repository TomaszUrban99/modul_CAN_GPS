/*
 * packet.h
 *
 *  Created on: Nov 26, 2024
 *      Author: tomasz
 */

#ifndef INC_PACKET_H_
#define INC_PACKET_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "can.h"
#include "gps.h"

typedef struct packet {

	struct can_data_collection _obd_data;

	struct gps _gps_data;

} packet;

/*!
 * \brief Prepare JSON string based on data in packet
 */
int prepare_json ( struct packet *packet,
		uint8_t *packet_json );

#endif /* INC_PACKET_H_ */
