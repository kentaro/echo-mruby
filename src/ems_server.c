#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "ems_server.h"

ems_server *ems_server_new(int port)
{
  ems *core;
  ems_server *self;

  self = malloc(sizeof(ems_server));
  if (self == NULL) {
    return NULL;
  }

  core = ems_new();
  if (core == NULL) {
    free(self);
    return NULL;
  }

  self->core = core;
  self->port = port;
  self->sock.sin_family = AF_INET;
  self->sock.sin_port = (in_port_t)htons(port);
  self->sock.sin_addr.s_addr = htonl(INADDR_ANY);

  return self;
}

void ems_server_free(ems_server *self)
{
  ems_free(self->core);
  free(self);
}

void ems_server_run(ems_server *self)
{
  char err_msg[256];

  int len;
  char buf[256];

  int listener;
  int conn;

  struct sockaddr_storage client_addr;
  unsigned int address_size = sizeof(client_addr);

  if ((listener = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    strcpy(err_msg, "failed to create a socket");
    goto ERROR;
  }
  if (bind(listener, (struct sockaddr *)&self->sock, sizeof(self->sock)) < 0) {
    strcpy(err_msg, "failed to bind");
    goto ERROR;
  }
  if (listen(listener, 1) < 0) {
    strcpy(err_msg, "failed to listen");
    goto ERROR;
  }

  while(1) {
    conn = accept(listener, (struct sockaddr *)&client_addr, &address_size);

    if (conn < 0) {
      strcpy(err_msg, "failed to accept");
      goto ERROR;
    }

    while ((len = read(conn, buf, 256)) > 0) {
      write(conn, buf, len);
    }

    close(conn);
  }

ERROR:
  fprintf(stderr, "%s\n", err_msg);
  ems_server_free(self);
  exit(1);
}

int main()
{
  ems_server *server = ems_server_new(12345);
  if (server == NULL) {
    fprintf(stderr, "%s\n", "failed to initialize server");
    exit(1);
  }

  ems_server_run(server);
  exit(0);
}