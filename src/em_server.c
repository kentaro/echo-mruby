#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "em_server.h"

em_server *em_server_new(int port)
{
  em *core;
  em_server *self;

  self = malloc(sizeof(em_server));
  if (self == NULL) {
    return NULL;
  }

  core = em_new();
  if (core == NULL) {
    free(self);
    return NULL;
  }

  self->core = core;
  self->port = port;
  self->server.sin_family = AF_INET;
  self->server.sin_port = (in_port_t)htons(port);
  self->server.sin_addr.s_addr = htonl(INADDR_ANY);

  return self;
}

void em_server_free(em_server *self)
{
  em_free(self->core);
  free(self);
}

#define BUF_SIZE 255

void em_server_run(em_server *self)
{
  int len;
  char code[BUF_SIZE];

  char *res;
  char *err;

  int sock;
  int conn;
  struct sockaddr_storage client;
  unsigned int client_size = sizeof(client);

  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    err = "failed to create a socket";
    goto ERROR;
  }
  if (bind(sock, (struct sockaddr *)&self->server, sizeof(self->server)) < 0) {
    err = "failed to bind";
    goto ERROR;
  }
  if (listen(sock, 1) < 0) {
    err = "failed to listen";
    goto ERROR;
  }

  while(1) {
    if ((conn = accept(sock, (struct sockaddr *)&client, &client_size)) < 0) {
      err = "failed to accept";
      goto ERROR;
    }

    while (1) {
      memset(code, 0, sizeof(code));
      len = recv(conn, code, sizeof(code), 0);

      if (len < 0) {         // recv error
        perror("an error occurred\n");
        break;
      } else if (len == 0) { // connection closed
        perror("connection closed by peer\n");
        break;
      } else if (len > 0) {
        res = em_mrb_eval(self->core, code);

        if (res != NULL) {
          send(conn, res, strlen(res), 0);
          em_string_free(self->core, res);
        }
      }
    }

    close(conn);
  }

ERROR:
  fprintf(stderr, "%s\n", err);
  em_server_free(self);
  exit(1);
}

int main(int argc, char *argv[])
{
  int port;

  if (argc > 1) {
    port = atoi(argv[1]);
  }
  if (port == 0) {
    port = 12345;
  }

  em_server *server = em_server_new(port);
  if (server == NULL) {
    fprintf(stderr, "%s\n", "failed to initialize server");
    exit(1);
  }

  em_server_run(server);
  exit(0);
}