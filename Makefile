CXX ?= clang
CC ?= clang

all: pridecat pridecat-lite-trans-bi pridebg-lite-trans-bi \
	pridehl-lite-trans-bi

pridecat-lite-trans-bi: pridecat-lite-trans-bi.c
	$(CC) pridecat-lite-trans-bi.c \
		-o pridecat-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridebg-lite-trans-bi: pridebg-lite-trans-bi.c
	$(CC) pridebg-lite-trans-bi.c \
		-o pridebg-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridehl-lite-trans-bi: pridecat-lite-trans-bi.c
	$(CC) pridecat-lite-trans-bi.c -DPRIDEHL \
		-o pridehl-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridecat: main.cpp
	$(CXX) main.cpp -o pridecat -std=c++11 -lstdc++ -Wall -Wextra -O3 -s

install: pridecat
	cp pridecat /usr/local/bin/pridecat

uninstall:
	rm -f /usr/local/bin/pridecat

clean:
	rm -f pridecat pride*-lite-trans-bi
