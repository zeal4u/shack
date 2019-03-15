SRC_UNIT = $(wildcard *.cpp)
OBJ_UNIT = $(patsubst %.cpp,%.o,$(notdir ${SRC_UNIT}))

MICROSOFT_TARGET= microsoft_intern

all: $(MICROSOFT_TARGET)

$(OBJ_UNIT):%.o:%.cpp
	$(XX) $(CPP_FLAGS) -c $< -o $@

$(MICROSOFT_TARGET): $(MICROSOFT_TARGET).cpp
	$(XX) $(CPP_FLAGS) $(INCLUDE) $(MICROSOFT_TARGET).cpp -o $(MICROSOFT_TARGET) -l:libcommon.a -L$(LIBS_DIR) 

.PHONY: DYNAMIC
DYNAMIC:$(MICROSOFT_TARGET).cpp
	$(XX) $(CPP_FLAGS) -lcommon -L$(LIBS_DIR) $(INCLUDE) $(MICROSOFT_TARGET).cpp -o $(MICROSOFT_TARGET)

.PHONY: clean
clean :
	rm -f *.o $(TARGET_BIN)
