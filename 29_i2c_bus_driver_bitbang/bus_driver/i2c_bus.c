#include <linux/i2c.h>
#include <linux/module.h>
#include "i2c_bitbang.h"

/* Private macros */
#define MOD_NAME "alman-bus"
#define MOD_INFO MOD_NAME ": "

#define ADAPTER_NAME "ALM_I2C_ADAPTER"
#define GPIO_SCL (4)
#define GPIO_SDA (17)

/**
 * alm_func - list supported functionalities for this bus driver
 *
 * Return: merged functionalities
 */
static u32 alm_func(struct i2c_adapter *adapter)
{
	return (I2C_FUNC_I2C | I2C_FUNC_SMBUS_QUICK | I2C_FUNC_SMBUS_BYTE |
		I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_WORD_DATA |
		I2C_FUNC_SMBUS_BLOCK_DATA);
}

/**
 * alm_i2c_xfer - low level i2c routine
 *
 * Return: errno
 */
static int alm_i2c_xfer(struct i2c_adapter *adapter, struct i2c_msg *msgs,
			int num)
{
	int ret = 0, i, j;
	struct i2c_gpio *gpio;
	struct i2c_msg *msg;

	if ((gpio = i2c_gpio_alloc()) == NULL) 
		return -ENOMEM;

	if (i2c_gpio_init(gpio, GPIO_SCL, GPIO_SDA) < 0)
		return -EINVAL;

	i2c_bbang_start(gpio);

	for (i = 0; i < num; i++) {
		msg = &msgs[i];

		if (i2c_bbang_send(gpio, msg->addr, msg->buf, msg->len))
			ret++;

		pr_info(MOD_INFO
			"[%s]: [count: %d] [addr: 0x%X] [len: %d] [data: ",
			__func__, i, msg->addr, msg->len);

		for (j = 0; j < msg->len; j++)
			pr_cont("0x%02X ", msg->buf[j]);
		pr_cont("]\n");
	}

	i2c_bbang_stop(gpio);
	i2c_gpio_deinit(gpio);

	return 0;
}

/**
 * alm_smbus_xfer - low level smbus routine
 *
 * Return: errno
 */
static int alm_smbus_xfer(struct i2c_adapter *adapter, u16 addr,
			  unsigned short flags, char read_write, u8 command,
			  int size, union i2c_smbus_data *data)
{
	pr_info(MOD_INFO "[%s]: Implement me\n", __func__);
	return 0;
}

/* I2C algorithm structure */
static struct i2c_algorithm alm_i2c_algorithm = {
	.smbus_xfer = alm_smbus_xfer,
	.master_xfer = alm_i2c_xfer,
	.functionality = alm_func,
};

/* I2C adapter structure */
static struct i2c_adapter alm_i2c_adapter = {
	.owner = THIS_MODULE,
	.class = I2C_CLASS_HWMON, // | I2C_CLASS_SPD,
	.algo = &alm_i2c_algorithm,
	.name = ADAPTER_NAME,
	.nr = 7,
};

/* Module init callback */
static int __init alm_init(void)
{
	if (i2c_add_numbered_adapter(&alm_i2c_adapter) < 0) {
		pr_err(MOD_INFO "Can't add I2C adapter\n");
		return -1;
	}

	pr_info(MOD_INFO "Driver added\n");
	return 0;
}

/* Module exit callback */
static void __exit alm_exit(void)
{
	i2c_del_adapter(&alm_i2c_adapter);
	pr_info(MOD_INFO "Driver removed\n");
}

module_init(alm_init);
module_exit(alm_exit);

/* Module description */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pudja Mansyurin");
MODULE_DESCRIPTION(MOD_NAME);
MODULE_VERSION("3:5.4");
