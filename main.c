#include "platform.h"
#include "vl53l8cx_api.h"

/***********************************/
/*   VL53L8CX ULD basic example    */
/***********************************/
/*
* This example is the most basic. It initializes the VL53L8CX ULD, and starts
* a ranging to capture 10 frames.
*
* By default, ULD is configured to have the following settings :
* - Resolution 4x4
* - Ranging period 1Hz
*
* In this example, we also suppose that the number of target per zone is
* set to 1 , and all output are enabled (see file platform.h).
*/
int main(void)
{

	/*********************************/
	/*   VL53L8CX ranging variables  */
	/*********************************/

	uint8_t status = 255, loop, isAlive, isReady;
	VL53L8CX_Configuration 	Dev;			/* Sensor configuration */
	VL53L8CX_ResultsData 	Results;		/* Results data from VL53L8CX */

	/*********************************/
	/*      Customer platform        */
	/*********************************/

	/* Fill the platform structure with customer's implementation. For this
	* example, only the I2C address is used.
	*/
	Dev.platform.address = 0x29; //VL53L8CX_DEFAULT_I2C_ADDRESS;
	Dev.platform.fd = open("/dev/i2c-1", O_RDWR);
	if (Dev.platform.fd < 0)
	{
		perror("Failed to open i2c bus.");
		return 1;
	}
	if (ioctl(Dev.platform.fd, I2C_SLAVE, 0x29) < 0) 
	{
		perror("Failed to acquire bus access.");
		close(Dev.platform.fd);
		return status;
	}

	/*********************************/
	/*   Power on sensor and init    */
	/*********************************/

	/* (Optional) Check if there is a VL53L8CX sensor connected */
	status = vl53l8cx_is_alive(&Dev, &isAlive);
	if(!isAlive || status)
	{
		printf("VL53L8CX not detected at requested address\n");
		return status;
	}

	/* (Mandatory) Init VL53L8CX sensor */
	status = vl53l8cx_init(&Dev);
	if (status)
	{
		printf("VL53L8CX ULD Loading failed\n");
		return status;
	}

	printf("VL53L8CX ULD ready ! (Version : %s)\n", VL53L8CX_API_REVISION);

	/*********************************/
	/*         Ranging loop          */
	/*********************************/

	status = vl53l8cx_start_ranging(&Dev);

	loop = 0;
	while(loop < 10)
	{
		/* Use polling function to know when a new measurement is ready.
		 * Another way can be to wait for HW interrupt raised on PIN A1
		 * (INT) when a new measurement is ready */
 
		status = vl53l8cx_check_data_ready(&Dev, &isReady);

		if(isReady)
		{
			vl53l8cx_get_ranging_data(&Dev, &Results);

			// Print sensor data as 4x4 matrix
			printf("\033[3;1H");
			printf("Sensor data data no : %3u\n", Dev.streamcount);
			for(int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					printf("%4d\t", Results.distance_mm[VL53L8CX_NB_TARGET_PER_ZONE * (4 * y + x)]);
				}
				printf("\n");
			}

			// Print all zones status as 4x4 matrix
			printf("\033[7;1H\n");
			printf("All zones status for data no : %3u\n", Dev.streamcount);
			for(int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					printf("%d\t", Results.target_status[VL53L8CX_NB_TARGET_PER_ZONE * (4 * y + x)]);
				}
				printf("\n");
			}

			loop++;
		}

		/* Wait a few ms to avoid too high polling (function in platform
		 * file, not in API) */
		WaitMs(&(Dev.platform), 5);
	}

	status = vl53l8cx_stop_ranging(&Dev);
	printf("End of ULD demo\n");
	return status;
}
