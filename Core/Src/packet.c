/*
 * packet.c
 *
 *  Created on: Nov 26, 2024
 *      Author: tomasz
 */
#include "packet.h"

int prepare_json ( struct packet *packet,
		uint8_t *packet_json ){

    return sprintf((char *) packet_json,
    		"{\"ivd\":\"%d\",\"lat\":\"%f\",\"latAtt\":\"%c\",\"lng\":\"%f\",\"lngAtt\":\"%c\",\"eng_rpm\":\"%d\",\"eng_load\":\"%d\",\"tnk_lvl\":\"%d\"}",
        packet->_gps_data.isValid, packet->_gps_data._latitude,packet->_gps_data._latitude_attitude,
		packet->_gps_data._longitude,packet->_gps_data._longitude_attitude,
		packet->_obd_data._engine_rpm,packet->_obd_data._engine_load,packet->_obd_data._tank_level);
}
