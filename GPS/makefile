
GPP=g++ -std=c++17 -Wall -lncurses -lcurl -lpthread $F $T
D=Build


all: begin dir $D/run.exe

begin:
	echo -e "\e[1;36m == Starting Build ==\e[0m"
	
dir: begin
	mkdir -p $D
	
	
$D/run.exe: dir $D/main.o $D/Adafruit_GPS.o
	$(GPP) $D/main.o $D/Adafruit_GPS.o -o $D/run.exe
	echo -e "\e[1;36m == Done Build ==\e[0m\n"

$D/main.o: dir main.cpp Adafruit_GPS.h
	$(GPP) main.cpp -c -o $D/main.o
	
$D/Adafruit_GPS.o: dir Adafruit_GPS.cpp Adafruit_GPS.h
	$(GPP) Adafruit_GPS.cpp -c -o $D/Adafruit_GPS.o


run: all
	$D/run.exe
	
clean:
	rm $D/* -f
	
force: clean all

debug: T=$F -D DEBUG -ggdb
debug: force

	