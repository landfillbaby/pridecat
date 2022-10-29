CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -O3 -s

all: pridecat-trans-bi pridebg-trans-bi pridehl-trans-bi
clean:
	rm -fv pridecat-trans-bi pridebg-trans-bi pridehl-trans-bi
pridecat-trans-bi: pridecat-trans-bi.c
pridebg-trans-bi: pridebg-trans-bi.c
pridehl-trans-bi: pridecat-trans-bi.c
	$(LINK.c) -DPRIDEHL $^ $(LOADLIBES) $(LDLIBS) -o $@
