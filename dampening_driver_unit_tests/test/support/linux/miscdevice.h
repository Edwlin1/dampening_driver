/*
 * miscdevice.h
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

#ifndef TEST_SUPPORT_LINUX_MISCDEVICE_H_
#define TEST_SUPPORT_LINUX_MISCDEVICE_H_

typedef struct {} miscdevice;

int misc_register(miscdevice* device);

#endif /* TEST_SUPPORT_LINUX_MISCDEVICE_H_ */
