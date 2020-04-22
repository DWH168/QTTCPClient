#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
namespace Ui {
class TcpClient;
}

class TcpClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();

private slots:
    void on_btn_connect_clicked();

    void on_send_btn_clicked();

    void on_clear_btn_clicked();

private:
    Ui::TcpClient *ui;
    void  TcpClient::sendData(QTcpSocket *client,QString data);
    QTcpSocket *tcpClient;

};

#endif // TCPCLIENT_H
