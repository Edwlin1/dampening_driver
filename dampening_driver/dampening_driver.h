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
#include "linux/errno.h"

/* Defines ---------------------------------------------------------------- */
#define RET_SUCCESS 0

/**
 * dampening_driver_init()
 * Sets up driver resources when inserted into kernel. See kernel log for messages on failure point.
 * @return Returns RET_SUCCESS on success, otherwise the return value of the failing resource request.
 */
int dampening_driver_init(void);

/**
 * dampening_driver_exit()
 * Releases driver resources when removed from kernel.
 */
void dampening_driver_exit(void);

/**
 * dampening_driver_write(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
 * Callback for when a user process activates a syscall to write data to the associated driver file.
 * This function is used to transfer the data to be dampened.
 * @file File used for data transfer
 * @user_buffer Data to be transferred
 * @size Size of data for transfer
 * @offset Offset for data in file
 * @return Returns the amount of data remaining to be transferred. Zero if all data is transferred, negative on error.
 */
int dampening_driver_write(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
int dampening_driver_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);

#endif /* DAMPENING_DRIVER_H_ */
