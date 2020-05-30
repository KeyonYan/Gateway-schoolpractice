NAME = kernel_sensor

ifneq ($(KERNELRELEASE),)
obj-m := $(NAME).o
 
else
PWD  := $(shell pwd)
KVER := $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build
#KDIR := /usr/src/linux-headers-$(KVER)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions modules.*  Module.*
install:
	sudo insmod $(NAME).ko
	dmesg | tail
	# sudo rm 设备节点目录
	# sudo mknod 设备节点目录 c major值 minor值
uninstall:
	sudo rmmod $(NAME).ko

endif

# dmesg | tail  查看内核打印内容
