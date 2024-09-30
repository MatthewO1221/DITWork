PRG=gnu.exe
#PRG=gcc0.exe # remove when copying to pontus

VALGRIND_OPTIONS=-q --leak-check=full
DIFFLAGS=--strip-trailing-cr -y --suppress-common-lines

gcc0:
	g++ -g -O2 -std=c++11 driver.cpp replace.cpp -o $(PRG) 
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16:
	echo "running test$@"
	@echo "should run in less than 200 ms"
	./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@ 2>&1
17:
	echo "running test$@"
	@echo "should run in less than 5000 ms"
	./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@ 2>&1
18 19:
	echo "running test$@"
	@echo "should run in less than 50 ms"
	./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@ 2>&1
mem15 mem16:
	echo "running memory test $@"
	@echo "should run in less than 2000 ms"
	valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@
	@echo "lines after this are memory errors"; cat difference$@
clean:
	rm -f *.exe *.o student*
