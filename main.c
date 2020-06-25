// vi: sw=2
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

static const uint32_t g_colorQueue[10] ={
  // info: https://en.wikipedia.org/wiki/Transgender_flags
  // colors: https://en.wikipedia.org/wiki/File:Transgender_Pride_flag.svg
  0x5BCEFA, 0xF5A9B8, 0xFFFFFF, 0xF5A9B8, 0x5BCEFA,

  // info: https://en.wikipedia.org/wiki/Bisexual_pride_flag
  // colors: https://en.wikipedia.org/wiki/File:Bisexual_Pride_Flag.svg
  0xD60270, 0xD60270, 0x9B4F96, 0x0038A8, 0x0038A8,
};

static unsigned int g_currentRow = 0;

static bool g_setBackgroundColor = false;
static bool g_blankLine = true;
static bool g_started = false;

static void nextColor(){
  const uint32_t color = g_colorQueue[g_currentRow];
  g_currentRow = (g_currentRow + 1) % 10;
  g_started = true;
  fprintf(stdout,
	  g_setBackgroundColor ? "\033[38;5;16m\033[48;2;%d;%d;%dm\033[2K"
				: "\033[38;2;%d;%d;%dm",
	  color >> 16, (color >> 8) & 255, color & 255);
}

static void resetColor(){
  if(g_started)
    fputs(g_setBackgroundColor ? "\033[39;49m" : "\033[39m", stdout);
}

static void abrt(int signo){ resetColor(); exit(signo); }

static void catFile(FILE *f){
  int c, prev = '\n';
  while((c = getc(f)) >= 0){
    if(c == '\n'){
      if(prev == '\n' && g_setBackgroundColor) nextColor();
      else resetColor();
      g_blankLine = true;
    }else if(g_blankLine && !isspace(c)){
      nextColor();
      g_blankLine = false;
    }
    putc(prev = c, stdout);
  }
}

static void help(void){
  fputs("pridecat, lucy's personal trans bi version\n"
	" -b for background color\n",
	stderr);
  exit(1);
}

int main(int argc, char **argv){
  signal(SIGINT, abrt);

#if defined(_WIN32)
  HANDLE const hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if(hOut == INVALID_HANDLE_VALUE) return 1;
  DWORD dwMode = 0;
  if(!GetConsoleMode(hOut, &dwMode)) return 1;
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if(!SetConsoleMode(hOut, dwMode)) return 1;
#endif

  while(--argc && **++argv == '-' && argv[0][1]){
    if(argv[0][2]) help();
    if(argv[0][1] == 'b'){
      if(g_setBackgroundColor) help();
      g_setBackgroundColor = true;
    }else if(argv[0][1] == '-'){
      argc--; argv++; break;
    }else help();
  }

  if(!argc) catFile(stdin);
  else for(; argc; argv++, argc--){
    if(**argv == '-' && !argv[0][1]){
      catFile(stdin);
    }else{
      FILE *f = fopen(*argv, "rb");
      if(!f){
	fprintf(stderr, "pridecat: Could not open %s for reading.\n", *argv);
	return 1;
      }
      catFile(f);
      fclose(f);
    }
  }

  resetColor();
}
