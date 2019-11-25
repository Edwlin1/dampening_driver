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
#define AVERAGING_BUFFER_SIZE 32
#define AVERAGED_ELEMENTS 8

struct dampening_driver_data {
	struct miscdevice miscdevice_handle;
	char input_buffer[AVERAGING_BUFFER_SIZE];
	char output_buffer[AVERAGING_BUFFER_SIZE];
	unsigned int input_index;
	unsigned int output_write_index;
	unsigned int output_read_index;
	unsigned int current_sum;
};

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

ssize_t dampening_driver_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

//TODO Add documentation
ssize_t dampening_driver_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);

//TODO Add documentation
int dampening_driver_open(struct inode* inode, struct file* file);

//TODO Add documentation
int dampening_driver_release(struct inode* inode, struct file* file);

#endif /* DAMPENING_DRIVER_H_ */
