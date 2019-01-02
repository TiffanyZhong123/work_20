#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
    int y;
    while(1) {
      printf("Server creating wkp...\n");
      mkfifo("wkp", 0644);

      printf("Server receiving client's message...\n");
      char arr[HANDSHAKE_BUFFER_SIZE];
      int wkp1 = open("wkp", O_RDONLY);
      if (read(wkp1, arr, HANDSHAKE_BUFFER_SIZE)) {
        printf("Client message: %s\n", arr);
        y = fork();
      }
      if (y) {
        remove("wkp");
        printf("Server removed wkp...\n");
      }
      else {
        printf("Subserver sending intial message...\n ");
        *to_client = open("ppipe", O_WRONLY);
        write(*to_client, ACK, sizeof(ACK));

        char m[BUFFER_SIZE];
        read(wkp1, m, BUFFER_SIZE);
        printf("Server received client message...\n");
        printf("Client message: %s\n", m);
        return wkp1;
      }
    }
  }

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char arr2[HANDSHAKE_BUFFER_SIZE];
  mkfifo("pipe", 0644);

  printf("Connecting to server...\n");
  *to_server = open("pipey",O_WRONLY);

  write(*to_server,"pipe",HANDSHAKE_BUFFER_SIZE);
  printf("Sending name of client FIFO...\n");

  int x = open("pipe",O_RDONLY);
  read(x,arr2,HANDSHAKE_BUFFER_SIZE);
  printf("Message from server: %s\n", arr2);

  remove("pipe");
  printf("Pipe is removed!\n");

  write(*to_server,ACK,sizeof(ACK));
  printf("Client sent message to server.\n");
  return x;
}
