// inspired by https://github.com/lunasorcery/pridecat
// vi: sw=2
// TODO: merge with pridecat-lite-trans-bi.c
/*
uses "Erase in Line" (EL) control code: ESC [ *n* K
clears part of the line, filling with the current background colour
n = 0 or omitted: clear from cursor to end
n = 1: clear from beginning to cursor
n = 2: clear whole line
*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#define x() if(l != 19) fputs("\033[39;49m\033[K", stdout)
#define rgb(r, g, b) fputs("\033[48;2;" #r ";" #g ";" #b "m\033[K", stdout)
static unsigned char l, p;
static void cat(FILE *f){
  int c = getc(f);
  if(c < 0) return;
  if(l == 19){
    fputs("\033[38;5;16m\033[48;2;91;206;250m\033[K", stdout); // black text
    l = 0;
    goto uhh;
  }
  do{
    if(p == '\n'){
      switch(l = (l + 1) % 10){
	case 0: case 4: rgb(91, 206, 250); break;
	case 1: case 3: rgb(245, 169, 184); break;
	case 2: fputs("\033[48;5;231m\033[K", stdout); break;
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
		rgb(0, 56, 168); break;
#ifndef DUPECOLORS
	case 6: case 9: fputs("\033[K", stdout);
#endif
    } }
uhh:
    putc(p = c, stdout);
  }while((c = getc(f)) >= 0);
}
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
	fprintf(stderr, "pridebg: Couldn't open \"%s\" for reading.\n", *v);
	return 1;
      }
      cat(f);
      fclose(f);
  } }
  x();
}
