/**
 * @file osal_thread.h
 * @author Li Bo (libo_go@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef OSAL_THREAD_H
#define OSAL_THREAD_H

#include "export_global.h"
#ifdef _WIN32 //If Windows Platform
#include "pthread.h"
#else //If Linux/UNIX Platform
#include <pthread.h>
#endif

typedef void* thread_handle_t;
typedef void* thread_mutex_t;
typedef void* thread_mutexattr_t;
typedef void* (*thread_code)(void*);

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief osal_thread_create:创造线程，该线程是可连接的（joinable）
 *
 * @param thread:&线程标识符名，thread_handle_t变量的指针，用于接收新创建的线程的标识符
 * @param code:线程函数名，void* (*)(void*)，指向线程入口函数（线程执行的起始函数）的指针,该函数需要提前定义好
 * @param arg:(void*)传递的变量名，void*，传递给线程入口函数的参数，可以是结构体等变量
 * @return int:返回特定值，成功为0，不成功为特定的错误代码
 */
SHARED_EXPORT int osal_thread_create(thread_handle_t *thread, thread_code code, void *arg);

/**
 * @brief osal_thread_cancel:取消线程函数，仅向目标线程发送取消请求，并不等待目标线程终止执行，目标线程是否响应这个线程取消请求取决于线程的取消状态和取消类型，用osal库创建的线程属性是取消使能（enable）状态和推迟取消（deferred）类型
 *
 * @param thread:&线程标识符名，thread_handle_t变量，用于接收新创建的线程的标识符
 * @return int:返回特定值，成功为0，不成功为特定的错误代码，需注意，发送取消信号成功，不代表线程一定会终止，具体处理取决于线程的设置
 */
SHARED_EXPORT int osal_thread_cancel(thread_handle_t thread);

/** 
 * @brief  osal_thread_join:用于等待一个线程结束并获取其返回值的函数，目标线程必须是可连接的（即处于 joinable 状态），注意该函数会阻塞当前线程，直到目标线程结束
 * 
 * @param thread:&线程标识符名，thread_handle_t变量，用于接收新创建的线程的标识符
 * @param value_ptr:&指向void类型的指针或NULL，如果 value_ptr 参数 不为 NULL，则 pthread_join() 函数会复制目标线程的退出状态
 * @return ** int:返回特定值，成功为0
 */
SHARED_EXPORT int osal_thread_join(thread_handle_t thread, void **value_ptr);

/**
 * @brief osal_thread_detach:将指定的线程标记为分离状态（detached state），使得该线程在终止时可以自动释放其资源，
 * 
 * @param thread:&线程标识符名，thread_handle_t变量，用于接收新创建的线程的标识符
 * @return ** int:返回特定值，成功为0
 */
SHARED_EXPORT int osal_thread_detach(thread_handle_t thread);

/**
 * @brief pthread_mutex_init:初始化互斥锁
 * 
 * @param mutex:&互斥锁标识符名，thread_mutex_t类型变量的指针，用于接收新创建的线程的标识符
 * @param attr:&互斥锁属性标识名或NULL,thread_mutexattr_t类型变量的指针
 * @return ** int:返回特定值，成功为0
 */
SHARED_EXPORT int osal_thread_mutex_init(thread_mutex_t *mutex, const thread_mutexattr_t *attr);

/**
 * @brief osal_thread_mutex_lock:将用于锁定（获取）互斥锁
 * 
 * @param mutex:&互斥锁标识符名，thread_mutex_t类型变量的指针，用于接收新创建的线程的标识符
 * @return ** int:返回特定值，成功为0
 */
SHARED_EXPORT int osal_thread_mutex_lock(thread_mutex_t *mutex);

/**
 * @brief osal_thread_detach:用于解锁（释放）互斥锁
 * 
 * @param mutex:&互斥锁标识符名，thread_mutex_t类型变量的指针，用于接收新创建的线程的标识符
 * @return ** int:返回特定值，成功为0
 */
SHARED_EXPORT int osal_thread_mutex_unlock(thread_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif
