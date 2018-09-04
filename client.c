#ifdef WIN32
#include <windows.h>
#endif
#include "client.h"
#include <stdio.h>

/* Opener + Closer */
#ifdef WIN32
int serial_open(serial_conn_t *conn, unsigned int id){
  char *port = malloc(7);
  sprintf(port, "COM%u", id);
  conn->portname = port;
  DWORD  accessdirection =GENERIC_READ | GENERIC_WRITE;
  HANDLE hSerial = CreateFile(conn->portname,
    accessdirection,
    0,
    0,
    OPEN_EXISTING,
    0,
    0);
  if (hSerial == INVALID_HANDLE_VALUE) {
    //call GetLastError(); to gain more information
    sprintf(conn->lasterr, "Windows error code: %i", GetLastError());
    return -1;
  }

  //After opening the port further settings like Baudrate, Byte size, the number of stopbits and the Parity need to be set.
  DCB dcbSerialParams = {0};
  dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
  if (!GetCommState(hSerial, &dcbSerialParams)) {
    //could not get the state of the comport
    sprintf(conn->lasterr, "could not get the state of the comport");
    return -1;
  }
  dcbSerialParams.BaudRate=460800;
  dcbSerialParams.ByteSize=8;
  dcbSerialParams.StopBits=ONESTOPBIT;
  dcbSerialParams.Parity=NOPARITY;
  if(!SetCommState(hSerial, &dcbSerialParams)){
    //analyse error
    sprintf(conn->lasterr, "set state error");
  }

  //Finally timeouts need to be set so that the program does not hang up when receiving nothing.
  COMMTIMEOUTS timeouts={0};
  timeouts.ReadIntervalTimeout=50;
  timeouts.ReadTotalTimeoutConstant=50;
  timeouts.ReadTotalTimeoutMultiplier=10;
  timeouts.WriteTotalTimeoutConstant=50;
  timeouts.WriteTotalTimeoutMultiplier=10;
  if(!SetCommTimeouts(hSerial, &timeouts)){
    //handle error
    sprintf(conn->lasterr, "set timeout error");
  }

  return 0;
}
int serial_close(serial_conn_t *conn){
  CloseHandle(conn->hSerial);
  return 0;
}
#endif

char *serial_error(serial_conn_t *conn){
  return conn->lasterr;
}

/* Packet */
int serial_client_write(HANDLE hSerial, void *data, int length){
  DWORD dwBytesRead = 0;
	if(!WriteFile(hSerial, data, length, &dwBytesRead, NULL)){
		// printLastError();
    return -1;
	}
  if(dwBytesRead!=length){
    return -2;
  }
	return 0;
}
int readFromSerialPort(HANDLE hSerial, void *buffer, int buffersize){
  DWORD dwBytesRead = 0;
  if(!ReadFile(hSerial, buffer, buffersize, &dwBytesRead, NULL)){
    //handle error
    return -1;
  }
  if(dwBytesRead!=buffersize){
    return -2;
  }
  return 0;
}
int serial_client_send_len(serial_conn_t *conn, int len){
  // TODO: DER Encoding
  int ret = serial_client_write(conn->hSerial, &len, sizeof(int));
  if(ret!=0){
    sprintf(conn->lasterr, "serial_client_write error %i", ret);
    return -1;
  }
  return 0;
}
int serial_client_send_packet(serial_conn_t *conn, void *data, int len){
  int ret = serial_client_send_len(conn, len);
  if(ret != 0){
    sprintf(conn->lasterr, "serial_client_send_len error %i", ret);
    return -1;
  }
  return serial_client_write(conn->hSerial, data, len);
}
int serial_client_read_packet_len(serial_conn_t *conn, int *len){
  // TODO: DER Encoding
  int ret = readFromSerialPort(conn->hSerial, len, sizeof(int));
  if(ret != 0){
    sprintf(conn->lasterr, "readFromSerialPort error %i", ret);
    return -1;
  }
  return 0;
}
int serial_client_read_packet_data(serial_conn_t *conn, void *data, int len){
  int ret = readFromSerialPort(conn->hSerial, data, len);
  if(ret !=0 ){
    sprintf(conn->lasterr, "readFromSerialPort error %i", ret);
    return -1;
  }
  return 0;
}

/* Function call */
// int serial_call_init(serial_request_t *request, int id, int func, int nargs){
//   request->id = id;
//   request->func = func;
//   request->nparam = nargs;
//   request->params = malloc(sizeof(serial_frame_t) * nargs);
//   return 0;
// }
// int serial_call_clean(serial_request_t *request){
//   free(request->params);
//   memset(request, 0, sizeof(serial_request_t));
//   return 0;
// }
// int serial_call_param_init(serial_request_t *request, int index, unsigned char type){
//   int size;
//   switch (type) {
//     case BYTE:    size = sizeof(char); break;
//     case SHORT:   size = sizeof(short); break;
//     case INTEGER: size = sizeof(int); break;
//     default:
//     return -1;
//   }
//   if(request->params[index]->data){
//     free(request->params[index]->data);
//   }
//   request->len = size;
//   request->params[index]->data = malloc(size);
//   return 0;
// }
// int serial_call_param_int(serial_request_t *request, int index, int data){
//   serial_call_param_init(request, index, INTEGER);
//   int *x = request->params[index]->data;
//   *x = data;
//   return 0;
// }
// int serial_call_param_binary(serial_request_t *request, int index, unsigned char type, void *data, int len){
//
// }
