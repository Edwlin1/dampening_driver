/*
 * dampening_driver.h
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

#ifndef DAMPENING_DRIVER_H_
#define DAMPENING_DRIVER_H_

/* Kernel Includes -------------------------------------------------------- */
#include "linux/module.h"
#include "linux/fs.h"
#include "linux/miscdevice.h"

int dampening_driver_init(void);
void dampening_driver_exit(void);

//ssize_t write(struct file* file, const char __user* buff, size_t count, loff_t* ppos);

#endif /* DAMPENING_DRIVER_H_ */
