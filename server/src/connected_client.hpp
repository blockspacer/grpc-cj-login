#pragma once

#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>

#include "semaphore.hpp"

class ConnectedClient {
public:
  ConnectedClient() {
    context = NULL;
    writer = NULL;
  }
  ~ConnectedClient() {}

  grpc::ServerContext *context;
  grpc::ServerWriter<ServerMessage> *writer;
  Semaphore s;
};
