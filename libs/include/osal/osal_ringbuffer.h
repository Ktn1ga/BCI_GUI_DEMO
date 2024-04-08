/**
 * @file osal_ringbuffer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef OSAL_RING_BUFFER_H
#define OSAL_RING_BUFFER_H

#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include "export_global.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
  BS_OK = 0,          /* Operation successful */
  BS_ENOMEM = ENOMEM, /* Not enough memory */
  BS_EINVAL = EINVAL  /* Invalid arguments */
}buffer_status_t;

typedef void* ring_buffer_handle;

/**
 * @brief initialize a buffer
 * 
 * @param capacity buffer size
 * @return ** ring_buffer_handle buffer handle
 */
SHARED_EXPORT ring_buffer_handle buffer_init(size_t capacity);

/**
 * @brief destroy a buffer
 * 
 * @param buffer buffer handle
 * @return ** void 
 */
SHARED_EXPORT void buffer_destroy(ring_buffer_handle buffer);
SHARED_EXPORT size_t buffer_data_size(const ring_buffer_handle buffer);
SHARED_EXPORT size_t buffer_space_available(const ring_buffer_handle buffer);
SHARED_EXPORT buffer_status_t buffer_push(ring_buffer_handle buffer, void *data, size_t data_size);
SHARED_EXPORT buffer_status_t buffer_read(ring_buffer_handle buffer, void *dest, size_t data_size);
SHARED_EXPORT buffer_status_t buffer_pop(ring_buffer_handle buffer, void *dest, size_t data_size);
SHARED_EXPORT void buffer_clear(ring_buffer_handle buffer);
SHARED_EXPORT void buffer_print_info(const ring_buffer_handle buffer, FILE *stream);

#ifdef __cplusplus
}
#endif

#endif // OSAL_RING_BUFFER_H
