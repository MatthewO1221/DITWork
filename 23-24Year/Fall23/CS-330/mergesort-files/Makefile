PRG=gnu.exe
GCC=g++
GCCFLAGS=-O2 -Wall -Wextra -std=c++11 -pedantic -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder

MSC=cl
MSCFLAGS=/EHa /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE /D"_SECURE_SCL 0" /O2i /GL

VALGRIND_OPTIONS=-q --leak-check=full
DIFFLAGS=--strip-trailing-cr -y --suppress-common-lines 

OBJECTS0=mergesort.cpp
DRIVER0=driver.cpp

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS)
msc0:
	$(MSC) /Fe$@.exe           $(DRIVER0) $(OBJECTS0) $(MSCFLAGS)
0:
	@echo "running test$@"
	watchdog 1000 ./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFFLAGS) > difference$@
mem0:
	watchdog 8000 valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@
clean:
	rm -f *gcno *gcda *gcov *.exe *.o *.obj *.tds difference* studentout*
