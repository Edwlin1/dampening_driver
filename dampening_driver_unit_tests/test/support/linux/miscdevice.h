/*
 * miscdevice.h
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

#ifndef TEST_SUPPORT_LINUX_MISCDEVICE_H_
#define TEST_SUPPORT_LINUX_MISCDEVICE_H_

#include "linux/fs.h"

#define MISC_DYNAMIC_MINOR 0

struct miscdevice {
	int 	minor;
	char*  	name;
	const struct file_operations* fops;
};

typedef struct miscdevice miscdevice_t;

#define MISCDEVICE_ERR	1

int misc_register(miscdevice_t* device);

void misc_deregister(miscdevice_t* device);

#endif /* TEST_SUPPORT_LINUX_MISCDEVICE_H_ */
