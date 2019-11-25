/*
 * dampening_driver_test.c
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

/* Ceedling & Standard Includes ------------------------------------------- */
#include "unity.h"
#include "string.h"

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

void test_dampening_driver_init_register_fails(void)
{
	misc_register_ExpectAndReturn(DUMMY_POINTER, MISCDEVICE_ERR);
	misc_register_IgnoreArg_device();

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
	loff_t offset;
	struct file test_file;
	char buffer[] = "Expected";

	struct dampening_driver_data test_state;
	memset(&test_state, 0, sizeof(test_state));
	data_injector_ExpectAndReturn(&test_state);

	copy_from_user_ExpectAndReturn(DUMMY_POINTER, DUMMY_POINTER, sizeof(buffer), COPY_USER_SUCCESS);
	copy_from_user_IgnoreArg_from();
	copy_from_user_IgnoreArg_to();


	TEST_ASSERT_EQUAL(sizeof(buffer), dampening_driver_write(&test_file, buffer, sizeof(buffer), &offset));
}

void test_dampening_driver_read_all_data_transferred(void)
{
	loff_t offset;
	struct file test_file;
	char expected_buffer[] = "Expected";
	char actual_buffer[]   = "Actual00";

	struct dampening_driver_data test_state;
	memset(&test_state, 0, sizeof(test_state));
	memcpy(test_state.output_buffer, expected_buffer, sizeof(expected_buffer));
	test_state.output_write_index = sizeof(expected_buffer);

	data_injector_ExpectAndReturn(&test_state);

	//TODO: get this Ceedling command to work
	//copy_to_user_ExpectWithArray(expected_buffer, sizeof(expected_buffer), test_state.output_buffer, sizeof(expected_buffer), sizeof(actual_buffer), COPY_USER_SUCCESS);
	copy_to_user_ExpectAndReturn(DUMMY_POINTER, DUMMY_POINTER, sizeof(expected_buffer), COPY_USER_SUCCESS);
	copy_to_user_IgnoreArg_from();
	copy_to_user_IgnoreArg_to();

	TEST_ASSERT_EQUAL(sizeof(actual_buffer), dampening_driver_read(&test_file, actual_buffer, sizeof(actual_buffer), &offset));
}

//TODO Add unit tests for failure of each copy_to_user/copy_from_user
//TODO Add unit tests for requests for more data than exists, more data than there is buffer space etc

void test_dampening_driver_average_one_byte(void)
{
	loff_t offset;
	struct file test_file;
	char input_data[1] = {23};

	struct dampening_driver_data test_state;
	memset(&test_state, 0, sizeof(test_state));
	data_injector_ExpectAndReturn(&test_state);

	copy_from_user_ExpectAndReturn(DUMMY_POINTER, DUMMY_POINTER, 1, COPY_USER_SUCCESS);
	copy_from_user_IgnoreArg_to();
	copy_from_user_ReturnArrayThruPtr_to(input_data, sizeof(input_data));
	copy_from_user_IgnoreArg_from();

	dampening_driver_write(&test_file, input_data, sizeof(input_data), &offset);

	TEST_ASSERT_EQUAL(test_state.output_buffer[0], input_data[0]);
}

void test_dampening_driver_average_eight_bytes(void)
{
	loff_t offset;
	struct file test_file;
	char input_data[8] = {10, 10, 50, 100, 100, 200, 100, 1};
	//FIXME Use a completely defined target dampening_driver_data struct instead, containing correct values for indices etc.
	char expected_output_data[8] = {10, 10, 23, 42, 54, 78, 81, 71};

	struct dampening_driver_data test_state;
	memset(&test_state, 0, sizeof(test_state));
	data_injector_ExpectAndReturn(&test_state);

	copy_from_user_ExpectAndReturn(DUMMY_POINTER, DUMMY_POINTER, sizeof(input_data), COPY_USER_SUCCESS);
	copy_from_user_IgnoreArg_to();
	copy_from_user_ReturnArrayThruPtr_to(input_data, sizeof(input_data));
	copy_from_user_IgnoreArg_from();

	dampening_driver_write(&test_file, input_data, sizeof(input_data), &offset);

	//FIXME Reads wrong data for some reason.
	TEST_ASSERT_EQUAL_INT8_ARRAY(expected_output_data, test_state.output_buffer, sizeof(expected_output_data));
}

void test_dampening_driver_average_nine_bytes(void)
{
	//TODO Implement test
	TEST_ASSERT_EQUAL(0, 1);
}

//TODO Add unit tests for buffer overwrite, i.e. more than 32 bytes, multiple and single write operations

//TODO Add unit tests for read function, at least same operations as for write
