#ifndef SERIAL_COMMON
#define SERIAL_COMMON

/* Data type */
#define FUNCTION  0x01
#define RESPONSE  0x02
// #define BYTE      0x11
// #define SHORT     0x12
// #define INTEGER   0x14
// #define BINARY    0x43

/* Function call */
// typedef struct{
//   unsigned int id;
//   unsigned int flag;
// } serial_packet_header_t;
// typedef struct{
//   serial_packet_header_t header;
//   unsigned int len;
//   void *data;
// } serial_packet_t;

/* Basic Function */
// int serial_send_packet(serial_packet_t *packet);
// int serial_read_packet();
// int serial_write_len();
// int serial_read_len();

#endif
