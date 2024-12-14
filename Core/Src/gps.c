#include "gps.h"

/* Indicate income messages */
extern QueueHandle_t gpsReceiver;

int receiveMessage ( uart_ds *uart2, char *message ){

	memset(message,'\0',90);
	uint8_t d = 1;

	while (1) {

		xQueueReceive(gpsReceiver,&d,portMAX_DELAY);

		usart2_dma_check_buffer(uart2,message);

		/* Check if message begin has been received */
		if ( strchr(message, 0x24 ) != NULL ){
			/* Check if message end has been received */
			if ( strchr(message,0x0a) != NULL ){
				return 0;

			}
			GPIOB->ODR ^= ODR_PB7;
		}

	}
}

int parseMessage ( gps *gpsStruct, char *msg ){

	if ( strstr(msg,GPGGA_HEADER) == NULL){
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

		gpsStruct->isValid = atoi(strtok(NULL,","));

		if (atoi(strtok(NULL,","))){
			return 0;
		}


	return 0;
}
