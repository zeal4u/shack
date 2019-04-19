SRC_UNIT = $(wildcard *.cpp)
OBJ_UNIT = $(patsubst %.cpp,%.o,$(notdir ${SRC_UNIT}))

TARGET_BIN = test_main
LIBS= -lpthread -lboost_thread

all:$(TARGET_BIN)

$(OBJ_UNIT):%.o:%.cpp
	$(XX) $(CPP_FLAGS) $(INCLUDE) -c $< -o $@

$(TARGET_BIN): $(OBJ_UNIT)
	$(XX) $(OBJ_UNIT) -o $(TARGET_BIN) $(LIBS) -l:libcommon.a -L$(LIBS_DIR) 

.PHONY: DYNAMIC
DYNAMIC:$(OBJ_UNIT)
	$(XX) $(OBJ_UNIT) -o $(TARGET_BIN) $(LIBS) -lcommon -L$(LIBS_DIR)

.PHONY: clean
clean :
	rm -f *.o $(TARGET_BIN)
