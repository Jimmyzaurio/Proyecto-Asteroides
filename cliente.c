#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define ANGULO (rand() % 17) + 30
#define RADIO ((rand()) % 17) + 28
#define THETA_RADIANES (((double)theta)/180.0)

int puerto = 7200;

int main() {
  srand(time(NULL));
  struct sockaddr_in msg_to_server_addr, client_addr;

  int s = socket(AF_INET, SOCK_DGRAM, 0);

  bzero((char *)&msg_to_server_addr, sizeof(msg_to_server_addr));
  msg_to_server_addr.sin_family      = AF_INET;
  msg_to_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  msg_to_server_addr.sin_port        = htons(puerto);

  bzero((char*)&client_addr, sizeof(client_addr));
  client_addr.sin_family      = AF_INET;
  client_addr.sin_addr.s_addr = INADDR_ANY;
  client_addr.sin_port        = htons(1234);

  bind(s, (struct sockaddr *)&client_addr, sizeof(client_addr));

  while (1) {
    // Se genera un offset para que cada asteroide tenga una ubicación diferente
    int off_x = rand() % 800;
    int off_y = rand() % 600;
    int coord[2];
    
    coord[0] = -1;
    // Enviamos un valor bandera, indicando que se mandarán las coordenadas del asteroide
    sendto(s, (char *)coord, 2 * sizeof(int), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));

    int inix, iniy, ultx, ulty, theta = 0;
    // Utilizamos un random para determinar el incremento del ángulo que se hará
    // dicho incremento estará entre 30 y 46 grados. 
    for (; theta <= 360; theta += ANGULO) {
      // Las funciones sin y cos reciben como parámetros los radianes a calcular
      coord[0] = RADIO * cos(M_PI*THETA_RADIANES) + off_x;
      coord[1] = RADIO * sin(M_PI*THETA_RADIANES) + off_y;
      
      // Enviamos las coordenadas
      sendto(s, (char *)coord, 2 * sizeof(int), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));
    }
    // Esperamos un segundo para enviar el siguiente asteroide
    sleep(1);
  }
  return 0;
}
