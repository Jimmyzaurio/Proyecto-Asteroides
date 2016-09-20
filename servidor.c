#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <X11/Xlib.h>

// Función que nos regresa el valor para asignar color a nuestra línea
int setColor(Display* dis, char* color_name){
  Colormap cmap = DefaultColormap(dis, 0);

  XColor color_cercano, color_verdadero;
  XAllocNamedColor(dis, cmap, color_name, &color_cercano, &color_verdadero);

  return(color_cercano.pixel);
}

int puerto = 7200;

#define NUEVO_ASTEROIDE coord[0] == -1

int main() {
  struct sockaddr_in server_addr, msg_to_client_addr;
  Display *disp = NULL;
  Window ventana;
  XColor color;

  int s = socket(AF_INET, SOCK_DGRAM, 0);

  bzero((char*)&server_addr, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(puerto);

  bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr));

  int clilen = sizeof(msg_to_client_addr);

  disp    = XOpenDisplay(NULL);
  ventana = XCreateSimpleWindow (disp, XDefaultRootWindow (disp), 100, 100, 800, 600, 1, 1,BlackPixel (disp, DefaultScreen(disp)));
  XMapWindow (disp, ventana);

  // Es necesario guardar el inicio del polígono para poder colocar la última línea del polígono
  int inix = 0, iniy = 0, prevx = 0, prevy = 0;
  while (1) {
    int coord[2];
    recvfrom(s, (char*)coord, sizeof(int) * 2, 0, (struct sockaddr*)&msg_to_client_addr, &clilen);

    // En caso de que sea el inicio de un nuevo asteroide, leemos la coordenada inicial
    // Es necesario mencionar que la última línea del último polígono se pondrá en éste momento
    if (NUEVO_ASTEROIDE) {
      XSetForeground (disp, XDefaultGC (disp, DefaultScreen(disp)), setColor(disp, "green"));
      XDrawLine (disp,ventana,XDefaultGC (disp, DefaultScreen(disp)), inix, iniy, prevx, prevy);
      recvfrom(s, (char*)coord, sizeof(int) * 2, 0, (struct sockaddr*)&msg_to_client_addr, &clilen);

      inix = prevx = coord[0];
      iniy = prevy = coord[1];
    } else {
      XSetForeground (disp, XDefaultGC (disp, DefaultScreen(disp)), setColor(disp, "green"));
      XDrawLine (disp,ventana,XDefaultGC (disp, DefaultScreen(disp)), prevx, prevy, coord[0], coord[1]);

      prevx = coord[0];
      prevy = coord[1];
    }
  }
}
