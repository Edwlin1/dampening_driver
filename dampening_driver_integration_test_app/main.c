/*
 * main.c
 *
 *  Created on: Nov 25, 2019
 *      Author: edward
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main (void)
{
	int dampening_driver_file = open("/dev/dampening_driver", O_RDWR);

	if(dampening_driver_file < 0) {
		perror("Error: Failed to open device file: /dev/HelloWorldDevice\n");
	} else {
		char input_data[8] = {10, 10, 50, 100, 100, 200, 100, 1};
		ssize_t remaining_data = write(dampening_driver_file, input_data, sizeof(input_data));

		if(remaining_data < 0) {
			perror("Error: failed to write to device file");
		} else {
			printf("Data remaining to be sent: %li\n", remaining_data);
		}

		close(dampening_driver_file);
	}

	return 0;
}


