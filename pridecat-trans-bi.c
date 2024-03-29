// inspired by https://github.com/lunasorcery/pridecat
// vi: sw=2
/*TODO:
merge pridecat, pridehl, pridebg into 1 program
note to self: see getargs.h (not a public file yet)

reimplement the printf of hexadecimal?
*/
#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#ifdef PRIDEHL
#define ESC "\33[4"
#define x \
  if(l >= 0) fputs("\33[39;49m", stdout) // reset colors
#else
#define ESC "\33[3"
#define x \
  if(l >= 0) fputs(ESC "9m", stdout) // reset colors
#endif
#define rgb(r, g, b) fputs(ESC "8;2;" #r ";" #g ";" #b "m", stdout)
static int l;
static bool e;
static volatile sig_atomic_t q;
static void cat(FILE *f) {
  int c;
  while(!q && (c = getc(f)) >= 0) {
    if(c == '\n') {
#ifdef PRIDEHL
      if(l >= 0) fputs(ESC "9m", stdout); // reset background
#endif
      e = 0;
    } else if(!e && !isspace(c)) {
#ifdef PRIDEHL
      if(l < 0) { // black on trans blue
	l = 0;
	fputs("\33[38;5;16m\33[48;2;91;206;250m", stdout);
      } else
#endif
	switch(l = (l + 1) % 10) {
	  case 0:
	  case 4: rgb(91, 206, 250); break; // trans blue
	  case 1:
	  case 3: rgb(245, 169, 184); break; // trans pink
	  case 2: fputs(ESC "8;5;231m", stdout); break; // white
	  case 5:
	  case 6: rgb(214, 2, 112); break; // bi pink
	  case 7: rgb(155, 79, 150); break; // bi purple
	  case 8:
	  case 9: rgb(0, 56, 168); break; // bi blue
	  default: assert(0); break;
	}
      e = 1;
    }
    putc(c, stdout);
  }
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
	  fprintf(stderr,
#ifdef PRIDEHL
	      "pridehl"
#else
	      "pridecat"
#endif
	      ": Couldn't open \"%s\" for reading.\n",
	      *v);
	  return 1;
	}
	cat(f);
	fclose(f);
      }
  x;
}
