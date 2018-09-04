#ifndef SERIAL_CLIENT
#define SERIAL_CLIENT

#include "common.h"

typedef struct{
  #ifdef WIN32
  LPCSTR portname;
  HANDLE hSerial;
  #endif
  char lasterr[0x40];
} serial_conn_t;

/* Opener + Closer */
int serial_open(serial_conn_t *conn, unsigned int id);
int serial_close(serial_conn_t *conn);
char *serial_error(serial_conn_t *conn);

/* Packet */
int serial_client_send_packet(serial_conn_t *conn, void *data, int len);
int serial_client_read_packet_len(serial_conn_t *conn, int *len);
int serial_client_read_packet_data(serial_conn_t *conn, void *data, int len);

/* TODO:
  - DER Encoding
  - more specific identifier
*/

/* Function call */
// int serial_call_init(serial_request_t *request, int id, int func, int nargs);
// int serial_call_clean(serial_request_t *request);
// int serial_call_param_int(serial_request_t *request, int index, unsigned char type, int data);
// int serial_call_param_binary(serial_request_t *request, int index, unsigned char type, void *data, int len);
// int serial_call_execute(serial_conn_t *conn, serial_request_t *request);
// int serial_call_result(serial_conn_t *conn, serial_response_t *response, int id);

#endif
