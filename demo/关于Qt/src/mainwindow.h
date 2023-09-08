#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMqtt/qmqttclient.h>
#include <QtMqtt/qmqtttopicname.h>
#include <QtMqtt/qmqttsubscription.h>
#include <QDebug>
#include <QDateTime>
#include <QLatin1Char>
#include <QLatin1Char>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QMap>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QHostInfo>
#include <unistd.h>

#include<QTcpSocket>
#include<string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow*ui;

    QMqttClient*m_client;
    QString hostName;
    int hostPort;
    QString clientId;
    QString userName;
    QString passWord;
    int keepAlive;
    void pressAddress(QString addr);

    QTcpSocket*mp_client_Socket;
    QString ip_addr;
    quint16 port;
    char receive_Buffer[1024];
private slots:
    void on_btn_connect_clicked(void);
    void on_btn_clr_send_buf_click(void);
    void on_btn_exp2file_click(void);
    void on_btn_showtime_click(void);
    void on_btn_showtopic_click(void);
    void on_btn_publish_click(void);
    void on_btn_ping_click(void);

    void on_btn_clr_recv_buf_click(void);//清空接收区
    void on_btn_unsub_all_click(void);//取消全部订阅
    void on_btn_unsubtopic_click(void);//取消订阅
    void on_btn_subtopic_click(void);//订阅主题


    void updateLogStateChange();
    void brokenDisconnect(void);
    void topicMessageReceived(const QByteArray &message,const QMqttTopicName &topic);
    void pingResponseReceived(void);

    void lookedup(const QHostInfo &info);


    void connect_btn();
    void disconnect_btn();
    void connectedInfo();
    void disconnectedInfo();
    void ClientReadData();
    void ClientSendData();
};
#endif // MAINWINDOW_H
