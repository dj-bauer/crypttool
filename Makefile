.PHONY: clean debug

TARGET ?= dj-crypt
SRC_DIRS ?= .
CC ?= g++

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp")
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)
LDLIBS ?= -lGL -lglfw -lGLEW #-fsanitize=address -fsanitize=undefined

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
WARNING ?= -Wall

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP $(WARNING) -m64 -O3

$(TARGET): $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

full: clean $(TARGET)

debug:
	echo -e "$(SRCS)\n$(OBJS)\n$(DEPS)\n$(LDLIBS)"

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)

