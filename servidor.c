#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <math.h>

unsigned long ObtieneColor(Display* dis, char* color_name){
   Colormap cmap;
   XColor color_cercano, color_verdadero;

   cmap = DefaultColormap(dis, 0);
   XAllocNamedColor(dis, cmap, color_name, &color_cercano, &color_verdadero);

	return(color_cercano.pixel);
}

int puerto = 7200;

#define NUEVO_ASTEROIDE num[0] == -1

int main() {
	int ax = 0, ay = 0, num[2];
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

	disp = XOpenDisplay(NULL);
	ventana = XCreateSimpleWindow (disp, XDefaultRootWindow (disp), 100, 100, 800, 600, 1, 1,BlackPixel (disp, DefaultScreen(disp)));
	XMapWindow (disp, ventana);

	int x = 0, y = 0;
	int prevx = 0, prevy = 0;
	while (1) {
		int num[2];
		recvfrom(s, (char*)num, sizeof(int) * 2, 0, (struct sockaddr*)&msg_to_client_addr, &clilen);

		if (NUEVO_ASTEROIDE) {
			XSetForeground (disp, XDefaultGC (disp, DefaultScreen(disp)), BlackPixel(disp, 0)^ObtieneColor(disp, "white"));
			XDrawLine (disp,ventana,XDefaultGC (disp, DefaultScreen(disp)), x, y, ax, ay);
			recvfrom(s, (char*)num, sizeof(int) * 2, 0, (struct sockaddr*)&msg_to_client_addr, &clilen);
			x = ax = num[0];
			y = ay = num[1];
			//sleep(1);
		}else{
			XSetForeground (disp, XDefaultGC (disp, DefaultScreen(disp)), BlackPixel(disp, 0)^ObtieneColor(disp, "white"));
			XDrawLine (disp,ventana,XDefaultGC (disp, DefaultScreen(disp)), ax, ay, num[0], num[1]);

			ax = num[0];
			ay = num[1];
		}
	}
}
