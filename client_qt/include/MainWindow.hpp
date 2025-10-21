#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <vector>
#include "TcpClient.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onConnectClicked();
    void onAddClicked();
    void onGetClicked();
    void onClearClicked();

    void onConnected();
    void onDisconnected();
    void onInfo(const QString&);
    void onError(const QString&);
    void onReplyOk(const QString&);
    void onReplyError(const QString&);
    void onGotSorted(const std::vector<int>& vals);

private:
    std::vector<int> parseInput(const QString& text);
    QString joinInts(const std::vector<int>& v);

    // UI
    QLineEdit* ipEdit_{};
    QLineEdit* portEdit_{};
    QPushButton* connectBtn_{};

    QLineEdit* inputEdit_{};
    QPushButton* addBtn_{};
    QPushButton* getBtn_{};
    QPushButton* clearBtn_{};

    QPlainTextEdit* outputEdit_{};
    QLabel* statusLabel_{};

    // Net
    TcpClient* client_{};
};
