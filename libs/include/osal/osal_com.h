
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
 * @brief com_init:���ڳ�ʼ��
 *
 * @param com_name:����windows�Ǵ��ںţ�����linux�Ǵ��ڵ�·��
 * @param rate:������
 * @param databits:����λ
 * @param stopbits:ֹͣλ
 * @param parity:У��λ
 * @return int:�ɹ�ʱ���ش��ڵľ�������ɹ���Ϊ�ض��ķ���ֵ
 */
SHARED_EXPORT int com_init(const char *com_name, const long rate, const int databits, const int stopbits, const int parity);
/**
 * @brief com_close:���ڹر�
 *
 * @param fd:���ڵľ��
 * @return int:�ض��ķ���ֵ
 */
SHARED_EXPORT int com_close(int fd);
/**
 * @brief com_send:���ڷ���
 *
 * @param fd:���ڵľ��
 * @param sbuf:���������������
 * @param sbuf_len:���ͳ���
 * @return int:�ض��ķ���ֵ
 */
SHARED_EXPORT int com_send(int fd, const char *sbuf, int sbuf_len);
/**
 * @brief com_recv:���ڽ���
 *
 * @param fd:���ڵľ��
 * @param rbuf:���������������
 * @param rbuf_len:���ճ���
 * @return int:�ض��ķ���ֵ
 */
SHARED_EXPORT int com_recv(int fd, char *rbuf, int rbuf_len);

#ifdef __cplusplus

}
#endif

#endif
