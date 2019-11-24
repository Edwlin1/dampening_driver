/*
 * dampening_driver.c
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

/* Include file ----------------------------------------------------------- */
#include "dampening_driver.h"

/* Private variables ------------------------------------------------------ */
static struct miscdevice miscdevice_handle = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "dampening_driver",
};

/**
 * dampening_driver_init()
 * Sets up driver resources when inserted into kernel.
 * @return Returns RET_SUCCESS on success, otherwise the return value of the failing resource request.
 * See kernel log for notes on failure point.
 */
int __init dampening_driver_init(void)
{
	int return_code = misc_register(&miscdevice_handle);
	if(return_code != RET_SUCCESS) {
		pr_info("DAMPENING DRIVER ERR: Could not register misc device, return code: %d\n", return_code);
		return return_code;
	}

	return RET_SUCCESS;
}

/**
 * dampening_driver_exit()
 * Releases driver resources when removed from kernel.
 */
void __exit dampening_driver_exit(void)
{
	misc_deregister(&miscdevice_handle);
}

module_init(dampening_driver_init);
module_exit(dampening_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edward Linderoth-Olson <edward.linderotholson@gmail.com>");
MODULE_DESCRIPTION("Driver for dampening input data using a rolling average");
