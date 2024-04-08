/**
 * @file osal_socket.h
 * @author Li Bo (libo_go@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef OSAL_SORKET_H
#define OSAL_SORKET_H

#ifdef _WIN32 //If Windows Platform
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
//#pragma comment(lib, "ws2_32.lib") //MSVC
#else //If Linux/UNIX Platform
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif
#include "export_global.h"
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32 
typedef int socklen_t;
#endif

SHARED_EXPORT int osal_socket(int domain, int type, int protocol);
SHARED_EXPORT unsigned short socket_htons(unsigned short hostshort);
SHARED_EXPORT unsigned long socket_inet_addr(const char* cp);
SHARED_EXPORT int socket_inet_pton(int domain, const char *cp, void *dst);
SHARED_EXPORT int socket_accept(int socket, struct sockaddr *addr, socklen_t *addrlen);
SHARED_EXPORT int socket_bind(int socket, const struct sockaddr *name, socklen_t namelen);
SHARED_EXPORT int socket_shutdown(int socket, int how);
SHARED_EXPORT int socket_connect(int socket, const struct sockaddr *name, socklen_t namelen);
SHARED_EXPORT int socket_listen(int socket, int backlog);
SHARED_EXPORT int socket_recv(int socket, void *mem, size_t len, int flags);
SHARED_EXPORT int socket_recvfrom(int socket, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
SHARED_EXPORT int socket_send(int socket, const void *dataptr, size_t size, int flags);
SHARED_EXPORT int socket_sendto(int socket, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
SHARED_EXPORT int socket_close(int socket);
SHARED_EXPORT int socket_setsockopt(int socket, int level, int optname, const char* optval, socklen_t optlen) ;
SHARED_EXPORT int socket_select(int socket, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
SHARED_EXPORT void socket_fd_zero(fd_set* set);
SHARED_EXPORT void socket_fd_set(int fd, fd_set* set);


#ifdef __cplusplus
}
#endif
#endif
