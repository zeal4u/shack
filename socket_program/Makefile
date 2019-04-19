CLIENT_SRC_UNIT = $(wildcard *client.cpp)
CLIENT_OBJ_UNIT = $(patsubst %.cpp,%.o,$(notdir ${CLIENT_SRC_UNIT}))

SERVER_SRC_UNIT = $(wildcard *server.cpp)
SERVER_OBJ_UNIT = $(patsubst %.cpp,%.o,$(notdir ${SERVER_SRC_UNIT}))

CLIENT_BIN = client
SERVER_BIN = server

all:$(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_OBJ_UNIT):%client.o:%client.cpp
	$(XX) $(CPP_FLAGS) -c $< -o $@

$(SERVER_OBJ_UNIT):%server.o:%server.cpp
	$(XX) $(CPP_FLAGS) -c $< -o $@

$(CLIENT_BIN): $(CLIENT_OBJ_UNIT) 
	$(XX) $(CLIENT_OBJ_UNIT) -o $(CLIENT_BIN) -l:libcommon.a -L$(LIBS_DIR) 

$(SERVER_BIN): $(SERVER_OBJ_UNIT) 
	$(XX) $(SERVER_OBJ_UNIT) -o $(SERVER_BIN) -l:libcommon.a -L$(LIBS_DIR) 

.PHONY: DYNAMIC
DYNAMIC:$(CLIENT_OBJ_UNIT) $(SERVER_OBJ_UNIT)
	$(XX) $(CLIENT_OBJ_UNIT) -o $(CLIENT_BIN) -l:libcommon.a -L$(LIBS_DIR) 
	$(XX) $(SERVER_OBJ_UNIT) -o $(SERVER_BIN) -l:libcommon.a -L$(LIBS_DIR) 

.PHONY: clean
clean :
	rm -f *.o $(CLIENT_BIN) $(SERVER_BIN)
