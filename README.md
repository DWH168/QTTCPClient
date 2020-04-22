## 基于QT的TCP通讯TCPClient服务端C++ 代码开源
本文是TCP客户端,如需服务端请点击[基于QT的TCP服务端](https://blog.csdn.net/dwh1314/article/details/105679844)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200422164449639.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R3aDEzMTQ=,size_16,color_FFFFFF,t_70)
Qt是一种基于C++的跨平台图形用户界面应用程序开发框架
本次使用QT的版本是5.6

TCP旨在适应支持多网络应用的分层协议层次结构。 连接到不同但互连的计算机通信网络的主计算机中的成对进程之间依靠TCP提供可靠的通信服务。TCP假设它可以从较低级别的协议获得简单的，可能不可靠的数据报服务。 原则上，TCP应该能够在从硬线连接到分组交换或电路交换网络的各种通信系统之上操作。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020042216460048.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R3aDEzMTQ=,size_16,color_FFFFFF,t_70)
可以实现16进制的接收显示以及16进制的发送。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200422164849802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R3aDEzMTQ=,size_16,color_FFFFFF,t_70)

```cpp
 connect(tcpClient,&QTcpSocket::readyRead,[=](){
        //读数据
        QByteArray buffer = tcpClient->readAll();

        //检测是否是HEX显示
        if(ui->ckb_hex_show->isChecked())
        {
            QString ret(buffer.toHex().toUpper());
            int len = ret.length()/2;
            for(int j=1;j<len;j++)
            {
                qDebug()<<j;
                ret.insert(2*j+j-1," ");
            }
            ui->re_edit->append(ret);
        }else{
             ui->re_edit->append(QString::fromLocal8Bit(buffer));
        }

    });

    connect(tcpClient,&QTcpSocket::disconnected,[=](){
        tcpClient->disconnectFromHost();
        ui->btn_connect->setText(tr("连接"));
        ui->send_btn->setEnabled(false);
        ui->re_edit->append(QString("已断开连接"));
    });
```

```cpp
/**
 *发送数据用 可发HEX数据
 * @brief TcpServer::sendData
 * @param client
 * @param data
 */
void  TcpClient::sendData(QTcpSocket *client,QString data){
        //如果发送HEX的选择框选中 那么就是需要发送HEX数据
        if(ui->ckb_hex_send->isChecked())
        {
            bool ok;
            QByteArray ret;
            data = data.trimmed();
            data = data.simplified();
            QStringList sl = data.split(" ");

            foreach (QString s, sl) {
                if(!s.isEmpty())
                {
                    char c = s.toInt(&ok,16)&0xFF;
                    if(ok){
                        ret.append(c);
                    }else{
                        QMessageBox::warning(0,tr("错误："),QString("非法的16进制字符: \"%1\"").arg(s));
                    }
                }
            }
            client->write(ret);
        }
        //直接发送字符串
        else{
            client->write(data.toLocal8Bit());
        }
}

```
需要更多资料可以关注微信公众号:**智慧小巷**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200326102352938.jpg)

