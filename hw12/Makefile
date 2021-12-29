CONFIG_MODULE_SIG = n
TARGET_MODULE := demo

obj-m := $(TARGET_MODULE).o
FLAGS := -std=gnu11

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: client
	$(MAKE) -C $(KDIR) M=$(PWD) modules

client: client.c
	$(CC) -o $@ $^

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm client