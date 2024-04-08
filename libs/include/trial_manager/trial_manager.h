/**
 * @file trial_manager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TRIAL_MANAGER_H
#define TRIAL_MANAGER_H
#include <stdio.h>
#include "export_global.h"

#define TRIAL_MAX_CLASS_NUM 8
#define TRIAL_LABLE_LENGTH 8
#define TRIAL_LABLE_INFO_LENGTH 64
#define DEVICE_PATH_LENGTH 256

typedef struct {
    int trial_num; //trial 数量 eg.30
    int class_num; //类型数量 eg.2
    char *trial_table; //if NULL, will generate with random
    char lable[TRIAL_MAX_CLASS_NUM][TRIAL_LABLE_LENGTH]; // "1,2"
    char lable_info[TRIAL_MAX_CLASS_NUM][TRIAL_LABLE_INFO_LENGTH]; // "left,right"
    int time_ms[TRIAL_MAX_CLASS_NUM]; // 4000,4000
    int sleep_ms[TRIAL_MAX_CLASS_NUM]; // 1500,1500
    char guide_device_name[DEVICE_PATH_LENGTH];  // vr/01
    char feedback_device_name[DEVICE_PATH_LENGTH]; // robot/01
} trial_config_t;


typedef void* trial_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Read a trial config file from path
 * 
 * @param path 
 * @return ** trial_handle_t 
 */
SHARED_EXPORT trial_handle_t trial_open(const char *path);

/**
 * @brief Read all trial config information
 * 
 * @param hdl 
 * @param buf 
 * @param size 
 * @return ** int 
 */
SHARED_EXPORT int trial_read(trial_handle_t hdl, char *buf, size_t size);

/**
 * @brief Change trial configs
 * 
 * @param hdl 
 * @param config 
 * @return ** int 
 */
SHARED_EXPORT int trial_config(trial_handle_t hdl, trial_config_t *config);

/**
 * @brief Close a trial, save changes to file
 * 
 * @param hdl 
 * @return ** int 
 */
SHARED_EXPORT int trial_close(trial_handle_t hdl);

/**
 * @brief A function that gets the number of trials.
 * 
 * @param hdl 
 * @param num 
 * @return ** int 
 */
SHARED_EXPORT int trial_get_number(trial_handle_t hdl, int *num);

/**
 * @brief A function that gets the device name.
 * 
 * @param hdl 
 * @param guide_dev 
 * @param feedback_dev 
 * @return ** int 
 */
SHARED_EXPORT int trial_get_device_name(trial_handle_t hdl, char **guide_dev, char **feedback_dev);

/**
 * @brief A function that gets the number of classes.
 * 
 * @param hdl 
 * @param num 
 * @return ** int 
 */
SHARED_EXPORT int trial_get_class_number(trial_handle_t hdl, int *num);

/**
 * @brief Getting the time information of the trial.
 * 
 * @param hdl 
 * @param lable_idx 
 * @param time_ms 
 * @param sleep_ms 
 * @return ** int 
 */
SHARED_EXPORT int trial_get_time(trial_handle_t hdl, size_t lable_idx, int *time_ms, int *sleep_ms);

/**
 * @brief Get trial table informaiton
 * 
 * @param hdl 
 * @param lable_table
 * @return ** int 
 */
SHARED_EXPORT int trial_get_label_table(trial_handle_t hdl, char **lable_table);

/**
 * @brief A function that gets the label information.
 * 
 * @param hdl 
 * @param lable_idx 
 * @param label 
 * @param info 
 * @return ** int 
 */
SHARED_EXPORT int trial_get_label_info(trial_handle_t hdl, size_t lable_idx, char **label, char **info);

#ifdef __cplusplus
}
#endif
#endif
