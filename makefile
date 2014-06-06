CFLAGS = -Iinclude -I../include -Wall -fPIC `rtm-config --cflags` 
LDFLAGS = -lpthread -ldl `rtm-config --libs`

OBJS = src/v_repExtRTC.o ../common/v_repLib.o src/VREPRTC.o src/SimulatorSVC_impl.o src/SimulatorStub.o src/RTCHelper.o src/RobotRTC.o src/Tasks.o src/RobotRTCContainer.o

OS = $(shell uname -s)
ECHO=@

ifeq ($(OS), Linux)
	CFLAGS += -D__linux
	OPTION = -shared
	EXT = so
	TARGET = lib/libv_repExtRTC.$(EXT)
else
	CFLAGS += -D__APPLE__
	OPTION = -dynamiclib -current_version 1.0
	EXT = dylib
	TARGET = lib/libv_repExtRTC.$(EXT)
endif

BINDIR = ../../vrep.app/Contents/MacOS/
all: v_repExtRTCLib


v_repExtRTCLib: $(OBJS)
		@mkdir -p lib
		@echo "Linking $(OBJS) to $(TARGET)"
		$(ECHO)$(CXX) $(CFLAGS) $(OBJS) $(OPTION) -o $(TARGET) $(LDFLAGS)

%.o: %.c
		@echo "Compiling $< to $@"
		$(ECHO)$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cpp
		@echo "Compiling $< to $@"
		$(ECHO)$(CXX) $(CFLAGS) -c $< -o $@

clean:
		@echo "Cleaning $(OBJS) $(TARGET)"
		$(ECHO)rm -rf $(OBJS) $(TARGET) *~ src/*~ include/*~

install:
		@echo "Installing $(TARGET) to $(BINDIR)"
		$(ECHO)cp $(TARGET) $(BINDIR)