#include "TcpClient.hpp"
#include "Json.hpp"

TcpClient::TcpClient(QObject* parent) : QObject(parent) {
    connect(&socket_, &QTcpSocket::connected, this, &TcpClient::connected);
    connect(&socket_, &QTcpSocket::disconnected, this, &TcpClient::disconnected);
    connect(&socket_, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(&socket_, &QTcpSocket::errorOccurred, this, &TcpClient::onErrorOccurred);
}

void TcpClient::connectTo(const QString& host, quint16 port) {
    if (isConnected()) socket_.disconnectFromHost();
    socket_.connectToHost(host, port);
}

void TcpClient::disconnectFromHost() {
    socket_.disconnectFromHost();
}

void TcpClient::sendAdd(const std::vector<int>& vals) {
    if (!isConnected()) { emit error("Not connected"); return; }
    socket_.write(Proto::makeAdd(vals));
}

void TcpClient::sendGet() {
    if (!isConnected()) { emit error("Not connected"); return; }
    socket_.write(Proto::makeGet());
}

void TcpClient::sendClear() {
    if (!isConnected()) { emit error("Not connected"); return; }
    socket_.write(Proto::makeClear());
}

void TcpClient::onReadyRead() {
    recvBuf_ += socket_.readAll();
    int idx;
    while ((idx = recvBuf_.indexOf('\n')) != -1) {
        const QByteArray line = recvBuf_.left(idx);
        recvBuf_.remove(0, idx + 1);

        Proto::Reply r;
        if (!Proto::parseReplyLine(line, r)) {
            emit error(QStringLiteral("Bad JSON from server: ") + QString::fromUtf8(line));
            continue;
        }
        if (r.status == "ok") {
            emit replyOk(r.message);
            if (!r.values.empty()) emit gotSortedValues(r.values);
        }
        else {
            emit replyError(r.message);
        }
    }
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError) {
    emit error(socket_.errorString());
}
