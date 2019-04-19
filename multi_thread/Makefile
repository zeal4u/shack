SRC_UNIT = $(wildcard *.cpp)
OBJ_UNIT = $(patsubst %.cpp,%.o,$(notdir ${SRC_UNIT}))

TARGET_BIN = multi_main

all:$(TARGET_BIN)

$(OBJ_UNIT):%.o:%.cpp
	$(XX) $(CPP_FLAGS) -c $< -o $@ $(INCLUDE)

$(TARGET_BIN): $(OBJ_UNIT)
	$(XX) $(OBJ_UNIT) -o $(TARGET_BIN) $(INCLUDE) -lpthread -l:libcommon.a -L$(LIBS_DIR) 

.PHONY: DYNAMIC
DYNAMIC:$(OBJ_UNIT)
	$(XX) $(OBJ_UNIT) -o $(TARGET_BIN) $(INCLUDE) -lpthread -lcommon -L$(LIBS_DIR) 

.PHONY: clean
clean :
	rm -f *.o $(TARGET_BIN)
