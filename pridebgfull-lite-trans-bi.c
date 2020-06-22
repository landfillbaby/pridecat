// inspired by https://github.com/lunasorcery/pridecat
// vi: sw=2
/*TODO:
maybe add an argument for background color and black text
single boolean replacing the 38 and 39 with 48 and 49
note to self: see getargs.h (not a public file yet)

reimplement the printf of hexadecimal?
*/
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define rgb(r, g, b) fputs("\033[48;2;" #r ";" #g ";" #b "m\033[2K", stdout)
#define x() if(l != 19) fputs("\033[39;49m\033[K", stdout)
static unsigned char l;
static bool e;
static void cat(FILE *f){
  int c;
  while((c = getc(f)) >= 0){
    if(c == '\n') e = false;
    else if(!e){
      if(l == 19) fputs("\033[38;5;16m", stdout); // black text
      switch(l = (l + 1) % 10){
	case 0: case 4: rgb(91, 206, 250); break;
	case 1: case 3: rgb(245, 169, 184); break;
	case 2: fputs("\033[48;5;231m\033[2K", stdout); break;
	case 5: rgb(214, 2, 112); break;
	case 7: rgb(155, 79, 150); break;
	case 8: rgb(0, 56, 168); break;
	case 6: case 9: fputs("\033[2K", stdout);
      }
      e = true;
    }
    putc(c, stdout);
  }
}
static void abrt(int signo){ x(); exit(signo); }
int main(int c, char **v){
  l = 19;
  signal(SIGINT, abrt);
  if(c < 2) cat(stdin);
  else for(--c, ++v; c; --c, ++v){
    if(**v == 45 && !v[0][1]) cat(stdin);
    else{
      FILE *f = fopen(*v, "r");
      if(!f){
	x();
	fprintf(stderr, "pridecat: Couldn't open \"%s\" for reading.\n", *v);
	return 1;
      }
      cat(f);
      fclose(f);
    }
  }
  x();
}
