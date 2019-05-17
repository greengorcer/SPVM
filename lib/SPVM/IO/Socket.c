#include "spvm_native.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

// Module file name
static const char* MFILE = "SPVM/IO/Socket.c";

int32_t SPNATIVE__SPVM__IO__Socket__shutdown(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* obj_socket = stack[0].oval;
  
  int32_t handle;
  SPVM_IFIELD(env, handle, obj_socket, "SPVM::IO::Socket", "handle", MFILE, __LINE__);
  
  int32_t success = shutdown(handle, SHUT_WR);
  
  if (success != 0) {
    SPVM_DIE("Can't shutdown send operation", MFILE, __LINE__);
  }
  
  return SPVM_SUCCESS;
}

int32_t SPNATIVE__SPVM__IO__Socket__close(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* obj_socket = stack[0].oval;
  
  int32_t handle;
  SPVM_IFIELD(env, handle, obj_socket, "SPVM::IO::Socket", "handle", MFILE, __LINE__);
  
  if (handle >= 0) {
    int32_t ret = close(handle);
    if (ret == 0) {
      SPVM_SET_IFIELD(env, obj_socket, "SPVM::IO::Socket", "handle", -1, MFILE, __LINE__);
    }
    else {
      SPVM_DIE("Fail close", MFILE, __LINE__);
    }
  }
  
  return SPVM_SUCCESS;
}

int32_t SPNATIVE__SPVM__IO__Socket__fileno(SPVM_ENV* env, SPVM_VALUE* stack) {
  // Self
  void* obj_self = stack[0].oval;
  if (!obj_self) { SPVM_DIE("Self must be defined", MFILE, __LINE__); }
  
  // File fh
  int32_t handle;
  SPVM_IFIELD(env, handle, obj_self, "SPVM::IO::Socket", "handle", MFILE, __LINE__);
  
  stack[0].ival = handle;

  return SPVM_SUCCESS;
}

int32_t SPNATIVE__SPVM__IO__Socket__new(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* obj_deststr = stack[0].oval;
  const char* deststr = (const char*)env->belems(env, obj_deststr);
  int32_t port = stack[1].ival;
  
  int32_t handle = socket(AF_INET, SOCK_STREAM, 0);
  
  if (handle < 0) {
    SPVM_DIE("Can't create socket", MFILE, __LINE__);
  }
  
  struct sockaddr_in server;
  server.sin_addr.s_addr = inet_addr(deststr);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  if (server.sin_addr.s_addr == 0xffffffff) {
    struct hostent *host;
    
    host = gethostbyname(deststr);
    if (host == NULL) {
      if (h_errno == HOST_NOT_FOUND) {
        fprintf(stderr, "host not found : %s\n", deststr);
        SPVM_DIE("Exception", MFILE, __LINE__);
      }
      return 1;
    }
    
    unsigned int **addrptr = (unsigned int **)host->h_addr_list;
    
    while (*addrptr != NULL) {
      server.sin_addr.s_addr = *(*addrptr);
      if (connect(handle,
        (struct sockaddr *)&server,
        sizeof(server)) == 0) {
          break;
      }

      addrptr++;
    }
    if (*addrptr == NULL) {
      SPVM_DIE("Connect", MFILE, __LINE__);
    }
  }
  
  // Create SPVM::IO::Socket object
  void* obj_socket;
  SPVM_NEW_OBJ(env, obj_socket, "SPVM::IO::Socket", MFILE, __LINE__);
  
  // Set handle
  SPVM_SET_IFIELD(env, obj_socket, "SPVM::IO::Socket", "handle", handle, MFILE, __LINE__);
  
  stack[0].oval = obj_socket;
  
  return SPVM_SUCCESS;
}

int32_t SPNATIVE__SPVM__IO__Socket__write(SPVM_ENV* env, SPVM_VALUE* stack) {
  void* obj_socket = stack[0].oval;
  void* obj_buffer = stack[1].oval;
  const char* buffer = (const char*)env->belems(env, obj_buffer);
  int32_t length = stack[2].ival;
  
  int32_t handle;
  SPVM_IFIELD(env, handle, obj_socket, "SPVM::IO::Socket", "handle", MFILE, __LINE__);

  if (handle < 0) {
    SPVM_DIE("Handle is closed", MFILE, __LINE__);
  }
  
  /* HTTPリクエスト送信 */
  int32_t write_length = write(handle, buffer, length);
  if (write_length < 0) {
    SPVM_DIE("Socket write error", MFILE, __LINE__);
  }
  
  stack[0].ival = write_length;
  
  return SPVM_SUCCESS;
}

int32_t SPNATIVE__SPVM__IO__Socket__read(SPVM_ENV* env, SPVM_VALUE* stack) {
  void* obj_socket = stack[0].oval;
  void* obj_buffer = stack[1].oval;
  const char* buffer = (const char*)env->belems(env, obj_buffer);
  int32_t length = env->len(env, obj_buffer);
  
  int32_t handle;
  SPVM_IFIELD(env, handle, obj_socket, "SPVM::IO::Socket", "handle", MFILE, __LINE__);

  if (handle < 0) {
    SPVM_DIE("Handle is closed", MFILE, __LINE__);
  }
  
  /* HTTPリクエスト送信 */
  int32_t read_length = read(handle, (char*)buffer, length);
  if (read_length < 0) {
    SPVM_DIE("Socket read error", MFILE, __LINE__);
  }
  
  stack[0].ival = read_length;
  
  return SPVM_SUCCESS;
}
