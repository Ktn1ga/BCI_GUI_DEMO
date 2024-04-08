/**
 * @file data_manager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <stdio.h>
#include "device_manager/device_manager.h"
#include "cjson/cJSON.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef TEST_DATA_MANAGER
#define DATA_MANAGER_BASE_PATH BASE_PATH_GENRAL
#else
#define DATA_MANAGER_BASE_PATH BASE_PATH_EEG
#endif
#define BASE_PATH_LOCAL_FILE "/local_data"
#define BASE_PATH_LOCAL_DATA "data"
#define BASE_PATH_LOCAL_EVT  "evt"
#define BASE_PATH_LOCAL_LABEL "label"

#define FILETYPE_SAVE_EDFPLUS 1
#define FILETYPE_SAVE_BDFPLUS 2

typedef void* data_handle_t;

typedef enum {
    REMOTE_SERVER,
    REMOTE_CLIENT
} remote_type_t;

typedef struct {
    remote_type_t type;
    char* ip;
    unsigned short port;
    int auto_start;
} data_remote_config_t;

typedef enum {
    DATA_TYPE_DOUBLE,
    DATA_TYPE_FLOAT,
    DATA_TYPE_INT
} _date_type_t;

typedef enum{
    DATA_READ_ALL,
    DATA_READ_CUL
} data_read_t;

typedef struct{
    int task_num;
    int* task_label;
    int trial_start;
    int trial_stop;
    int block_start;
    int block_stop;
    int session_start;
    int session_stop;
    int open_eyes;
    int close_eyes;
    int rest;
} message_label_t;

typedef struct{
    double task_time;
    double cue_time;
    double break_time;
    double rest_time;
} message_task_t;

/**
 * @brief Open a data source
 * if open local device, data can be read from local or be sent to remote(as remote server),
 * if open remote device, data can be read from remote(as remote client).
 *
 * @param name
 * open local device, using ${id}
 * open remote device, using ${BASE_PATH_REMOTE}/${id}
 * @return data_handle_t data operate handle
 */
SHARED_EXPORT data_handle_t data_open(const char* name);

/**
 * @brief Turn off device reading
 *
 * @param handle data operate handle
 * @return
 */
SHARED_EXPORT void data_close(data_handle_t handle);

/**
 * @brief Read data from opened device
 *
 * @param handle data operate handle
 * @param buf data buffer to save recived data
 * @param size The length of data to be obtained
 * @return int actural read size
 */
SHARED_EXPORT int data_read(data_handle_t handle, void *buf,int size);

/**
 * @brief Save data to local file
 *
 * @param handle data operate handle
 * @param filetype Format of data file
 * @param file_path Path to save the data file
 * @return int
 */
SHARED_EXPORT void data_write(data_handle_t handle,int filetype,const char *file_path);

//SHARED_EXPORT void data_trigger_push(char *trigger_set);

/**
 * @brief Set Sampling Rate
 *
 * @param sample Sampling frequency of equipment
 * @return int
 */
SHARED_EXPORT int data_sample_set(int sample);

/**
 * @brief Set channel information
 *
 * @param channel Number of channels of the device
 * @param info Name of the channels
 * @return int
 */
SHARED_EXPORT int data_channel_set(int channel, char **info);

/**
 * @brief Set data type
 *
 * @param data_type Read Data Format
 * @return int
 */
SHARED_EXPORT int data_type_set(int data_type);

/**
 * @brief Set data window length
 *
 * @param length Time length
 * @return int
 */
SHARED_EXPORT int data_windowlength_set(int length);

/**
 * @brief Set label meaning
 *
 * @param label_meaning Meaning of labels
 * @param task_meaning Time for various tasks
 * @return SHARED_EXPORT
 */
SHARED_EXPORT void data_trigger_set(message_label_t* label_meaning, message_task_t* task_meaning);
/**
 * @brief Config remote server/client
 *
 * @param handle data operate handle
 * @param config
 * @return int
 */
SHARED_EXPORT void data_remote_config(data_handle_t handle, data_remote_config_t *config);

/**
 * @brief Start remote server/client
 *
 * @param handle data operate handle
 * @return
 */
SHARED_EXPORT void data_remote_start(data_handle_t handle);

/**
 * @brief Stop remote server/client
 *
 * @param handle data operate handle
 * @return
 */
SHARED_EXPORT void data_remote_stop(data_handle_t handle);

#ifdef __cplusplus
}
#endif
#endif
