#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define ANGULO (rand() % 22) + 30
#define RADIO ((rand()) % 30) + 28
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
    int off_x = rand()%800;
    int off_y = rand()%600;
    int coord[2];
    
    coord[0] = -1;
    sendto(s, (char *)coord, 2 * sizeof(int), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));

    int inix, iniy, ultx, ulty, theta = 0;
    for (; theta <= 360; theta += ANGULO) {
      coord[0] = RADIO * cos(M_PI*THETA_RADIANES) + off_x;
      coord[1] = RADIO * sin(M_PI*THETA_RADIANES) + off_y;
      
      if (theta == 0)
        inix = coord[0], iniy = coord[1];
      else if (theta == 359)
        ultx = coord[0], ulty = coord[1];

      sendto(s, (char *)coord, 2 * sizeof(int), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));
    }
    sleep(1);
  }
  return 0;
}
