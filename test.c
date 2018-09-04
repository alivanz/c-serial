#include <windows.h>
#include "client.h"
#include <stdio.h>

#define DEBUG(fx) ret = fx; if(ret != 0){ \
  fprintf(stderr, "line %i: " #fx "\n", __LINE__); \
  fprintf(stderr, "ERROR : %s\n", serial_error(conn)); \
  return -1; \
}

int main(){
  serial_conn_t *conn = malloc(sizeof(serial_conn_t));
  int ret;
  // Open COM6
  DEBUG(serial_open(conn, 6));

  char *msg = "hello world!";
  DEBUG(serial_client_send_packet(conn, msg, strlen(msg)+1));

  int len;
  char buf[0x40];
  DEBUG(serial_client_read_packet_len(conn, &len));
  DEBUG(serial_client_read_packet_data(conn, buf, len));

  DEBUG(serial_close(conn));
  return 0;
}
