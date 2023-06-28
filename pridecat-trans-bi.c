// inspired by https://github.com/lunasorcery/pridecat
// vi: sw=2
/*TODO:
merge pridecat, pridehl, pridebg into 1 program
note to self: see getargs.h (not a public file yet)

reimplement the printf of hexadecimal?
*/
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#ifdef PRIDEHL
#define ESC "\33[4"
#define x() \
  if(l != 19) fputs("\33[39;49m", stdout)
#else
#define ESC "\33[3"
#define x() \
  if(l != 19) fputs(ESC "9m", stdout)
#endif
#define rgb(r, g, b) fputs(ESC "8;2;" #r ";" #g ";" #b "m", stdout)
static unsigned l;
static bool e, q;
static void cat(FILE *f) {
  int c;
  while(!q && (c = getc(f)) >= 0) {
    if(c == '\n') {
#ifdef PRIDEHL
      if(l != 19) fputs(ESC "9m", stdout);
#endif
      e = false;
    } else if(!e && !isspace(c)) {
#ifdef PRIDEHL
      if(l == 19) {
	l = 0;
	fputs("\33[38;5;16m\33[48;2;91;206;250m", stdout); // black text
      } else
#endif
	switch(l = (l + 1) % 10) {
	  case 0:
	  case 4: rgb(91, 206, 250); break;
	  case 1:
	  case 3: rgb(245, 169, 184); break;
	  case 2: /*rgb(255, 255, 255);*/ fputs(ESC "8;5;231m", stdout); break;
	  case 5:
	  case 6: rgb(214, 2, 112); break;
	  case 7: rgb(155, 79, 150); break;
	  case 8:
	  case 9: rgb(0, 56, 168);
	}
      e = true;
    }
    putc(c, stdout);
  }
}
static void abrt(int x) {
  (void)x;
  q = 1; // TODO: more/different q checks?
}
int main(int c, char **v) {
  l = 19;
  signal(SIGINT, abrt);
  if(c < 2) cat(stdin);
  else
    for(--c, ++v; !q && c; --c, ++v)
      if(**v == '-' && !v[0][1]) cat(stdin);
      else {
	FILE *f = fopen(*v, "r");
	if(!f) {
	  x();
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
  x();
}
