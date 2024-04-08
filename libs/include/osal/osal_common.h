/**
 * @file osal_common.h
 * @author Li Bo (libo_go@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef OSAL_COMMON_H
#define OSAL_COMMON_H

#include <stdio.h>
#include "export_global.h"

#ifdef __cplusplus
extern "C"
{
#endif

SHARED_EXPORT void osal_sleep_ms(int ms);

#ifdef __cplusplus
}
#endif

#endif
