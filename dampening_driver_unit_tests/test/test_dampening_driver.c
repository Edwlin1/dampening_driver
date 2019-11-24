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
#include "mock_uaccess.h"
#include "mock_errno.h"

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

void test_dampening_driver_init_succeeds(void)
{
	misc_register_ExpectAndReturn(DUMMY_POINTER, RET_SUCCESS);
	misc_register_IgnoreArg_device();

	TEST_ASSERT_EQUAL(RET_SUCCESS, dampening_driver_init());
}

void test_dampening_driver_init_fails(void)
{
	misc_register_ExpectAndReturn(DUMMY_POINTER, MISCDEVICE_ERR);
	misc_register_IgnoreArg_device();

	pr_info_Expect("DAMPENING DRIVER ERR: Could not register misc device, return code: %d\n");

	TEST_ASSERT_EQUAL(MISCDEVICE_ERR, dampening_driver_init());
}

void test_dampening_driver_exit(void)
{
	misc_deregister_Expect(DUMMY_POINTER);
	misc_deregister_IgnoreArg_device();

	dampening_driver_exit();
}

void test_dampening_driver_write_all_data_transferred(void)
{
	const int data_size = 1;
	copy_from_user_ExpectAndReturn(DUMMY_POINTER, DUMMY_POINTER, data_size, COPY_USER_SUCCESS);
	copy_from_user_IgnoreArg_from();
	copy_from_user_IgnoreArg_to();

	loff_t offset;
	struct file test_file;
	char buffer[] = "Test";

	TEST_ASSERT_EQUAL(sizeof(buffer), dampening_driver_write(&test_file, buffer, sizeof(buffer), &offset));
}

void test_dampening_driver_read_all_data_transferred(void)
{
	const int data_size = 1;
	copy_to_user_ExpectAndReturn(DUMMY_POINTER, DUMMY_POINTER, data_size, COPY_USER_SUCCESS);
	copy_to_user_IgnoreArg_from();
	copy_to_user_IgnoreArg_to();

	loff_t offset;
	struct file test_file;
	char buffer[] = "Test";

	TEST_ASSERT_EQUAL(sizeof(buffer), dampening_driver_read(&test_file, buffer, sizeof(buffer), &offset));
}


