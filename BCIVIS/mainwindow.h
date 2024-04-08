#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "frame.h"
#include "osal/osal_log.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TimerHandle: public QObject{
    Q_OBJECT
private:
    std::queue<std::pair<int,int>> triallist;
    void startNextTimer(){
        if(!triallist.empty()){
            std::pair<int,int> currentTrial = triallist.front();
            int label = currentTrial.first,time = currentTrial.second;
            QTimer::singleShot(time,this,SLOT(onTimerTimeout()));
        }
    }
public:
    TimerHandle(std::queue<std::pair<int,int>> trials){
        triallist = trials;
        OSAL_LOG("hi","init");
        startNextTimer();
    }
signals:
    void setTrigger(int label);
private slots:
    void onTimerTimeout(){
        std::pair<int,int> currentTrial = triallist.front();
        int label = currentTrial.first,time = currentTrial.second;
        triallist.pop();
        emit setTrigger(label);
        OSAL_LOG("-","sending label:%d at %s",label,QDateTime::currentDateTime().toString(Qt::ISODate).toUtf8().constData());
        startNextTimer();
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void getTrigger(int label);

    void dynamicLemniscateSlot(); //绘图定时器调用

    void on_p1_pushButton_start_clicked();

    void on_p1_pushButton_stop_clicked();

    void on_p1_pushButton_trigger_clicked();

    void on_p1_pushButton_save_clicked();

    void on_p1_pushButton_load_clicked();

    void on_p1_pushButton_init_clicked();

    void on_p1_pushButton_test_clicked();

    void on_p1_pushButton_exp_clicked();

private:
    Ui::MainWindow *ui;
    TimerHandle *expTimerHandle;
    Frame *_frame;
    //存储系统参数
    std::map<std::string,std::string> _system_para;
    QString _json_filename;
    void commit(); //更新参数
    void load_clicked_1_get_file();  //选择参数文件
    void load_clicked_2_parse_file();   //解析参数文件
    ////接收数据标志位
    bool data_receive_flag = false;
    void _receive_data();
    ////波形显示:画布
    QCustomPlot *plot_raw_widget;
    QCustomPlot *plot_trigger_widget;
    ////波形显示：定时调用
    QTimer *plot_timer; //定时器
    void _setupPlot_raw(QCustomPlot *pCustomPlot);
    void _setupPlot_trigger(QCustomPlot *pCustomPlot);
    ////波形显示:参数设置
    bool plot_running_flag = true;
    int plot_x_range = 500;
    int plot_y_range = 100;
    double plot_y_proportion = 1.0/20;
    ////波形显示：数据
    double *plot_buf; //存储接收的buf
    int plot_size;   //buf的长度，一般设置为100
    QVector<QVector<double>> plot_vector;
    QVector<double> plot_x_data = QVector<double>(1);//存放数据的向量
    QVector<double> plot_y_data1 = QVector<double>(1);
    QVector<double> plot_y_data2 = QVector<double>(1);
    QVector<double> plot_y_data3 = QVector<double>(1);
    QVector<double> plot_y_data4 = QVector<double>(1);
    QVector<double> plot_y_data5 = QVector<double>(1);
    QVector<double> plot_y_data6 = QVector<double>(1);
    QVector<double> plot_y_data7 = QVector<double>(1);
    QVector<double> plot_y_data8 = QVector<double>(1);
    QVector<double> plot_t_data = QVector<double>(1);  //存放标签
};
#endif // MAINWINDOW_H
