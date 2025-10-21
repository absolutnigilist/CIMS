#include "MainWindow.hpp"
#include <QGridLayout>
#include <QMessageBox>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), client_(new TcpClient(this)) {
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* grid = new QGridLayout(central);
    int row = 0;

    // Network panel
    grid->addWidget(new QLabel("Server IP:"), row, 0);
    ipEdit_ = new QLineEdit("127.0.0.1");
    grid->addWidget(ipEdit_, row, 1);

    grid->addWidget(new QLabel("Port:"), row, 2);
    portEdit_ = new QLineEdit("12345");
    portEdit_->setFixedWidth(90);
    grid->addWidget(portEdit_, row, 3);

    connectBtn_ = new QPushButton("Connect");
    grid->addWidget(connectBtn_, row, 4);
    ++row;

    // IO panel
    grid->addWidget(new QLabel("Input array (e.g. 5, 2, 8, 1):"), row, 0, 1, 2);
    inputEdit_ = new QLineEdit();
    grid->addWidget(inputEdit_, row, 2, 1, 3);
    ++row;

    addBtn_ = new QPushButton("Add to queue");
    getBtn_ = new QPushButton("Get sorted");
    clearBtn_ = new QPushButton("Clear queue");
    grid->addWidget(addBtn_, row, 0, 1, 2);
    grid->addWidget(getBtn_, row, 2, 1, 2);
    grid->addWidget(clearBtn_, row, 4);
    ++row;

    grid->addWidget(new QLabel("Sorted output:"), row, 0, 1, 2);
    outputEdit_ = new QPlainTextEdit();
    outputEdit_->setReadOnly(true);
    outputEdit_->setMinimumHeight(130);
    grid->addWidget(outputEdit_, row, 2, 1, 3);
    ++row;

    statusLabel_ = new QLabel("Disconnected");
    grid->addWidget(statusLabel_, row, 0, 1, 5);

    // Wire signals
    connect(connectBtn_, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(addBtn_, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(getBtn_, &QPushButton::clicked, this, &MainWindow::onGetClicked);
    connect(clearBtn_, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    connect(client_, &TcpClient::connected, this, &MainWindow::onConnected);
    connect(client_, &TcpClient::disconnected, this, &MainWindow::onDisconnected);
    connect(client_, &TcpClient::info, this, &MainWindow::onInfo);
    connect(client_, &TcpClient::error, this, &MainWindow::onError);
    connect(client_, &TcpClient::replyOk, this, &MainWindow::onReplyOk);
    connect(client_, &TcpClient::replyError, this, &MainWindow::onReplyError);
    connect(client_, &TcpClient::gotSortedValues, this, &MainWindow::onGotSorted);

    setWindowTitle("Queue Client (Qt)");
    resize(780, 380);
}

void MainWindow::onConnectClicked() {
    if (client_->isConnected()) { client_->disconnectFromHost(); return; }
    bool ok = false; quint16 port = portEdit_->text().toUShort(&ok);
    if (!ok) { QMessageBox::warning(this, "Port", "Invalid port"); return; }
    client_->connectTo(ipEdit_->text(), port);
}

void MainWindow::onAddClicked() {
    const auto vals = parseInput(inputEdit_->text());
    if (vals.empty()) { QMessageBox::information(this, "Input", "Enter at least one integer"); return; }
    client_->sendAdd(vals);
}
void MainWindow::onGetClicked() { client_->sendGet(); }
void MainWindow::onClearClicked() { client_->sendClear(); }

void MainWindow::onConnected() {
    statusLabel_->setText("Connected");
    connectBtn_->setText("Disconnect");
}
void MainWindow::onDisconnected() {
    statusLabel_->setText("Disconnected");
    connectBtn_->setText("Connect");
}
void MainWindow::onInfo(const QString& s) { statusLabel_->setText(s); }
void MainWindow::onError(const QString& s) { statusLabel_->setText("Error: " + s); }
void MainWindow::onReplyOk(const QString& s) { statusLabel_->setText("OK: " + s); }
void MainWindow::onReplyError(const QString& s) { statusLabel_->setText("Server error: " + s); }

void MainWindow::onGotSorted(const std::vector<int>& vals) {
    outputEdit_->setPlainText(joinInts(vals));
}

std::vector<int> MainWindow::parseInput(const QString& text) {
    std::vector<int> out;
    QString tok; tok.reserve(text.size());
    for (QChar ch : text) {
        if (ch.isDigit() || ch == '-') tok.append(ch);
        else if (ch == ',' || ch.isSpace()) {
            if (!tok.isEmpty()) { out.push_back(tok.toInt()); tok.clear(); }
        } // прочие символы игнорируем
    }
    if (!tok.isEmpty()) out.push_back(tok.toInt());
    return out;
}

QString MainWindow::joinInts(const std::vector<int>& v) {
    QStringList parts; parts.reserve(int(v.size()));
    for (int n : v) parts << QString::number(n);
    return parts.join(", ");
}
