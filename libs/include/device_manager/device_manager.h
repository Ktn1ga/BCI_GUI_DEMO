/**
 * @file device_manager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "export_global.h"

#define BASE_PATH_REMOTE_PREFIX "/remote"
#define BASE_PATH_GENRAL "/dev/general"
#define BASE_PATH_EEG "/dev/eeg"
#define BASE_PATH_VR "/dev/vr"
#define BASE_PATH_ROBOT "/dev/robot"

typedef struct {
    int (*control)(char *command, void *param);
    int (*write)(void *buf, size_t size);
    int (*read)(void *buf, size_t size);
}device_commom_api_t;

typedef enum {
    DEVICE_TYPE_GENERAL = 0x00,
    DEVICE_TYPE_EEG = 0x01,
    DEVICE_TYPE_VR = 0x02,
    DEVICE_TYPE_ROBOT = 0x03,
    DEVICE_TYPE_MAX,
}device_type_t;

typedef struct {
    device_type_t type;
    char name[256];
    char path[256];
    char serial[256];
} device_info_t;

typedef void* device_handle_t;

SHARED_EXPORT int general_device_register(const char* base_path, int id, device_commom_api_t* apis);
SHARED_EXPORT int eeg_device_register(const char* base_path, int id, device_commom_api_t* apis);
SHARED_EXPORT int vr_device_register(const char* base_path, int id, device_commom_api_t* apis);
SHARED_EXPORT int robot_device_register(const char* base_path, int id, device_commom_api_t* apis);

SHARED_EXPORT int device_register(const char* base_path, device_type_t type, int id, device_commom_api_t* apis);
SHARED_EXPORT int device_unregister(int handle);

//int device_manager_init(void* param);
SHARED_EXPORT device_handle_t device_open(const char* path);
SHARED_EXPORT int device_close(device_handle_t handle);
SHARED_EXPORT int device_read(device_handle_t handle, void *buf, size_t size);
SHARED_EXPORT int device_write(device_handle_t handle, void *buf, size_t size);
SHARED_EXPORT int device_control(device_handle_t handle, char *command, void *param);
//SHARED_EXPORT int device_get_serial(device_handle_t handle, char *serial);
//SHARED_EXPORT int device_get_name(device_handle_t handle, char *name);
//SHARED_EXPORT int device_get_info(device_handle_t handle, device_info_t *info);
//SHARED_EXPORT int device_get_all_info(device_info_t *info, size_t num);


#ifdef __cplusplus
}
#endif
#endif
