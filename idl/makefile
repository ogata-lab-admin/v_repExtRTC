RTM_ROOT=/usr/local/include/openrtm-1.1/

SRC=Simulator.idl
OUTPUT=Simulator.hh SimulatorDynSK.cc SimulatorSK.cc SimulatorSkel.* SimulatorStub.*

all:
	omniidl -bcxx -Wba -nf -I${RTM_ROOT}/rtm/idl ${SRC}
	rtm-skelwrapper --skel-suffix=Skel --stub-suffix=Stub --idl-file=${SRC}
	mv *.cpp ../src/
	mv *.cc ../src/
	mv *.hh ../include/
	mv *.h ../include/
clean:
	rm ${OUTPUT}