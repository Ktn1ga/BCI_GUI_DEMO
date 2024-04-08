#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "frame.h"

#include "osal/osal_log.h"
#include "driver_eeg.h"
#include "data_manager/data_manager.h"

# define TAG "MAINWINDOW"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //初始化框架
    _frame = new Frame();
    ui->setupUi(this);
    //波形初始设置
    plot_raw_widget = ui->p1_pustomplot_raw;
    _setupPlot_raw(plot_raw_widget);

    plot_trigger_widget = ui->p1_pustomplot_trigger;
    _setupPlot_trigger(plot_trigger_widget);

    //绘制波形的定时器
    plot_timer = new QTimer(this);
    connect(plot_timer, SIGNAL(timeout()), this, SLOT(dynamicLemniscateSlot()));
    plot_timer->start(10);      // 10ms刷新一次
}

MainWindow::~MainWindow()
{
    delete ui;
}

//绘制波形画布——初始化
void MainWindow::_setupPlot_raw(QCustomPlot *pCustomPlot)
{
    //添加曲线
    QCPGraph* pgraph1 = pCustomPlot->addGraph();
    QCPGraph* pgraph2 = pCustomPlot->addGraph();
    QCPGraph* pgraph3 = pCustomPlot->addGraph();
    QCPGraph* pgraph4 = pCustomPlot->addGraph();
    QCPGraph* pgraph5 = pCustomPlot->addGraph();
    QCPGraph* pgraph6 = pCustomPlot->addGraph();
    QCPGraph* pgraph7 = pCustomPlot->addGraph();
    QCPGraph* pgraph8 = pCustomPlot->addGraph();

    //修改纵坐标标签
    QVector<double> ticks;
    QVector<QString> labels;
    //设置X,Y轴范围
    pCustomPlot->yAxis->setRange(-plot_y_range,plot_y_range); // Y轴上下限为±Show_Range（default:100）
    pCustomPlot->xAxis->setRange(0,plot_x_range);//（default:500）

    // 替换刻度标签
    //设置Y轴标签 首先向向量中添加实际的坐标值作为位置指示，然后添加labels替换掉目标处的值
    int Channel_Num = 8;
    for(int i = 0; i < Channel_Num; i++){
        int j = -plot_y_range*0.8 + 20*i; // 选取需要替换的刻度值并输入到向量中
        ticks<< j;
    }
    labels << "CP5" << "Cz" << "CP6" << "FC5" << "C3" << "Fz" << "C4" << "FC6"; //将替换的刻度值标签添加到向量
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    pCustomPlot->yAxis->setTicker(textTicker);

     //设置数据
    pCustomPlot->graph(0)->setData(this->plot_x_data,this->plot_y_data1);
    pCustomPlot->graph(1)->setData(this->plot_x_data,this->plot_y_data2);
    pCustomPlot->graph(2)->setData(this->plot_x_data,this->plot_y_data3);
    pCustomPlot->graph(3)->setData(this->plot_x_data,this->plot_y_data4);
    pCustomPlot->graph(4)->setData(this->plot_x_data,this->plot_y_data5);
    pCustomPlot->graph(5)->setData(this->plot_x_data,this->plot_y_data6);
    pCustomPlot->graph(6)->setData(this->plot_x_data,this->plot_y_data7);
    pCustomPlot->graph(7)->setData(this->plot_x_data,this->plot_y_data8);
    //x轴名字
    pCustomPlot->xAxis->setLabel("Time");
    //Y轴名字
    pCustomPlot->yAxis->setLabel("Channel");
    //设置大小
    pCustomPlot->resize(ui->p1_pustomplot_raw->width(),ui->p1_pustomplot_raw->height());
    //可以进行鼠标位置 放大缩小 拖拽 放大缩小坐标系
    pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    //重绘 每次改变完以后都要调用这个进行重新绘制
    pCustomPlot->replot();
}
void MainWindow::_setupPlot_trigger(QCustomPlot *pCustomPlot)
{
    //添加曲线
    QCPGraph* pgraph = pCustomPlot->addGraph();

    //设置X轴范围
    pCustomPlot->xAxis->setRange(0,plot_x_range);//（default:500）

     //设置数据
    pCustomPlot->graph(0)->setData(this->plot_x_data,this->plot_t_data);

    //x轴名字
    pCustomPlot->xAxis->setLabel("Time");
    //Y轴名字
    pCustomPlot->yAxis->setLabel("Trigger");
    //设置大小
    pCustomPlot->resize(ui->p1_pustomplot_raw->width(),ui->p1_pustomplot_raw->height());
    //可以进行鼠标位置 放大缩小 拖拽 放大缩小坐标系
    pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    //重绘 每次改变完以后都要调用这个进行重新绘制
    pCustomPlot->replot();
}

//刷新图像 （槽函数：定时中断响应）
void MainWindow::dynamicLemniscateSlot()
{
    //接收数据
    if(data_receive_flag){
        _receive_data();
    }
    if(plot_vector.empty()){
        return;
    }
    // 找到X的长度并增加1的长度
    int i = this->plot_x_data.length();
    while(!plot_vector.empty()){
        // 找到X的长度并增加1的长度
        i = this->plot_x_data.length();
        if(this->plot_x_data.length() == 0) i = 0;
        else i = this->plot_x_data.last() + 1;

        this->plot_x_data.append(i) ;

        double plot_temp[10]={0};
        for (int j=0;j<10;j++) {
            if(j<8){
                plot_temp[j]= plot_vector.first()[j]/plot_y_proportion; //设置缩放比例
            }else{
                plot_temp[j]= plot_vector.first()[j];
            }
        }
//        OriData2WaveMutex.lock();//进程锁
        plot_vector.pop_front();
//        OriData2WaveMutex.unlock();
        this->plot_y_data2.append(plot_temp[0]+60);
        this->plot_y_data3.append(plot_temp[1]+40);
        this->plot_y_data4.append(plot_temp[2]+20);
        this->plot_y_data1.append(plot_temp[3]);
        this->plot_y_data5.append(plot_temp[4]-20);
        this->plot_y_data6.append(plot_temp[5]-40);
        this->plot_y_data7.append(plot_temp[6]-60);
        this->plot_y_data8.append(plot_temp[7]-80);

        //放电量
        //电量显示
        int batteryFlag= plot_temp[8];
        //        qDebug()<<"*********** batteryFlag **********"<<batteryFlag;
        //        ui->progressBar_battery->setValue(batteryFlag);
        if(batteryFlag>=50 && batteryFlag <= 100)
        {
            ui->progressBar_battery->setStyleSheet("QProgressBar{border: 2px solid grey;"
                                                   "border-radius: 1px;"
                                                   "text-align: center}"
                                                   "QProgressBar::chunk {background-color: green;}");

        }
        else if(batteryFlag<=49 && batteryFlag >= 20)
        {
            ui->progressBar_battery->setStyleSheet("QProgressBar{border: 2px solid grey;"
                                                   "border-radius: 1px;"
                                                   "text-align: center}"
                                                   "QProgressBar::chunk {background-color: orange;}");
        }
        else
        {
            ui->progressBar_battery->setStyleSheet("QProgressBar{border: 2px solid grey;"
                                                   "border-radius: 1px;"
                                                   "text-align: center}"
                                                   "QProgressBar::chunk {background-color: red;}");
        }
        ui->progressBar_battery->setValue(batteryFlag);
        //100-50 绿色
        //20-49 黄色
        //19 红色

        //放标签
        this->plot_t_data.append(plot_temp[9]);
    }

    while(this->plot_x_data.size() > (int)(plot_x_range * 0.9))
    {
        this->plot_x_data.pop_front();
        this->plot_y_data1.pop_front();
        this->plot_y_data2.pop_front();
        this->plot_y_data3.pop_front();
        this->plot_y_data4.pop_front();
        this->plot_y_data5.pop_front();
        this->plot_y_data6.pop_front();
        this->plot_y_data7.pop_front();
        this->plot_y_data8.pop_front();
        this->plot_t_data.pop_front();
    }
    //更新X,Y数值到曲线
    if(this->plot_running_flag == 1)
    {
        plot_raw_widget->graph(0)->setData(this->plot_x_data,this->plot_y_data1);  //更新曲线1数据
        plot_raw_widget->graph(1)->setData(this->plot_x_data,this->plot_y_data2); //更新曲线2数据
        plot_raw_widget->graph(2)->setData(this->plot_x_data,this->plot_y_data3);  //更新曲线3数据
        plot_raw_widget->graph(3)->setData(this->plot_x_data,this->plot_y_data4); //更新曲线4数据
        plot_raw_widget->graph(4)->setData(this->plot_x_data,this->plot_y_data5);  //更新曲线5数据
        plot_raw_widget->graph(5)->setData(this->plot_x_data,this->plot_y_data6); //更新曲线6数据
        plot_raw_widget->graph(6)->setData(this->plot_x_data,this->plot_y_data7);  //更新曲线7数据
        plot_raw_widget->graph(7)->setData(this->plot_x_data,this->plot_y_data8); //更新曲线8数据

        plot_trigger_widget->graph(0)->setData(this->plot_x_data,this->plot_t_data); //更新trigger数据

        if(i >= (int)(plot_x_range)){
            //改变坐标轴范围
            plot_raw_widget->xAxis->setRange(i-(int)(plot_x_range * 0.9),i+(int)(plot_x_range * 0.05));

            plot_trigger_widget->xAxis->setRange(i-(int)(plot_x_range * 0.9),i+(int)(plot_x_range * 0.05));

        }

        plot_trigger_widget->yAxis->setRange(-1,11);
        //重绘
        plot_raw_widget->replot();
        plot_trigger_widget->replot();
     }
}

//接收数据
void MainWindow::_receive_data()
{
    plot_size =1;
    int channel = 11;
    plot_buf = (double *)malloc(plot_size*channel*sizeof(double));  //获取数据

    int result = _frame->get_data(plot_buf,plot_size,channel);
//    qDebug();
    if(result != -1){
        for(int i=0;i<plot_size;i++){
            QVector<double> single_value;
            for(int j=0;j<8;j++){
//                qDebug()<<(double)plot_buf[i*channel+j];
                single_value.append(plot_buf[i*channel+j]);
            }
            // 存 电量
//            qDebug()<<(double)plot_buf[i*channel+9];
            single_value.append(plot_buf[i*channel + 9]);
            // 存 trigger
//            qDebug()<<(double)plot_buf[i*channel+10];
            single_value.append(plot_buf[i*channel + 10]);

            plot_vector.append(single_value);
        }
    }
}

//开始
void MainWindow::on_p1_pushButton_start_clicked()
{
    this->ui->p1_textBrowser_log->append("start\n");
    data_receive_flag = true;
//    device_control(_frame->eeg_device_handle,"stop",NULL);   // 如果上次使用没有关闭，就要先关闭
    device_control(_frame->eeg_device_handle,"start",NULL);
}

//停止
void MainWindow::on_p1_pushButton_stop_clicked()
{
    this->ui->p1_textBrowser_log->append("stop\n");
    data_receive_flag = false;
    device_control(_frame->eeg_device_handle,"stop",NULL);
}

//打标
void MainWindow::on_p1_pushButton_trigger_clicked()
{
    int t = this->ui->p1_spinBox_trigger->value();
    this->ui->p1_textBrowser_log->append(QString("put trigger %1").arg(t));
    device_control(_frame->eeg_device_handle,"trigger",&t);
}

//初始化
void MainWindow::on_p1_pushButton_init_clicked()
{
    std::string config_filepath = "D:/BCI_ALL/gui-shu-bci-v1/SHUBCI_1/config/";
    _json_filename = QString::fromStdString(config_filepath+"driver.json");
    qDebug()<<_json_filename;
    load_clicked_2_parse_file(); //加载json，显示到ui上
    this->_frame->init(config_filepath);
    this->ui->p1_textBrowser_log->append("INIT\n");
//    system("start http://192.168.31.202/cgi-bin/MultiTPDstart");
}


//save 保存设备信息
void MainWindow::on_p1_pushButton_save_clicked()
{
    qDebug()<<"ptn_save_clicked";
    commit();  //理论上这里只要做从ui文件保存到json中，但考虑到ui的参数可能被framework需要。这里的逻辑是 ui->_system_para， _system_para保存。
    QString file_name = QFileDialog::getSaveFileName(this,tr("save device config"),_json_filename,"device config(*.json)");
    qDebug()<<"filename"<<file_name;
    QFile file(file_name);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"File open error";
    }
    else{
        qDebug()<<"File open!";
    }
    QJsonObject jsonObject;
    std::map<std::string,std::string>::iterator it;
    for(it=_system_para.begin();it!=_system_para.end();it++){
        jsonObject.insert(it->first.data(),it->second.data());
    };
    QJsonArray jsonArray;
    jsonArray.append(jsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);
    file.write(jsonDoc.toJson());
    file.close();

    //保存后，重新加载更新数据
    _json_filename = file_name;
    load_clicked_2_parse_file();
}

void MainWindow::commit()
{
    qDebug()<<"commit";
    QDateTime current_date_time = QDateTime::currentDateTime();
    _system_para["tcp_ip"] = ui->p1_textEdit_ip->text().toStdString();
    _system_para["buffer_size"] = ui->p1_textEdit_buffer_size->text().toStdString();
    _system_para["packet_size"] = ui->p1_textEdit_packet_size->text().toStdString();
    _system_para["channels"] = ui->p1_textEdit_channel->text().toStdString();
    std::map<std::string,std::string>::iterator it;
    for(it=_system_para.begin();it!=_system_para.end();it++){
        qDebug()<<it->first.data()<<it->second.data();
    };
    //保存的同时，更新ui和
    load_clicked_2_parse_file();
}

//load 加载设备信息
void MainWindow::on_p1_pushButton_load_clicked(){
    load_clicked_1_get_file();  //选择文件
    load_clicked_2_parse_file();//更新
}

void MainWindow::load_clicked_1_get_file()
{
    QString initialPath = "D:\BCI_ALL\gui\SHUBCI_1";
    QString file_name = QFileDialog::getOpenFileName(this,tr("open device config"),initialPath,"device config fie(*json)");
    qDebug()<<"filename"<<file_name;
    ui->groupBox_setting->setTitle(file_name);
    _json_filename = file_name;
}

void MainWindow:: load_clicked_2_parse_file(){
    QString file_name = _json_filename;
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"File open error";
        return;
    }
    else{
        qDebug()<<"File open!";
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    QJsonArray jsonArray = jsonDoc.array();
    QJsonObject jsonObject = jsonArray[0].toObject();
//    std::map<std::string,std::string> m_system_para;
    _system_para.clear();
    //加载到UI
    for(auto it = jsonObject.begin();it!=jsonObject.end();it++){
        _system_para[it.key().toStdString()]= it.value().toString().toStdString() ;
    }
    for(auto it=_system_para.begin();it!=_system_para.end();it++){
        qDebug()<<it->first.data()<<it->second.data();
    };
    ui->p1_textEdit_ip->setText(QString::fromStdString(_system_para["tcp_ip"]));
    ui->p1_textEdit_buffer_size->setText(QString::fromStdString(_system_para["buffer_size"]));
    ui->p1_textEdit_packet_size->setText(QString::fromStdString(_system_para["packet_size"]));
    ui->p1_textEdit_channel->setText(QString::fromStdString(_system_para["channels"]));
    qDebug()<<"end";
    //加载到driver
    char config[4096];
    QString jsonString = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
    //    jsonString = jsonString.replace("\"","\\\"");
    QByteArray byteArray = jsonString.toUtf8();
    if(byteArray.size()<sizeof(config)){
        strcpy(config,byteArray.data());
    }else{
        return;
    }
    eeg_device_id_t device_choosed= eeg_device_id_t(1);  // C++使用时需要对enum进行强制类型转换。
    eeg_device_set_config(device_choosed,config,NULL);
}


void MainWindow::on_p1_pushButton_exp_clicked()
{
    OSAL_LOG(TAG,"EXP START");
    std::queue<std::pair<int,int>> trials = this->_frame->get_label_list();
    expTimerHandle = new TimerHandle(trials);
    connect(this->expTimerHandle,&TimerHandle::setTrigger,this,&MainWindow::getTrigger);
}

void MainWindow::getTrigger(int label){
    OSAL_LOG(TAG,"GET Trigger %d",label);
    device_control(_frame->eeg_device_handle,"trigger",&label);
}


void MainWindow::on_p1_pushButton_test_clicked()
{
    ;
}



