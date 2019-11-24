/*
 * fs.h
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

#ifndef TEST_SUPPORT_LINUX_FS_H_
#define TEST_SUPPORT_LINUX_FS_H_

#include "linux/uaccess.h"

struct file { };

typedef struct file file_t;

struct file_operations {
	ssize_t (*write)(file_t *file, const char __user *user_buffer, size_t size, loff_t *offset);
	ssize_t (*read)(file_t *file, char __user *user_buffer, size_t size, loff_t *offset);
};

typedef struct file_operations file_operations_t;

#endif /* TEST_SUPPORT_LINUX_FS_H_ */
