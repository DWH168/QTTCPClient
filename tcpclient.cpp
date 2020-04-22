#include "tcpclient.h"
#include "ui_tcpclient.h"
/**
  *微信公众号：智慧小巷
  *可获取更多资料哦
  *
  */
TcpClient::TcpClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    ui->send_btn->setEnabled(false);
    ui->ip_edit->setText("192.168.0.115");
    ui->prot_edit->setText("777");

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

}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::on_btn_connect_clicked()
{
    if(ui->btn_connect->text()=="连接")
    {
        if(ui->ip_edit->text().isEmpty())
        {
            QMessageBox::warning(0,tr("错误："),QString("连接IP不能为空"));
            return ;
        }
        if(ui->prot_edit->text().isEmpty())
        {
             QMessageBox::warning(0,tr("错误："),QString("连接端口不能为空"));
             return ;
        }

        tcpClient->connectToHost(ui->ip_edit->text(), ui->prot_edit->text().toInt());
        if (tcpClient->waitForConnected(1000))  // 连接成功则进入if{}
        {
            ui->btn_connect->setText("断开");
            ui->send_btn->setEnabled(true);
            ui->re_edit->append(QString("已连接到[%1:%2]").arg(ui->ip_edit->text()).arg(ui->prot_edit->text()));
        }
        else{
            QMessageBox::warning(0,tr("错误："),QString("连接失败"));
            return ;
        }

    }
    else
    {
        tcpClient->disconnectFromHost();
        if (tcpClient->state() == QAbstractSocket::UnconnectedState \
                || tcpClient->waitForDisconnected(1000))  //已断开连接则进入if{}
        {
            ui->btn_connect->setText("连接");
            ui->send_btn->setEnabled(false);
        }
    }
}

void TcpClient::on_send_btn_clicked()
{
    QString data = ui->send_edit->toPlainText();
     // 文本输入框为空时直接退出无需发送
    if(data == "")
    {
        QMessageBox::warning(0,tr("错误："),QString("请输入发送内容"));
        return;
    }
    sendData(tcpClient,data);
}
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

void TcpClient::on_clear_btn_clicked()
{
    ui->re_edit->clear();
}
