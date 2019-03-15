TARGET_SUPPORT_STATIC = libcommon.a
TARGET_SUPPORT_DYNAMIC = libcommon.so

SRCS_UNIT = $(wildcard *.cpp)
OBJ_UNIT = $(patsubst %.cpp,%.o,$(notdir ${SRCS_UNIT}))

UNIT_DIR = $(shell pwd)

all:$(TARGET_SUPPORT_DYNAMIC)

$(OBJ_UNIT):%.o:%.cpp
	$(XX) $(CPP_FLAGS) -c $< -o $@

$(TARGET_SUPPORT_DYNAMIC):$(OBJ_UNIT)
	$(XX) $(SHARED_FLAGS) $(OBJ_UNIT) -o $@
	ar rcs $(TARGET_SUPPORT_STATIC) $(OBJ_UNIT)
	$(LINK) $(UNIT_DIR)/$@ $(LIBS_DIR)/$@
	$(LINK) $(UNIT_DIR)/$(TARGET_SUPPORT_STATIC) $(LIBS_DIR)/$(TARGET_SUPPORT_STATIC)

.PHONY: clean
clean:
	@rm -fr *.o *.so *.a
