VREP_DIR=../../../
VREP_PROGRAMMING_DIR=${VREP_DIR}/programming/

CFLAGS = -I../include -I${VREP_PROGRAMMING_DIR}include -Wall -fPIC `rtm-config --cflags` 
LDFLAGS = -lpthread -ldl `rtm-config --libs`

OBJS = v_repExtRTC.o ${VREP_PROGRAMMING_DIR}common/v_repLib.o VREPRTC.o SimulatorSVC_impl.o SimulatorStub.o RTCHelper.o RobotRTC.o Tasks.o RobotRTCContainer.o RangeRTC.o CameraRTC.o

OS = $(shell uname -s)
ECHO=@

ifeq ($(OS), Linux)
	CFLAGS += -D__linux
	OPTION = -shared
	EXT = so
	TARGET = ../lib/libv_repExtRTC.$(EXT)
else
	CFLAGS += -D__APPLE__
	OPTION = -dynamiclib -current_version 1.0
	EXT = dylib
	TARGET = ../lib/libv_repExtRTC.$(EXT)
	BINDIR = ${VREP_DIR}vrep.app/Contents/MacOS/
endif

all: v_repExtRTCLib

v_repExtRTCLib: $(OBJS)
		@mkdir -p ../lib
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
		$(ECHO)rm -rf $(OBJS) *~ ../*~ ../include/*~ $(TARGET)

install:
		@echo "Installing $(TARGET) to $(BINDIR)"
		$(ECHO)cp $(TARGET) $(BINDIR)


