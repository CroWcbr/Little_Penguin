KERNEL_VERSION = $(shell uname -r)
KERNELDIR ?= /lib/modules/$(KERNEL_VERSION)/build
PWD := $(shell pwd)

MODULE_NAME = main

obj-m += $(MODULE_NAME).o

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

fclean: clean
	rm -f main.ko

re: fclean all

.PHONY: clean fclean all re
