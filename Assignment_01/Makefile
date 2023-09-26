ifneq ($(KERNELRELEASE),)
	obj-m := main.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
endif

# KERNEL_VERSION = $(shell uname -r)
# KERNELDIR ?= /lib/modules/$(KERNEL_VERSION)/build
# PWD := $(shell pwd)

# MODULE_NAME = main

# obj-m += $(MODULE_NAME).o

# all:
# 	make -C $(KERNELDIR) M=$(PWD) modules

# clean:
# 	make -C $(KERNELDIR) M=$(PWD) clean

# re: clean all

# .PHONY: clean all re
