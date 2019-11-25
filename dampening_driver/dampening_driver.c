/*
 * dampening_driver.c
 *
 *  Created on: Nov 24, 2019
 *      Author: edward
 */

/* Include files ---------------------------------------------------------- */
#include "dampening_driver.h"
#include "linux/string.h"

/* Private defines -------------------------------------------------------- */

/* Private variables ------------------------------------------------------ */
static const struct file_operations file_operations_handle = {
		.open = dampening_driver_open,
		.release = dampening_driver_release,
		.write = dampening_driver_write,
		.read = dampening_driver_read,
};

struct dampening_driver_data driver_data;


int __init dampening_driver_init(void)
{
	int return_code;

	driver_data.miscdevice_handle.minor = MISC_DYNAMIC_MINOR;
	driver_data.miscdevice_handle.name = "dampening_driver";
	driver_data.miscdevice_handle.fops = &file_operations_handle;

	return_code = misc_register(&driver_data.miscdevice_handle);
	if(return_code != RET_SUCCESS) {
		pr_info("DAMPENING DRIVER ERR: Could not register misc device, return code: %d\n", return_code);
		return return_code;
	}

	driver_data.input_index = 0;
	driver_data.output_write_index = 0;
	driver_data.output_read_index = 0;
	memset(driver_data.input_buffer, 0, sizeof(driver_data.input_buffer));
	memset(driver_data.output_buffer, 0, sizeof(driver_data.output_buffer));

	pr_info("DAMPENING_DRIVER: started\n");

	return RET_SUCCESS;
}

void __exit dampening_driver_exit(void)
{
	misc_deregister(&driver_data.miscdevice_handle);

	pr_info("DAMPENING_DRIVER: exited\n");
}

int dampening_driver_open(struct inode* inode, struct file* file)
{
	pr_info("DAMPENING_DRIVER: open called\n");
	return RET_SUCCESS;
}

int dampening_driver_release(struct inode* inode, struct file* file)
{
	pr_info("DAMPENING_DRIVER: close called\n");
	return RET_SUCCESS;
}

//TODO Confirm how inlining works, perhaps this would be better with a macro?
static inline int get_remove_element_index(unsigned int index) {
	int adjusted_index = index - AVERAGED_ELEMENTS;
	if(adjusted_index >= 0) {
		return adjusted_index;
	} else {
		return AVERAGING_BUFFER_SIZE - adjusted_index;
	}
}

ssize_t dampening_driver_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset)
{
	//TODO Add buffer wraparound handling
	int copied_data = 0;
	int i;
	struct dampening_driver_data* p_driver_data = container_of(file->private_data, struct dampening_driver_data, miscdevice_handle);
	int calculation_index = p_driver_data->input_index;
	int available_space = AVERAGING_BUFFER_SIZE - p_driver_data->input_index;

	pr_info("DAMPENING_DRIVER: write called\n");

	if(size == 0 || size > AVERAGING_BUFFER_SIZE)
			return -EINVAL;

	if(size < available_space) {
		if(copy_from_user(&p_driver_data->input_buffer[p_driver_data->input_index], user_buffer, size) != 0) {
			return -EFAULT;
		}
		p_driver_data->input_index += size;
		copied_data = size;
	} else {
		if(copy_from_user(&p_driver_data->input_buffer[p_driver_data->input_index], user_buffer, available_space) != 0) {
			return -EFAULT;
		}
		p_driver_data->input_index = 0;
		copied_data = available_space;
	}

	pr_info("DAMPENING_DRIVER: Input buffer state: ");
	for(i = 0; i < AVERAGING_BUFFER_SIZE; i++) {
		pr_cont("%d,", p_driver_data->input_buffer[i]);
	}
	pr_cont("\n");
	pr_info("DAMPENING_DRIVER: Output buffer state: ");
	for(i = 0; i < AVERAGING_BUFFER_SIZE; i++) {
		pr_cont("%d,", p_driver_data->output_buffer[i]);
	}
	pr_cont("\n");

	pr_info("DAMPENING_DRIVER: input_index: %d\n", p_driver_data->input_index);
	pr_info("DAMPENING_DRIVER: output_write_index: %d\n", p_driver_data->output_write_index);
	pr_info("DAMPENING_DRIVER: output_read_index: %d\n", p_driver_data->output_read_index);
	pr_info("DAMPENING_DRIVER: current_sum: %d\n", p_driver_data->current_sum);

	//TODO Rework to for-loop to avoid possibility of eternal loops?
	while(calculation_index != p_driver_data->input_index) {
		pr_info("DAMPENING_DRIVER: calculation_index: %d\n", calculation_index);

		if(calculation_index >= 8 || (p_driver_data->input_buffer[p_driver_data->input_index+1] != 0)) {
			pr_info("DAMPENING_DRIVER: standard case\n");

			p_driver_data->current_sum = p_driver_data->current_sum
										 - p_driver_data->input_buffer[get_remove_element_index(calculation_index)]
										 + p_driver_data->input_buffer[calculation_index];

			//Divide by 8
			p_driver_data->output_buffer[p_driver_data->output_write_index] = p_driver_data->current_sum >> 3;

			p_driver_data->output_write_index++;
			if(p_driver_data->output_write_index >= AVERAGING_BUFFER_SIZE)
				p_driver_data->output_write_index = 0;

			calculation_index++;
			if(calculation_index >= AVERAGING_BUFFER_SIZE)
				calculation_index = 0;

		} else {
			pr_info("DAMPENING_DRIVER: start case\n");

			p_driver_data->current_sum += p_driver_data->input_buffer[calculation_index];
			p_driver_data->output_buffer[p_driver_data->output_write_index] = p_driver_data->current_sum / (calculation_index+1);

			p_driver_data->output_write_index++;
			if(p_driver_data->output_write_index >= AVERAGING_BUFFER_SIZE) {
				//This should not happen - since calculation_index is less than 8 - but avoid overflow and return an error
				p_driver_data->output_write_index = 0;
				//TODO Use better error code.
				return -EINVAL;
			}

			calculation_index++;
		}
	}

	pr_info("DAMPENING_DRIVER: Output buffer state: ");
	for(i = 0; i < AVERAGING_BUFFER_SIZE; i++) {
		pr_cont("%d,", p_driver_data->output_buffer[i]);
	}
	pr_cont("\n");

	pr_info("DAMPENING_DRIVER: input_index: %d\n", p_driver_data->input_index);
	pr_info("DAMPENING_DRIVER: output_write_index: %d\n", p_driver_data->output_write_index);
	pr_info("DAMPENING_DRIVER: output_read_index: %d\n", p_driver_data->output_read_index);

	return copied_data;
}

ssize_t dampening_driver_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
	//FIXME In order to make reduce unnecessary overhead, wrap around end of buffer instead of truncate

	int available_data;
	struct dampening_driver_data* p_driver_data = container_of(file->private_data, struct dampening_driver_data, miscdevice_handle);

	pr_info("DAMPENING_DRIVER: read called\n");

	if(size == 0 || size > AVERAGING_BUFFER_SIZE)
		return -EINVAL;

	if(p_driver_data->output_write_index >=  p_driver_data->output_read_index)
		available_data = p_driver_data->output_write_index - p_driver_data->output_read_index;
	else
		available_data = AVERAGING_BUFFER_SIZE - p_driver_data->output_read_index;

	pr_info("DAMPENING_DRIVER: available_data: %d\n", available_data);

	if(available_data == 0)
		return -ENODATA;

	if(copy_to_user(user_buffer, &p_driver_data->output_buffer[p_driver_data->output_read_index], available_data) != 0)
		return -EFAULT;

	p_driver_data->output_read_index += available_data;

	if(p_driver_data->output_read_index >= AVERAGING_BUFFER_SIZE)
		p_driver_data->output_read_index = 0;

	pr_info("DAMPENING_DRIVER: input_index: %d\n", p_driver_data->input_index);
	pr_info("DAMPENING_DRIVER: output_write_index: %d\n", p_driver_data->output_write_index);
	pr_info("DAMPENING_DRIVER: output_read_index: %d\n", p_driver_data->output_read_index);

	return available_data;
}

module_init(dampening_driver_init);
module_exit(dampening_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edward Linderoth-Olson <edward.linderotholson@gmail.com>");
MODULE_DESCRIPTION("Driver for dampening input data using a rolling average");
