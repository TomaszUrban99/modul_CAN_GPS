#include "gps.h"

int parseMessage ( gps *gpsStruct, char *msg, size_t length){

	for ( int i = 0; i < strlen(GPGGA_HEADER); ++i){
		if ( msg[i] != GPGGA_HEADER[i] )
			return 1;
	}

		if ( strtok(msg,",") == NULL){
			return 1;
		}

		if ( strtok(NULL,",") == NULL){
			return 1;
		}

		gpsStruct->_latitude = atof(strtok(NULL,","));
		gpsStruct->_latitude_attitude = strtok(NULL,",")[0];

		gpsStruct->_longitude = atof(strtok(NULL,","));
		gpsStruct->_longitude_attitude = strtok(NULL,",")[0];

		if (atoi(strtok(NULL,","))){
			return 0;
		}


	return 1;
}
