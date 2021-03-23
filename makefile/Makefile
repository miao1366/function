PLAT ?= none

.PHONY : none linux mac all

.PHONY : default

default :
	$(MAKE) $(PLAT)

none :
	@echo "none message"

linux :
mac :

linux mac :
	$(MAKE) all

all : main.c
	gcc main.c