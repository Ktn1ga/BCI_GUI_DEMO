
#ifndef OSAL_COM_H

#define OSAL_COM_H


#ifdef _WIN32 //If Windows Platform
#include <windows.h>
#include <Strsafe.h>

#else //If Linux/UNIX Platform

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#endif

#include "export_global.h"

#ifdef __cplusplus

extern "C"
{
#endif

#ifdef _WIN32

typedef HANDLE PORT;

#else //If Linux/UNIX Platform

static int maxSend = 1024;

#endif
/**
 * @brief com_init:串口初始化
 *
 * @param com_name:对于windows是串口号，对于linux是串口的路径
 * @param rate:波特率
 * @param databits:数据位
 * @param stopbits:停止位
 * @param parity:校验位
 * @return int:成功时返回串口的句柄，不成功则为特定的返回值
 */
SHARED_EXPORT int com_init(const char *com_name, const long rate, const int databits, const int stopbits, const int parity);
/**
 * @brief com_close:串口关闭
 *
 * @param fd:串口的句柄
 * @return int:特定的返回值
 */
SHARED_EXPORT int com_close(int fd);
/**
 * @brief com_send:串口发送
 *
 * @param fd:串口的句柄
 * @param sbuf:发送数组的数组名
 * @param sbuf_len:发送长度
 * @return int:特定的返回值
 */
SHARED_EXPORT int com_send(int fd, const char *sbuf, int sbuf_len);
/**
 * @brief com_recv:串口接收
 *
 * @param fd:串口的句柄
 * @param rbuf:接收数组的数组名
 * @param rbuf_len:接收长度
 * @return int:特定的返回值
 */
SHARED_EXPORT int com_recv(int fd, char *rbuf, int rbuf_len);

#ifdef __cplusplus

}
#endif

#endif
