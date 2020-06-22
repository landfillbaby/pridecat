CXX ?= clang
CC ?= clang

all: pridecat pridecat-lite-trans-bi pridebg-lite-trans-bi \
	pridebgfull-lite-trans-bi

pridecat-lite-trans-bi: pridecat-lite-trans-bi.c
	$(CC) pridecat-lite-trans-bi.c \
		-o pridecat-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridebg-lite-trans-bi: pridecat-lite-trans-bi.c
	$(CC) pridecat-lite-trans-bi.c -DPRIDEBG \
		-o pridebg-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridebgfull-lite-trans-bi: pridebgfull-lite-trans-bi.c
	$(CC) pridebgfull-lite-trans-bi.c \
		-o pridebgfull-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridecat: main.cpp
	$(CXX) main.cpp -o pridecat -std=c++11 -lstdc++ -Wall -Wextra -O3 -s

install: pridecat
	cp pridecat /usr/local/bin/pridecat

uninstall:
	rm -f /usr/local/bin/pridecat

clean:
	rm -f pridecat
