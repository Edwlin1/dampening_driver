/*
 * module.h
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

#ifndef TEST_SUPPORT_LINUX_MODULE_H_
#define TEST_SUPPORT_LINUX_MODULE_H_

#define __init
#define __exit

#define module_init(x)
#define module_exit(x)

#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)

#include <x86_64-linux-gnu/sys/types.h>

#define container_of(container, type, member)  (type*) data_injector();

void* data_injector();

#define pr_info(...)
#define pr_cont(...)


#endif /* TEST_SUPPORT_LINUX_MODULE_H_ */
