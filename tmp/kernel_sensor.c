/* Module: 模拟传感器设备-内核驱动
 * Author: 闫科宇
 * Date:   2020-05-30 19:26:12
 * Desc:   模拟传感器设备而写的一个内核驱动程序，并生成其设备节点
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE_NAME "sensor_dev_yky"
#define DEVICE_PATH "/dev/sensor_dev_yky"

static struct cdev chr_dev;
static dev_t ndev;
// static struct class *myclass;
// static struct device *mydevice;

// 设备打开
static int chr_open(struct inode* inode, struct file* filp)
{
	int major;
	int minor;
	
	major = MAJOR(inode->i_rdev);
	minor = MINOR(inode->i_rdev);
	
	printk("chr_open, major = %d, minor = %d\n", major, minor);
    return 0;
}

// read操作
static ssize_t chr_read(struct file* filp, char __user* u, size_t sz, loff_t* off)
{
	char buf[2];
	int temperatureVal; // 采集到的温度值
	int humidityVal; // 采集到的湿度值
	printk("chr_read process!\n");
	memset(buf, 0, sizeof(buf));
	
	srand((unsigned)time(NULL));
	temperatureVal = rand() % 21 + 20; // 生成一个20~40之间的随机数
	humidityVal = rand() % 71 + 20; // 生成一个20~70之间的随机数
	buf[0] = temperatureVal;
	buf[1] = humidityVal;
	
	int ret;
	ret = copy_to_user(u, buf, sizeof(buf));
	if (ret == 0) {
		return sizeof(buf);
	} else {
		return -1;
	}
	return 0;
}

// write操作
static ssize_t chr_write(struct file* filp, const char __user* buf, ssize_t size, loff_t *ppos) {

}
 
static const struct file_operations chr_ops = 
{
	.owner = THIS_MODULE,
	.read     = chr_read,
	.write    = chr_write,
	.open = chr_open
};
 
static int demo_init(void)
{
	// 1. 创建设备
	int ret;
	cdev_init(&chr_dev, &chr_ops);
	// 2. 动态分配设备号
	ret = alloc_chrdev_region(&ndev, 0, 1, DEVICE_PATH);
	if(ret < 0 )
	{
		return ret;
	}
	// 3. 添加设备
	printk("demo_init(): major = %d, minor = %d\n", MAJOR(ndev), MINOR(ndev));
	ret = cdev_add(&chr_dev, ndev, 1);
	if(ret < 0)
	{
		return ret;
	}
	// 4. 创建设备节点
	//myclass = class_create(THIS_MODULE, DEVICE_NAME);
	//mydevice = device_create(myclass, NULL, ndev, chr_dev, DEVICE_NAME, DEVICE_NAME);
	
	return 0;
}
 
static void demo_exit(void)
{
	printk("demo_exit process!\n");
	cdev_del(&chr_dev);
	unregister_chrdev_region(ndev, 1);
}

module_init(demo_init);
module_exit(demo_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Keyon");
MODULE_DESCRIPTION("A simulate sensor device!");