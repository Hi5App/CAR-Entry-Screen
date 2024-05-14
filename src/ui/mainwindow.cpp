#include <QMessageBox>
#include <QTimer>
#include <QMenu>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../framework/defination/ImageDefination.h"
#include <QTimer>
#include <future>

#include "../framework/defination/ImageDefination.h"
#include "../framework/service/CachedUserData.h"
#include "../framework/service/HttpClient.h"
#include "../framework/config/AppConfig.h"
#include "../framework/defination/TypeDef.h"
#include "json.hpp"
#include "editorusersettings.h"

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->size());

    auto rawHeadPhotoBinData = CachedUserData::getInstance().CachedUserMetaInfo.HeadPhotoBinData;
    QByteArray byteArray(reinterpret_cast<const char*>(rawHeadPhotoBinData.c_str()), rawHeadPhotoBinData.length());
    // Decode QByteArray from Base64
    QByteArray decodedData = QByteArray::fromBase64(byteArray);
    //        auto data = QByteArray::fromStdString(rawdata);

//    auto headPhotoBinData = QByteArray::fromStdString(rawHeadPhotoBinData);

//    connect(m_UserSettingBtn, &QPushButton::clicked, this, [this](bool checked) {
//        EditorUserSettings editorUserSettings(this);
//        editorUserSettings.exec();
//    });

    ui->m_AccountBtn->setPopupMode(QToolButton::InstantPopup);
    ui->m_AccountBtn->setText("Account");
    ui->m_AccountBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->m_AccountBtn->setIconSize({32, 32});

    ui->iconLabel->setPixmap(QPixmap::fromImage(QImage(Image::ImageAppBg).scaled({83,74})));

    if (decodedData.size() > 0) {
        QPixmap pixmap;
        pixmap.loadFromData(decodedData);
        pixmap = pixmap.scaled(QSize(128, 128), Qt::KeepAspectRatioByExpanding);
        ui->m_AccountBtn->setIcon(pixmap);
    }
    else {
        ui->m_AccountBtn->setIcon(QIcon(Image::ImageDefaultUserHeadPhoto));
    }

    // 创建下拉菜单
    QMenu *menu = new QMenu(ui->m_AccountBtn);

    auto* logoutAction = new QAction(this);
    logoutAction->setText("Logout");
    connect(logoutAction, &QAction::triggered, this, [this](bool checked) {
        auto btn = QMessageBox::information(this, "Warning",
                                            "Are you sure to logout? Please save all your work before logout!",
                                            QMessageBox::StandardButton::Ok,
                                            QMessageBox::StandardButton::Cancel);
        if (btn == QMessageBox::StandardButton::Ok) {
            auto json = HttpClient::getInstance().userLogout(this);
            if (!json.empty()) {
                AppConfig::getInstance().setSecurityConfig(AppConfig::SecurityConfigItem::eCachedUserName, "");
                AppConfig::getInstance().setSecurityConfig(AppConfig::SecurityConfigItem::eCachedPassword, "");
                AppConfig::getInstance().setSecurityConfig(AppConfig::SecurityConfigItem::eAccountExpiredTime, "");
                AppConfig::getInstance().writeSecurityConfig();
                qApp->exit(RestartCode);
            }
        }
    });
//    ui->m_AccountBtn->addAction(logoutAction);

    auto* editInfoAction = new QAction(this);
    editInfoAction->setText("Edit Info");
    connect(editInfoAction, &QAction::triggered, this, [this](bool checked) {
        EditorUserSettings editorUserSettings(this);
        editorUserSettings.exec();
    });
//    ui->m_AccountBtn->addAction(editInfoAction);

    menu->addAction(logoutAction);
    menu->addAction(editInfoAction);
    menu->setStyleSheet("QMenu::item { color: rgb(85,85,85); } QMenu { background-color: rgb(245,245,245); }");
//    this->setStyleSheet("QAction { color: rgb(85,85,85); }");

    ui->m_AccountBtn->setMenu(menu);

//    connect(ui->m_CollaborationBtn, &QPushButton::clicked, this, &MainWindow::on_m_CollaborationBtn_clicked);
//    connect(ui->m_DBClientBtn, &QPushButton::clicked, this, &MainWindow::on_m_DBClientBtn_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setAccountBtnIcon(QPixmap pixmap){
    ui->m_AccountBtn->setIcon(pixmap);
}

void MainWindow::on_m_CollaborationBtn_clicked()
{
    char formattedString[256];
    const char* programDirPath = (QCoreApplication::applicationDirPath().toStdString() + "/thirdparty/CAR-WS-VR").c_str();
    const char* programName = "vaa3d_CAR.exe";

    snprintf(formattedString, sizeof(formattedString), R"(start "" "%s/%s")", programDirPath, programName);

    std::future<int> resultFuture = std::async(std::launch::async, [&formattedString]() {
        return system(formattedString);
    });

    int result = resultFuture.get();
    if(result == 0){
        std::cout << "Application exited successfully." << std::endl;
    } else{
        std::cout << "Failed to start or application exited with an error." << std::endl;
    }
}


void MainWindow::on_m_DBClientBtn_clicked()
{
    char formattedString[256];
    const char* programDirPath = (QCoreApplication::applicationDirPath().toStdString() + "/thirdparty/SwcDbmsQtGuiClient_Win").c_str();
    const char* programName = "SwcManagerClient.exe";

    snprintf(formattedString, sizeof(formattedString), R"(start "" "%s/%s")", programDirPath, programName);

    std::future<int> resultFuture = std::async(std::launch::async, [&formattedString]() {
        return system(formattedString);
    });

    int result = resultFuture.get();
    if(result == 0){
        std::cout << "Application exited successfully." << std::endl;
    } else{
        std::cout << "Failed to start or application exited with an error." << std::endl;
    }
}

