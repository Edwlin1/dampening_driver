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
#include "linux/uaccess.h"

/* Defines ---------------------------------------------------------------- */
#define RET_SUCCESS 0

int dampening_driver_init(void);
void dampening_driver_exit(void);

int dampening_driver_write(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);

#endif /* DAMPENING_DRIVER_H_ */
