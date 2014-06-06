all:
	(cd src; make)

clean:
	(rm -rf *~ *.o)
	(cd src; make clean)

install:
	(cd src; make install)