CXX ?= clang
CC ?= clang

all: pridecat pridecat-lite-trans-bi

pridecat-lite-trans-bi: pridecat-lite-trans-bi.c
	$(CC) pridecat-lite-trans-bi.c \
		-o pridecat-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridecat: main.cpp
	$(CXX) main.cpp -o pridecat -std=c++11 -lstdc++ -Wall -Wextra -O3 -s

install: pridecat
	cp pridecat /usr/local/bin/pridecat

uninstall:
	rm -f /usr/local/bin/pridecat

clean:
	rm -f pridecat
