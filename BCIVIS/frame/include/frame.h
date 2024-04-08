 #ifndef FRAME_H
#define FRAME_H
#include <QMainWindow>
#include "data_manager/data_manager.h"
#include "device_manager/device_manager.h"
#include <string>
#include <queue>
class Frame
{
private:
    void _trial_init(std::string config_filepath);
    void _data_manager_init();
    void _driver_init();
    std::queue<std::pair<int,int>> _trial_list;  //存储 pair<标签,时间>

public:
    device_handle_t eeg_device_handle = nullptr;  //eeg设备handle

    data_handle_t data_local_handle = nullptr;  //data manager设备handle


    void init(std::string config_filepath);

    int get_data(void *buf,int plot_size,int channel);
    std::queue<std::pair<int,int>> get_label_list();

    Frame();
};

#endif // FRAME_H
