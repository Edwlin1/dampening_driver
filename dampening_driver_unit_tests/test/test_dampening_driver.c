/*
 * dampening_driver_test.c
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

/* Ceedling Includes ------------------------------------------------------ */
#include "unity.h"

/* Mocked Item Includes --------------------------------------------------- */
#include "mock_module.h"
#include "mock_miscdevice.h"
#include "mock_fs.h"

/* Object under Test Includes --------------------------------------------- */
#include "dampening_driver.h"

/* Defines ---------------------------------------------------------------- */
#define DUMMY_POINTER 0

/* State Setup and Teardown Methods --------------------------------------- */
void setUp(void)
{

}

void tearDown(void)
{

}

/* Test Cases ------------------------------------------------------------- */

void test_module_init_succeeds(void)
{
	misc_register_ExpectAndReturn(DUMMY_POINTER, RET_SUCCESS);
	misc_register_IgnoreArg_device();

	TEST_ASSERT_EQUAL(RET_SUCCESS, dampening_driver_init());
}

void test_module_init_fails(void)
{
	misc_register_ExpectAndReturn(DUMMY_POINTER, MISCDEVICE_ERR);
	misc_register_IgnoreArg_device();

	pr_info_Expect("DAMPENING DRIVER ERR: Could not register misc device, return code: %d\n");

	TEST_ASSERT_EQUAL(MISCDEVICE_ERR, dampening_driver_init());
}

void test_module_exit(void)
{
	misc_deregister_Expect(DUMMY_POINTER);
	misc_deregister_IgnoreArg_device();

	dampening_driver_exit();
}
