#ifndef OSAL_LOG_H
#define OSAL_LOG_H
#include "stdio.h"

typedef int osal_err_t;

/* Definitions for error constants. */
#define OSAL_OK          0       /*!< osal_err_t value indicating success (no error) */
#define OSAL_FAIL        -1      /*!< Generic osal_err_t code indicating failure */

#define OSAL_ERR_NO_MEM              0x101   /*!< Out of memory */
#define OSAL_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
#define OSAL_ERR_INVALID_STATE       0x103   /*!< Invalid state */
#define OSAL_ERR_INVALID_SIZE        0x104   /*!< Invalid size */
#define OSAL_ERR_NOT_FOUND           0x105   /*!< Requested resource not found */
#define OSAL_ERR_NOT_SUPPORTED       0x106   /*!< Operation or feature not supported */
#define OSAL_ERR_TIMEOUT             0x107   /*!< Operation timed out */
#define OSAL_ERR_INVALID_RESPONSE    0x108   /*!< Received response was invalid */
#define OSAL_ERR_INVALID_CRC         0x109   /*!< CRC or checksum was invalid */
#define OSAL_ERR_INVALID_VERSION     0x10A   /*!< Version was invalid */
#define OSAL_ERR_INVALID_MAC         0x10B   /*!< MAC address was invalid */
#define OSAL_ERR_NOT_FINISHED        0x10C   /*!< There are items remained to retrieve */

#define OSAL_LOG(TAG, ...) {printf(TAG ": " __VA_ARGS__); printf("\n");}

#define OSAL_CHECK(TAG, a, str, ret) if(!(a)) { \
        printf(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
        return (ret); \
    }

#define OSAL_CHECK_GOTO(TAG, a, str, lable) if(!(a)) { \
        printf(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
        goto lable; \
    }

#endif // OSAL_LOG_H
