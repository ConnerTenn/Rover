
GPP=g++ -std=c++17 -Wall -lncurses -lcurl -lpthread $F $T
D=Build


all: begin dir $D/run.exe

begin:
	echo -e "\e[1;36m == Starting Build ==\e[0m"
	
dir: begin
	mkdir -p $D
	
	
$D/run.exe: dir $D/main.o $D/LMS1xx.o 
	$(GPP) $D/main.o $D/LMS1xx.o -o $D/run.exe
	echo -e "\e[1;36m == Done Build ==\e[0m\n"

$D/main.o: dir main.cpp LMS1xx.h
	$(GPP) main.cpp -c -o $D/main.o
	
$D/LMS1xx.o: dir LMS1xx.cpp LMS1xx.h
	$(GPP) LMS1xx.cpp -c -o $D/LMS1xx.o


run: all
	$D/run.exe
	
clean:
	rm $D/* -f
	
force: clean all

debug: T=$F -D DEBUG -ggdb
debug: force

	