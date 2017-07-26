#ifndef EMS_SERVER_H
#define EMS_SERVER_H

#include <sys/socket.h>
#include "ems.h"

typedef struct ems_server_t {
  ems *core;
  int port;
  struct sockaddr_in sock;
} ems_server;

ems_server *ems_server_new(int port);
void ems_server_free(ems_server *self);
void ems_server_run(ems_server *self);

#endif // EMS_SERVER_H