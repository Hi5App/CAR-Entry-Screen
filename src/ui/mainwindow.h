#pragma once

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void setAccountBtnIcon(QPixmap pixmap);

private slots:
    void on_m_CollaborationBtn_clicked();

    void on_m_DBClientBtn_clicked();

private:
    Ui::MainWindow *ui;
//    QTimer* m_HeartBeatTimer;
//    QTimer* m_OnlineStatusTimer;

//    QSplitter* m_Splitter;

};
