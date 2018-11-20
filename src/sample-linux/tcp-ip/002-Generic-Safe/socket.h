#ifndef SOCKET_H_
#define SOCKET_H_

int read_socket(int sock, void *buf, size_t count);
int write_socket(int sock, const void *buf, size_t count);


#endif /* SOCKETUTIL_H_ */