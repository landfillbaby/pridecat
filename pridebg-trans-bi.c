// inspired by https://github.com/lunasorcery/pridecat
// vi: sw=2
// TODO: merge with pridecat-lite-trans-bi.c
/*
uses "Erase in Line" (EL) control code: ESC [ *n* K
clears part of the line, filling with the current background color
n = 0 or omitted: clear from cursor to end
n = 1: clear from beginning to cursor
n = 2: clear whole line
*/
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#define x \
  if(l >= 0) fputs("\33[39;49m\33[K", stdout) // reset colors
#define rgb(r, g, b) fputs("\33[48;2;" #r ";" #g ";" #b "m\33[K", stdout)
static int l, p;
static volatile sig_atomic_t q;
static void cat(FILE *f) {
  int c = getc(f);
  if(c < 0) return;
  if(l < 0) { // black on trans blue
    l = 0;
    fputs("\33[38;5;16m\33[48;2;91;206;250m\33[K", stdout);
    goto uhh;
  }
  do {
    if(p == '\n') switch(l = (l + 1) % 10) {
	case 0:
	case 4: rgb(91, 206, 250); break; // trans blue
	case 1:
	case 3: rgb(245, 169, 184); break; // trans pink
	case 2: fputs("\33[48;5;231m\33[K", stdout); break; // white
	case 5:
	case 6: rgb(214, 2, 112); break; // bi pink
	case 7: rgb(155, 79, 150); break; // bi purple
	case 8:
	case 9: rgb(0, 56, 168); break; // bi blue
	default: assert(0); break;
      }
  uhh:
    putc(p = c, stdout);
  } while(!q && (c = getc(f)) >= 0);
}
static void abrt(int z) {
  (void)z;
  q = 1; // TODO: more/different q checks?
}
int main(int c, char **v) {
  l = -1;
  signal(SIGINT, abrt);
  if(c < 2) cat(stdin);
  else
    for(--c, ++v; !q && c; --c, ++v)
      if(**v == '-' && !v[0][1]) cat(stdin);
      else {
	FILE *f = fopen(*v, "r");
	if(!f) {
	  x;
	  fprintf(stderr, "pridebg: Couldn't open \"%s\" for reading.\n", *v);
	  return 1;
	}
	cat(f);
	fclose(f);
      }
  x;
}
