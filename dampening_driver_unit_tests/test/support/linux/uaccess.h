/*
 * uaccess.h
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

#ifndef TEST_SUPPORT_LINUX_UACCESS_H_
#define TEST_SUPPORT_LINUX_UACCESS_H_

#define __user

#define COPY_USER_SUCCESS 0

unsigned long copy_from_user(void* to, const void __user* from, unsigned long n);
unsigned long copy_to_user(void* to, const void __user* from, unsigned long n);

#endif /* TEST_SUPPORT_LINUX_UACCESS_H_ */
