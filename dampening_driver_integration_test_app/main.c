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
#include <string.h>

int main (int argc, char** argv)
{
	int dampening_driver_file = open("/dev/dampening_driver", O_RDWR);

	if(dampening_driver_file < 0) {
		perror("Error: Failed to open device file: /dev/HelloWorldDevice\n");
		return 0;
	}

	char input_data[8] = {10, 10, 50, 100, 100, 200, 100, 1};
	ssize_t written_bytes = write(dampening_driver_file, input_data, sizeof(input_data));

	if(written_bytes < 0) {
		perror("Error: failed to write to device file");
	} else {
		printf("Amount of data written: %li\n", written_bytes);
	}

	char output_data[8];
	memset(output_data, 0, sizeof(output_data));

	ssize_t read_bytes = read(dampening_driver_file, output_data, sizeof(output_data));

	if(read_bytes < 0) {
		perror("Error: failed to read from device file");
		return 0;
	} else {
		if(read_bytes <= sizeof(output_data)) {
			int i;
			printf("Read data was: ");
			for(i = 0; i < read_bytes-1; i++) {
				printf("%d, ", output_data[i]);
			}
			printf("%d\n", output_data[read_bytes]);
		} else {
			printf("Error: read returned illegal write size.\n");
			return 0;
		}
	}

	close(dampening_driver_file);

	return 0;
}


