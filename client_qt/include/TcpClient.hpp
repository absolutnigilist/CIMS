#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <vector>

class TcpClient : public QObject {
    Q_OBJECT
public:
    explicit TcpClient(QObject* parent = nullptr);

    void connectTo(const QString& host, quint16 port);
    void disconnectFromHost();

    bool isConnected() const { return socket_.state() == QAbstractSocket::ConnectedState; }

    void sendAdd(const std::vector<int>& vals);
    void sendGet();
    void sendClear();

signals:
    void connected();
    void disconnected();
    void info(const QString& msg);
    void error(const QString& msg);
    void replyOk(const QString& msg);
    void replyError(const QString& msg);
    void gotSortedValues(const std::vector<int>& vals);

private slots:
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError);

private:
    QTcpSocket socket_;
    QByteArray recvBuf_; // аккумулируем до '\n'
};
