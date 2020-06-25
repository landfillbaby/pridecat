CXX ?= clang
CC ?= clang

pridecat: main.cpp
	$(CXX) main.cpp -o pridecat -std=c++11 -lstdc++ -Wall -Wextra -O3 -s

install: pridecat
	cp pridecat /usr/local/bin/pridecat

uninstall:
	rm -fv /usr/local/bin/pridecat

clean:
	rm -fv pridecat pride*-trans-bi

all: pridecat pridecat-trans-bi pridecat-lite-trans-bi \
	pridebg-lite-trans-bi pridehl-lite-trans-bi

pridecat-trans-bi: main-trans-bi.c
	$(CC) main-trans-bi.c \
		-o pridecat-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridecat-lite-trans-bi: pridecat-lite-trans-bi.c
	$(CC) pridecat-lite-trans-bi.c \
		-o pridecat-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridebg-lite-trans-bi: pridebg-lite-trans-bi.c
	$(CC) pridebg-lite-trans-bi.c \
		-o pridebg-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s

pridehl-lite-trans-bi: pridecat-lite-trans-bi.c
	$(CC) pridecat-lite-trans-bi.c -DPRIDEHL \
		-o pridehl-lite-trans-bi -std=c11 -Wall -Wextra -O3 -s
