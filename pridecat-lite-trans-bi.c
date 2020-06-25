// inspired by https://github.com/lunasorcery/pridecat
// vi: sw=2
/*TODO:
merge pridecat, pridehl, pridebg into 1 program
note to self: see getargs.h (not a public file yet)

reimplement the printf of hexadecimal?
*/
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef PRIDEHL
#ifndef DUPECOLORS
#define DUPECOLORS
#endif
#define ESC "\033[4"
#define x() if(l != 19) fputs("\033[39;49m", stdout)
#else
#define ESC "\033[3"
#define x() if(l != 19) fputs(ESC "9m", stdout)
#endif
#define rgb(r, g, b) fputs(ESC "8;2;" #r ";" #g ";" #b "m", stdout)
static unsigned char l;
static bool e;
static void cat(FILE *f){
  int c;
#define C(x) (c != x)
  while((c = getc(f)) >= 0){
    if(!C('\n')){
#ifdef PRIDEHL
      if(l != 19) fputs(ESC "9m", stdout);
#endif
      e = false;
    }else if(!e && C(' ') && C('\t') && C('\r') && C('\f') && C('\v')){
#ifdef PRIDEHL
      if(l == 19){
	l = 0;
	fputs("\033[38;5;16m\033[48;2;91;206;250m", stdout); // black text
      }else
#endif
      switch(l = (l + 1) % 10){
	case 0: case 4: rgb(91, 206, 250); break;
	case 1: case 3: rgb(245, 169, 184); break;
	case 2: /*rgb(255, 255, 255);*/ fputs(ESC "8;5;231m", stdout); break;
	case 5:
#ifdef DUPECOLORS
	case 6:
#endif
	  rgb(214, 2, 112); break;
	case 7: rgb(155, 79, 150); break;
	case 8:
#ifdef DUPECOLORS
	case 9:
#endif
	  rgb(0, 56, 168);
      }
      e = true;
    }
    putc(c, stdout);
} }
static void abrt(int signo){ x(); exit(signo); }
int main(int c, char **v){
  l = 19;
  signal(SIGINT, abrt);
  if(c < 2) cat(stdin);
  else for(--c, ++v; c; --c, ++v){
    if(**v == '-' && !v[0][1]) cat(stdin);
    else{
      FILE *f = fopen(*v, "r");
      if(!f){
	x();
	fprintf(stderr, "pride"
#ifdef PRIDEHL
	    "hl"
#else
	    "cat"
#endif
	    ": Couldn't open \"%s\" for reading.\n", *v);
	return 1;
      }
      cat(f);
      fclose(f);
  } }
  x();
}
