BASE_DIR= ${HOME}/develop/server

DEBUG_LIB_DIR = $(BASE_DIR)/lib/Debug
RELEASE_LIB_DIR = $(BASE_DIR)/lib/Release

ifeq ($(mode),d)
	BIN_SUFFIX = _dbg
	LIB_DIR = $(DEBUG_LIB_DIR)
	BIN_DIR = $(BASE_DIR)/bin/Debug
	CPPFLAGS= -g -fPIC -Wall -DDEBUG $(INC) -Wno-invalid-offsetof -std=c++0x
	LDFLAGS = -g -fPIC -L$(LIB_DIR) -lpthread -lnetevent -llogger -lcommon -lframe -ltinyxml -lmemcached
	DEBUG_TARGET = $(BIN_DIR)/cacheproxy$(BIN_SUFFIX)
	TARGET	= $(DEBUG_TARGET)
else
	LIB_DIR = $(RELEASE_LIB_DIR)
	BIN_DIR = $(BASE_DIR)/bin/Release
	CPPFLAGS= -g -fPIC -Wall  $(INC) -Wno-invalid-offsetof -std=c++0x
	LDFLAGS = -g -fPIC -L$(LIB_DIR) -lpthread -lnetevent -llogger -lcommon -lframe -ltinyxml -lmemcached
	RELEASE_TARGET = $(BIN_DIR)/cacheproxy$(BIN_SUFFIX)
	TARGET	= $(RELEASE_TARGET)
endif

CACHEPROXY_DIR = $(BASE_DIR)/cacheproxy

SRC = $(wildcard *.cpp)
OBJ_DIR	= $(CACHEPROXY_DIR)/.objs
CACHEPROXY_OBJS = $(addprefix $(OBJ_DIR)/, $(subst .cpp,.o,$(SRC)))
OBJS = $(wildcard $(OBJ_DIR)/*.o)

COMMON_INCLUDE_DIR = $(BASE_DIR)/common
NETEVENT_INCLUDE_DIR = $(BASE_DIR)/netevent
LOGGER_INCLUDE_DIR = $(BASE_DIR)/logger
FRAME_INCLUDE_DIR = $(BASE_DIR)/frame
JSONCPP_INCLUDE_DIR = $(BASE_DIR)/jsoncpp
INC = -I$(COMMON_INCLUDE_DIR) -I$(NETEVENT_INCLUDE_DIR) -I$(LOGGER_INCLUDE_DIR) -I$(FRAME_INCLUDE_DIR)

all : $(TARGET)

$(TARGET) : $(CACHEPROXY_OBJS)
	$(CXX)  -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
