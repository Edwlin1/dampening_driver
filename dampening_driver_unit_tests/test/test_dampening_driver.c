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
#define SUCCESS 0

/* State Setup and Teardown Methods --------------------------------------- */
void setUp(void)
{

}

void tearDown(void)
{

}

/* Test Cases ------------------------------------------------------------- */

void test_module_init(void)
{
	misc_register_ExpectAndReturn(DUMMY_POINTER, SUCCESS);
	misc_register_IgnoreArg_device();

	TEST_ASSERT_EQUAL(SUCCESS, dampening_driver_init());
}

void test_module_exit(void)
{

}
