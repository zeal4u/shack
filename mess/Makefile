SRC_UNIT = $(wildcard *.cpp)
OBJ_UNIT = $(patsubst %.cpp,%.o,$(notdir ${SRC_UNIT}))
HEAD_UNIT = $(wildcard ${ROOT_DIR}/common/*.h)

UNIT_TEST = unit_test

all: $(UNIT_TEST)

$(OBJ_UNIT):%.o:%.cpp
	$(XX) $(CPP_FLAGS) -c $< -o $@

$(UNIT_TEST): $(UNIT_TEST).cpp $(HEAD_UNIT)
	$(XX) $(CPP_FLAGS) -lboost_unit_test_framework -l:libcommon.a -L$(LIBS_DIR) $(INCLUDE) $(UNIT_TEST).cpp -o $(UNIT_TEST)  

.PHONY: DYNAMIC
DYNAMIC:$(UNIT_TEST).cpp $(HEAD_UNIT)
	$(XX) $(CPP_FLAGS) -lboost_unit_test_framework -lcommon -L$(LIBS_DIR) $(INCLUDE) $(UNIT_TEST).cpp -o $(UNIT_TEST)

.PHONY: clean
clean :
	rm -fr *.o $(UNIT_TEST)
