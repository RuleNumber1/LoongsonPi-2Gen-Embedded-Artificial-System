#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mywidgetitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_addr->setText("mqtts.heclouds.com");
    ui->checkBox_port->setChecked(1);
    ui->comboBox_clientId->addItem("dev1");
    ui->comboBox_clientId->addItem("dev2");
    ui->lineEdit_productID->setText("FqrA5Q4Nuy");
    ui->comboBox_Token->addItem("version=2018-10-31&res=products%2FFqrA5Q4Nuy%2Fdevices%2Fdev1&et=1908107045&method=md5&sign=RP9dvBouzP%2BLlEu66ozG2g%3D%3D");
    ui->comboBox_Token->addItem("version=2018-10-31&res=products%2FFqrA5Q4Nuy%2Fdevices%2Fdev2&et=1908107045&method=md5&sign=8PjKVuxUpBhNi6WaakZQkw%3D%3D");
    ui->version_check->setChecked(1);
    ui->spinBox_60->setValue(60);
    ui->radioButton_0->setChecked(1);
    ui->pushButton_disconnect->setEnabled(false);

    ui->allPublished->appendPlainText("示例：");
    ui->allPublished->appendPlainText("{\"id\":123,\"dp\":{");
    ui->allPublished->appendPlainText("\"temperatrue\":[{\"v\":30,\"t\":1552289676}],");
    ui->allPublished->appendPlainText("\"power\":[{\"v\":4.5,\"t\":1552289676}],");
    ui->allPublished->appendPlainText("\"status\":[{\"v\":{\"color\":\"blue\"},\"t\":1552289677},{\"v\":{\"color\":\"red\"},\"t\":1552289678}]");
    ui->allPublished->appendPlainText("}}");
    ui->allPublished->appendPlainText("其他说明：");
    ui->allPublished->appendPlainText("id类型为int，必填，消息ID，大于0");
    ui->allPublished->appendPlainText("dp类型为object，必填，数据点内容，key-value格式，key为数据流名称，value为list格式的一个或多个数据点值");
    ui->allPublished->appendPlainText("v无类型，必填，数据点值，可以为int/float/string/object多种格式");
    ui->allPublished->appendPlainText("t类型为int，选填，数据点时间戳，unix时间，单位为秒，若不携带，则系统将以数据到达平台时间作为其数据时间");
    m_client = new QMqttClient(this);
    ui->comboBox_topic->addItem("$sys/FqrA5Q4Nuy/dev1/dp/post/json");//设备上传数据点,QoS0或1，仅可发布
    ui->comboBox_topic->addItem("$sys/FqrA5Q4Nuy/dev2/dp/post/json");

    ui->topic_2->addItem("$sys/FqrA5Q4Nuy/dev1/dp/post/json/+");
    ui->topic_2->addItem("$sys/FqrA5Q4Nuy/dev1/dp/post/json/accepted");//系统通知"设备上传数据点成功"	,QoS0，仅可订阅
    ui->topic_2->addItem("$sys/FqrA5Q4Nuy/dev1/dp/post/json/rejected");//系统通知"设备上传数据点失败",QoS0，仅可订阅
    ui->topic_2->addItem("$sys/FqrA5Q4Nuy/dev1/cmd/#");
    ui->topic_2->addItem("$sys/FqrA5Q4Nuy/dev2/dp/post/json/+");
    ui->topic_2->addItem("$sys/FqrA5Q4Nuy/dev2/dp/post/json/accepted");//系统通知"设备上传数据点成功"	,QoS0，仅可订阅
    ui->topic_2->addItem("$sys/FqrA5Q4Nuy/dev2/dp/post/json/rejected");//系统通知"设备上传数据点失败",QoS0，仅可订阅
    ui->topic_2->addItem("$sys/FqrA5Q4Nuy/dev2/cmd/#");

    connect(this->ui->pushButton_connect,SIGNAL(clicked()),this,SLOT(on_btn_connect_clicked()));//连接按钮
    connect(this->ui->pushButton_publish,SIGNAL(clicked()),this,SLOT(on_btn_publish_click()));//发布按钮
    connect(this->ui->pushButton_ping,SIGNAL(clicked()),this,SLOT(on_btn_ping_click()));//Ping按钮
    connect(this->ui->pushButton_disconnect,SIGNAL(clicked()),this,SLOT(brokenDisconnect()));//断开连接按钮
    connect(this->ui->clear_send,SIGNAL(clicked()),this,SLOT(on_btn_clr_send_buf_click()));//清空发送区
    connect(this->ui->clear_receive,SIGNAL(clicked()),this,SLOT(on_btn_clr_recv_buf_click()));//清空接收区
    connect(this->ui->CancelAll,SIGNAL(clicked()),this,SLOT(on_btn_unsub_all_click()));//取消全部订阅
    connect(this->ui->output2file,SIGNAL(clicked()),this,SLOT(on_btn_exp2file_click()));//导出到文件
    connect(this->ui->CancelOne,SIGNAL(clicked()),this,SLOT(on_btn_unsubtopic_click()));//取消订阅
    connect(this->ui->showTopic,SIGNAL(clicked()),this,SLOT(on_btn_showtopic_click));//显示主题
    connect(this->ui->showTime,SIGNAL(clicked()),this,SLOT(on_btn_showtime_click));//显示时间
    connect(this->ui->chooseTopic,SIGNAL(clicked()),this,SLOT(on_btn_subtopic_click()));//订阅主题

    connect(m_client, SIGNAL(disconnected()),this,SLOT(brokenDisconnect()));
    connect(m_client, SIGNAL(pingResponseReceived()),this,SLOT(pingResponseReceived()));
    //接收消息
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(" Received Topic: ")
                    + topic.name()
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');
        ui->subReceive->appendPlainText(content);
    });

    mp_client_Socket=new QTcpSocket;
    this->ui->ip_addr_input->setText("192.168.1.114");
    //ip_addr="192.168.1.114";
    this->ui->port_input->setText("8888");
    //port=8888;
    this->ui->disconnect_socket->setEnabled(false);
    this->ui->sendContent->setText("TH");

    connect(mp_client_Socket,SIGNAL(connected()),this,SLOT(connectedInfo()));
    connect(mp_client_Socket,SIGNAL(disconnected()),this,SLOT(disconnectedInfo()));
    connect(mp_client_Socket,&QIODevice::readyRead,[=](){
        this->ui->cmd->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        //char*data=new char[1024];
        mp_client_Socket->readLine(receive_Buffer,1024);
        QStringList temp_list=QString(receive_Buffer).split("-");
        temp_list.removeAll("");
        for(int i=0;i<temp_list.size();i++)ui->cmd->appendPlainText(temp_list[i]);
    });
    connect(this->ui->send,SIGNAL(clicked()),this,SLOT(ClientSendData()));
    connect(this->ui->connect,SIGNAL(clicked()),this,SLOT(connect_btn()));
    connect(this->ui->disconnect_socket,SIGNAL(clicked()),this,SLOT(disconnect_btn()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//以下为槽函数

//按下连接按钮
void MainWindow::on_btn_connect_clicked(void){
    if(this->ui->lineEdit_addr->text().isEmpty()&&this->ui->lineEdit_ip->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Error"),tr("域名和IP地址必须填一个！"));
        return;
    }
    if(this->ui->checkBox_port->isChecked()!=true){
        QMessageBox::critical(this,tr("Error"),tr("请选择端口！"));
        return;
    }
    hostPort=this->ui->checkBox_port->text().toInt();
    userName=this->ui->lineEdit_productID->text();
    passWord=this->ui->comboBox_Token->currentText();
    if(this->ui->spinBox_60->value()<=10){
        QMessageBox::critical(this,tr("Error"),tr("KeepAlive太小！"));
        return;
    }
    keepAlive=this->ui->spinBox_60->value();
    if(this->ui->version_check->isChecked()!=true){
        QMessageBox::critical(this,tr("Error"),tr("请选择协议版本！"));
        return;
    }
    if(this->ui->radioButton_0->isChecked()!=true&&this->ui->radioButton_1->isChecked()!=true){
        QMessageBox::critical(this,tr("Error"),tr("请选择QoS！"));
        return;
    }

    if(this->ui->lineEdit_ip->text().isEmpty()){
        //如果写了域名没写IP地址
        pressAddress(this->ui->lineEdit_addr->text());
    }else{
        if(m_client->state()==QMqttClient::Disconnected){
            m_client->setHostname(ui->lineEdit_ip->text());
            m_client->setPort(hostPort);
            m_client->setClientId(clientId);
            m_client->setUsername(userName);
            m_client->setPassword(passWord);
            m_client->setKeepAlive(keepAlive);
            if(this->ui->version_check->text()=="MQTT 3.1.1"){
                m_client->setProtocolVersion(QMqttClient::MQTT_3_1_1);
            }
            if(this->ui->radioButton_0->isChecked())
                m_client->setWillQoS(0);
            else if(this->ui->radioButton_1->isChecked()){
                m_client->setWillQoS(1);
            }
            m_client->connectToHost();

            this->ui->lineEdit_addr->setEnabled(false);
            this->ui->lineEdit_ip->setEnabled(false);
            this->ui->checkBox_port->setEnabled(false);
            this->ui->comboBox_clientId->setEnabled(false);
            this->ui->lineEdit_productID->setEnabled(false);
            this->ui->comboBox_Token->setEnabled(false);
            this->ui->spinBox_60->setEnabled(false);
            this->ui->version_check->setEnabled(false);
            this->ui->radioButton_0->setEnabled(false);
            this->ui->radioButton_1->setEnabled(false);
            this->ui->pushButton_connect->setEnabled(false);
            this->ui->pushButton_disconnect->setEnabled(true);
        }
    }
}
//按下清空发送区
void MainWindow::on_btn_clr_send_buf_click(void)
{
    qDebug() << "on_btn_clr_send_buf_click";
    ui->linePublish->clear();
}
//按下导出到文件
void MainWindow::on_btn_exp2file_click(void)
{
    //暂未启用
    qDebug() << "on_btn_exp2file_click";
    QString path = QFileDialog::getSaveFileName(this,tr("保存路径"),".",tr("Text Files(*.txt)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,tr("Error"),tr("Save Failed!"));
            return;
        }
        QTextStream out(&file);
        out << ui->allPublished->toPlainText();
        file.close();
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("未选择路径！"));
    }
}
//显示时间
void MainWindow::on_btn_showtime_click(void)
{
    qDebug() << "on_btn_showtime_click";
}
//显示主题
void MainWindow::on_btn_showtopic_click(void)
{
    qDebug() << "on_btn_showtopic_click";
}
//按下发布按钮
void MainWindow::on_btn_publish_click(void)
{
    qDebug() << "on_btn_publish_click";
    //{"id":123,"dp":{"temp1":[{"v":30}]}}
    //{"id":123,"dp":{"temp1":[{"v":30}],"temp2":[{"v":29}]}}
    QString topic = this->ui->comboBox_topic->currentText();
    QString payload = this->ui->linePublish->toPlainText();
    int QoS;
    if(this->ui->radioButton_0->isChecked()){
        QoS=0;
    }else if(this->ui->radioButton_1->isChecked()){
        QoS=1;
    }

    if(m_client->publish(topic,payload.toUtf8(),QoS) == -1)
    {
        this->ui->allPublished->appendPlainText("public error");
    }else{
        this->ui->allPublished->appendPlainText("publish successfully,waiting receive...");
    }
    this->ui->allPublished->appendPlainText(payload);
}
//按下ping按钮
void MainWindow::on_btn_ping_click(void)
{
    ui->pushButton_ping->setEnabled(false);
    m_client->requestPing();
}

void MainWindow::on_btn_clr_recv_buf_click(void){
    this->ui->subReceive->clear();
}

void MainWindow::on_btn_unsub_all_click(void){
    while(this->ui->AlreadySubscribed->count()){
        QListWidgetItem*item_sel=this->ui->AlreadySubscribed->item(0);
        MyWidgetItem *cur_item =dynamic_cast<MyWidgetItem *>(ui->AlreadySubscribed->itemWidget(item_sel));
        m_client->unsubscribe(cur_item->text->text());
        this->ui->AlreadySubscribed->takeItem(0);
    }
    QMessageBox::information(this,tr("提示"),tr("取消订阅成功"));
}

void MainWindow::on_btn_unsubtopic_click(void){
    QString topic=ui->topic_2->currentText();
    m_client->unsubscribe(topic);
    for(int i=0;i<ui->AlreadySubscribed->count();i++){
        QListWidgetItem*item_sel=ui->AlreadySubscribed->item(i);
        MyWidgetItem*cur_item=dynamic_cast<MyWidgetItem*>(ui->AlreadySubscribed->itemWidget(item_sel));
        if(cur_item->text->text()==topic){
            ui->AlreadySubscribed->takeItem(i);
        }
    }
}

void MainWindow::on_btn_subtopic_click(void){
    QString topic=ui->topic_2->currentText();
    for(int i=0;i<ui->AlreadySubscribed->count();i++){
        QListWidgetItem*item_sel=ui->AlreadySubscribed->item(i);
        MyWidgetItem*cur_item=dynamic_cast<MyWidgetItem *>(ui->AlreadySubscribed->itemWidget(item_sel));
        if(cur_item->text->text() == topic ){
            QMessageBox::critical(this,tr("Error"),tr("主题：%1已经订阅").arg(topic));
            return;
        }
    }

    //后端代码
    auto subscription = m_client->subscribe(topic);//连接云端并订阅

    if(!subscription)
    {
        QMessageBox::critical(this,tr("Error"),tr("订阅失败！请查看是否连接？"));
        return;
    }
    MyWidgetItem *myitem = new MyWidgetItem;
    myitem->text->setText(ui->topic_2->currentText());
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(0,30));
    ui->AlreadySubscribed->addItem(item);
    ui->AlreadySubscribed->setItemWidget(item,myitem);
    connect(myitem->btn_del,&QPushButton::clicked,[this](){
        int item_n=ui->AlreadySubscribed->currentRow();
        if(item_n<0)return;
        QListWidgetItem*item_sel=ui->AlreadySubscribed->item(item_n);
        MyWidgetItem*cur_item=dynamic_cast<MyWidgetItem *>(ui->AlreadySubscribed->itemWidget(item_sel));
        m_client->unsubscribe(cur_item->text->text());//取消订阅的按钮
        ui->AlreadySubscribed->takeItem(item_n);
    });
}

void MainWindow::updateLogStateChange(){
    const QString content=QDateTime::currentDateTime().toString()
            +QLatin1String(":State Change")
            +QString::number(m_client->state())
            +QLatin1Char('\n');
    ui->allPublished->appendPlainText(content);
}
//按下断开按钮
void MainWindow::brokenDisconnect(void)
{
    this->ui->lineEdit_addr->setEnabled(true);
    this->ui->lineEdit_ip->setEnabled(true);
    this->ui->checkBox_port->setEnabled(true);
    this->ui->comboBox_clientId->setEnabled(true);
    this->ui->lineEdit_productID->setEnabled(true);
    this->ui->comboBox_Token->setEnabled(true);
    this->ui->spinBox_60->setEnabled(true);
    this->ui->version_check->setEnabled(true);
    this->ui->radioButton_0->setEnabled(true);
    this->ui->radioButton_1->setEnabled(true);
    this->ui->pushButton_connect->setEnabled(true);
    this->ui->pushButton_disconnect->setEnabled(false);
    m_client->disconnectFromHost();
}

void MainWindow::topicMessageReceived(const QByteArray &message,const QMqttTopicName &topic){
    QString content;
    if(ui->showTime->isChecked()){
        content=QDateTime::currentDateTime().toString();
    }
    if(ui->showTopic->isChecked()){
        content+=QLatin1String(" Received Topic: ")+topic.name();
    }
    content+=QLatin1String(" Message: ")+message+QLatin1Char('\n');
    ui->allPublished->appendPlainText(content);
}
//接收到ping的返回消息
void MainWindow::pingResponseReceived(void)
{
    ui->pushButton_ping->setEnabled(true);
    const QString content = QDateTime::currentDateTime().toString()
            +QLatin1String(" PingReponse")
            +QLatin1Char('\n');
    ui->allPublished->appendPlainText(content);
}
//根据域名推IP地址
void MainWindow::pressAddress(QString addr)
{
    QHostInfo::lookupHost(addr,this,SLOT(lookedup(QHostInfo)));

}
//查阅IP地址
void MainWindow::lookedup(const QHostInfo &info)
{
    if (info.error() != QHostInfo::NoError)
    {
         qDebug() << "Lookup failed:" << info.errorString();
         return;
    }
    hostName=info.addresses()[0].toString();
    this->ui->lineEdit_ip->setText(hostName);
    if(m_client->state()==QMqttClient::Disconnected){
        m_client->setHostname(hostName);
        m_client->setPort(ui->checkBox_port->text().toInt());
        m_client->setClientId(ui->comboBox_clientId->currentText());
        m_client->setUsername(ui->lineEdit_productID->text());
        m_client->setPassword(ui->comboBox_Token->currentText());
        m_client->setKeepAlive(ui->spinBox_60->value());
        if(this->ui->version_check->text()=="MQTT 3.1.1"){
            m_client->setProtocolVersion(QMqttClient::MQTT_3_1_1);
        }
        if(this->ui->radioButton_0->isChecked())
            m_client->setWillQoS(0);
        else if(this->ui->radioButton_1->isChecked()){
            m_client->setWillQoS(1);
        }

        m_client->connectToHost();
        this->ui->lineEdit_addr->setEnabled(false);
        this->ui->lineEdit_ip->setEnabled(false);
        this->ui->checkBox_port->setEnabled(false);
        this->ui->comboBox_clientId->setEnabled(false);
        this->ui->lineEdit_productID->setEnabled(false);
        this->ui->comboBox_Token->setEnabled(false);
        this->ui->spinBox_60->setEnabled(false);
        this->ui->version_check->setEnabled(false);
        this->ui->radioButton_0->setEnabled(false);
        this->ui->radioButton_1->setEnabled(false);
        this->ui->pushButton_connect->setEnabled(false);
        this->ui->pushButton_disconnect->setEnabled(true);
    }
}



void MainWindow::connect_btn(){
    ip_addr=ui->ip_addr_input->text();
    port=ui->port_input->text().toUInt();
    mp_client_Socket->connectToHost(ip_addr,port);
    ui->cmd->appendPlainText("try connecting...");
}
void MainWindow::disconnect_btn(){
    mp_client_Socket->disconnectFromHost();
    //ui->cmd->appendPlainText("disconnecting from server...");
}
void MainWindow::connectedInfo(){
    ui->cmd->appendPlainText("already connecting to server!");
    ui->connect->setEnabled(false);
    ui->disconnect_socket->setEnabled(true);
}
void MainWindow::disconnectedInfo(){
    ui->cmd->appendPlainText("already disconnected from server!");
    ui->connect->setEnabled(true);
    ui->disconnect_socket->setEnabled(false);
}
void MainWindow::ClientReadData(){
    qDebug()<<"readyRead() is triggered!";
    return;
}
void MainWindow::ClientSendData(){

    ui->cmd->appendPlainText("sending message...");
    ui->cmd->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    const char*send_content=this->ui->sendContent->text().toStdString().c_str();
    qint64 content_len=(qint64)this->ui->sendContent->text().length();
    mp_client_Socket->write(send_content,content_len);
    mp_client_Socket->flush();
    mp_client_Socket->waitForBytesWritten();
    return;
}
