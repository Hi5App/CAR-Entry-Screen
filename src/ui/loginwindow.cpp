#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include "registerwindow.h"
#include "../framework/config/AppConfig.h"
#include <QTimer>
#include <QTimerEvent>

#include "../framework/defination/ImageDefination.h"
#include "../framework/service/CachedUserData.h"
#include "../framework/service/HttpClient.h"
#include "json.hpp"
#include <QSettings>

using json = nlohmann::json;

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->size());
//    this->setStyleSheet("background-color: rgb(1,19,35);");

    connect(ui->loginBtn,&QPushButton::clicked, this, &LoginWindow::onLoginBtnClicked);
    connect(ui->registerBtn, &QPushButton::clicked, this, &LoginWindow::onRegisterBtnClicked);

    ui->label->setPixmap(QPixmap::fromImage(QImage(Image::ImageUser).scaled({42,42})));
    ui->label_2->setPixmap(QPixmap::fromImage(QImage(Image::ImagePassword).scaled({40,40})));
    ui->iconLabel->setPixmap(QPixmap::fromImage(QImage(Image::ImageAppBg).scaled({83,74})));

    ui->passwordEditor->setEchoMode(QLineEdit::Password);

    QFont font;
    font.setPointSize(14);
    ui->userNameEditor->setFont(font);

    static QTimer timer;
    connect(&timer,&QTimer::timeout,this,[this]() {
        verifyCachedAccount();
    });
    timer.setSingleShot(true);
    timer.start(300);
}

LoginWindow::~LoginWindow() {
    delete ui;
}

void LoginWindow::verifyCachedAccount() {
    auto cachedUserName = AppConfig::getInstance().getSecurityConfig(AppConfig::SecurityConfigItem::eCachedUserName);
    auto cachedPassword = AppConfig::getInstance().getSecurityConfig(AppConfig::SecurityConfigItem::eCachedPassword);
    auto accountExpiredTime = AppConfig::getInstance().getSecurityConfig(AppConfig::SecurityConfigItem::eAccountExpiredTime);

    auto timestampeNow = std::chrono::system_clock::now().time_since_epoch().count();
    long long timestampeAccountExpired = 0;
    if(!accountExpiredTime.empty()) {
        timestampeAccountExpired = std::stoll(accountExpiredTime);
    }

    if(!cachedUserName.empty() && !cachedPassword.empty() && timestampeAccountExpired > timestampeNow) {
        doLogin(QString::fromStdString(cachedUserName), QString::fromStdString(cachedPassword), true);
    }
}

void LoginWindow::onLoginBtnClicked(bool checked) {
    if(ui->userNameEditor->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Error","User Name cannot be empty!");
        return;
    }
    if(ui->passwordEditor->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Error","Password cannot be empty!");
        return;
    }
    doLogin(ui->userNameEditor->text().trimmed(), ui->passwordEditor->text().trimmed());
}

void LoginWindow::onRegisterBtnClicked(bool checked) {
    RegisterWindow registerWindow{this};
    registerWindow.exec();
}

bool LoginWindow::doLogin(QString userName, QString password, bool slientMode) {
    json response = HttpClient::getInstance().userLogin(userName.toStdString(), password.toStdString(), this);
    if(!response.empty()){
        if(!slientMode) {
            QMessageBox::information(this,"Info","Login Successfully!");
        }
        AppConfig::getInstance().setSecurityConfig(AppConfig::SecurityConfigItem::eCachedUserName, userName.toStdString());
        AppConfig::getInstance().setSecurityConfig(AppConfig::SecurityConfigItem::eCachedPassword, password.toStdString());

        auto timestampeNow = std::chrono::system_clock::now();
        std::chrono::days days(15);
        auto expiredTime = timestampeNow + days;
        auto seconds_since_epoch = expiredTime.time_since_epoch().count();

        AppConfig::getInstance().setSecurityConfig(AppConfig::SecurityConfigItem::eAccountExpiredTime, std::to_string(seconds_since_epoch));

        AppConfig::getInstance().writeSecurityConfig();

        CachedUserData::getInstance().CachedUserMetaInfo.setUserMetaInfo(response["UserInfo"]);
        CachedUserData::getInstance().UserName = userName.toStdString();
        CachedUserData::getInstance().Password = password.toStdString();

        QSettings settings("HHMI", "CAR");
        settings.setValue("UserName", userName);
        settings.setValue("UserPasswd", password);

        accept();
        return true;
    }

    return false;
}
