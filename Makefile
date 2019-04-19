XX = g++
CPP_FLAGS = -std=c++11 -ggdb3 -fPIC
SHARED_FLAGS = -fPIC -shared 
LINK = ln -sf

ROOT_DIR = $(shell pwd)
LIBS_DIR = $(ROOT_DIR)/export

COMMON_LIB = $(LIBS_DIR)/libcommon.so

INCLUDE = -I$(ROOT_DIR)/common

export XX CPP_FLAGS ROOT_DIR LIBS_DIR INCLUDE SHARED_FLAGS LINK

SORT_TARGET = sort
MULTI_THREAD_TARGET = multi
MESS_TARGET = mess
TEST_SAFE = test
ATM = atm

all: COMMON $(SORT_TARGET)_d $(MULTI_THREAD_TARGET)_d $(TEST_SAFE)_d $(MESS_TARGET)_d $(ATM)_d

.PHONY: COMMON
COMMON:
	@make -C common

.PHONY: $(SORT_TARGET)_d
$(SORT_TARGET)_d: COMMON
	@make -C sort_algorithm DYNAMIC

.PHONY: $(SORT_TARGET)
$(SORT_TARGET): COMMON
	@make -C sort_algorithm

.PHONY: $(MULTI_THREAD_TARGET)_d
$(MULTI_THREAD_TARGET)_d: COMMON
	@make -C multi_thread DYNAMIC

.PHONY: $(MULTI_THREAD_TARGET)
$(MULTI_THREAD_TARGET): COMMON
	@make -C multi_thread

.PHONY: $(MESS_TARGET)_d
$(MESS_TARGET)_d: COMMON
	@make -C mess DYNAMIC

.PHONY: $(MESS_TARGET)
$(MESS_TARGET): COMMON
	@make -C mess

.PHONY: $(TEST_SAFE)_d
$(TEST_SAFE)_d: COMMON
	@make -C test_safe DYNAMIC

.PHONY: $(TEST_SAFE)
$(TEST_SAFE): COMMON
	@make -C test_safe

.PHONY: $(ATM)_d
$(ATM)_d: COMMON
	@make -C atm DYNAMIC

.PHONY: $(ATM)
$(ATM): COMMON
	@make -C atm

.PHONY: clean
clean :
	@rm -rf $(LIBS_DIR)/*
	@find -name '*.o' -o -name '*.a' -o -name '*.so' -o -name '*.out' -o -name '$(SORT_TARGET)' | xargs rm -rf  
	@make -C mess clean
	@make -C test_safe clean
	@make -C multi_thread clean
	@make -C atm clean
	@make -C common clean
	@make -C sort_algorithm clean
