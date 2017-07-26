#ifndef EM_SERVER_H
#define EM_SERVER_H

#include <sys/socket.h>
#include "em.h"

typedef struct em_server_t {
  em *core;
  int port;
  struct sockaddr_in server;
} em_server;

em_server *em_server_new(int port);
void em_server_free(em_server *self);
void em_server_run(em_server *self);

#endif // EM_SERVER_H