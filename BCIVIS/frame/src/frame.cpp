#include "frame.h"
#include"driver_eeg.h"
#include"device_manager/device_manager.h"
#include"data_manager/data_manager.h"
#include"trial_manager/trial_manager.h"
#include "osal//osal_log.h"

#define TAG "FRAME:"
Frame::Frame(){
    ;
}

int Frame::get_data(void *buf,int plot_size,int channel){
    bool use_driver = true;
    if(use_driver==true){
        int result = device_read(this->eeg_device_handle,buf,plot_size); //从driver读
        return result;
    }
    int size = plot_size*channel*sizeof(double);
    int result = data_read(this->data_local_handle,buf,size); //从data manager读
    printf("%d",result);
    return result;
}
void Frame::init(std:: string config_filepath){
    _driver_init();
//    _data_manager_init();
    _trial_init(config_filepath);
}

void Frame::_driver_init()
{
//    char trial_path[512] = "D:/BCI_ALL/gui-shu-bci-v1/SHUBCI_1/config/driver.json";
//    char config[4096];

//    QFile file(file_name);
//    if(!file.open(QIODevice::ReadOnly)){
//        qDebug()<<"File open error";
//        return;
//    }
//    else{
//        qDebug()<<"File open!";
//    }
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
//    QJsonArray jsonArray = jsonDoc.array();
//    QJsonObject jsonObject = jsonArray[0].toObject();

//    QString jsonString = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
//    //    jsonString = jsonString.replace("\"","\\\"");
//    QByteArray byteArray = jsonString.toUtf8();
//    if(byteArray.size()<sizeof(config)){
//        strcpy(config,byteArray.data());
//    }else{
//        return;
//    }
//    eeg_device_id_t device_choosed= eeg_device_id_t(1);  // C++使用时需要对enum进行强制类型转换。
//    eeg_device_set_config(device_choosed,config);
    //选择设备
    eeg_device_id_t device_choosed= eeg_device_id_t(1);  // C++使用时需要对enum进行强制类型转换。
    eeg_device_deinit(device_choosed);
    //先初始化设备
    eeg_device_init(device_choosed);
    //使能
    eeg_device_enable(device_choosed);
    //再添加到设备管理器中,打开设备
    this->eeg_device_handle = device_open("/dev/eeg/1");
}

void Frame::_data_manager_init()
{
    OSAL_LOG(TAG,"data_manager_init")
    //初始化设置
    char *channel_info[11]= {  "PO5","PO3","POz","PO4","PO6","O1","Oz","O2","A1","A2","A3"  };
    char *cut_channel_info[9] = { "PO5","PO3","POz","PO4","PO6","O1","Oz","O2","trigger"};
    data_sample_set(1000);
    data_channel_set(11, channel_info);
    data_type_set(DATA_TYPE_DOUBLE);
    data_windowlength_set(3);

    eeg_device_id_t device_choosed= eeg_device_id_t(1);

    this->data_local_handle = data_open("1");

    data_channel_set(11, channel_info);
//    data_channel_set(9, cut_channel_info);
}

void Frame::_trial_init(std::string config_filepath){
    config_filepath = config_filepath +"trial.json";
    char trial_path[512];
    strcpy(trial_path,config_filepath.data());
    int trial_num;
    trial_handle_t hdl = trial_open(trial_path);
    //获取标签数量
    trial_get_number(hdl, &trial_num);
    //获取标签类别
    int label[20] = {1,2,1,1,2,2,1,1,1,2,2,2,1,1,1,1,2,2,2,2};
    OSAL_LOG(TAG,"trial num:%d",trial_num);
    for(int i=0;i<trial_num;++i){
        _trial_list.push({label[i],8000});
    }
}

std::queue<std::pair<int,int>> Frame::get_label_list(){
    int n = _trial_list.size();
//    for(int i=0;i<n;++i){
//        OSAL_LOG(TAG,"%d-%d",_trial_list.front().first,_trial_list.front().second);
//        _trial_list.push(_trial_list.front());
//        _trial_list.pop();
//    }
    return _trial_list;
}
