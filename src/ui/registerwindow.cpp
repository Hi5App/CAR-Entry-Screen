#include "registerwindow.h"

#include <QMessageBox>

#include "ui_registerwindow.h"
#include "../framework/defination/ImageDefination.h"
#include "../framework/service/UserMetaInfo.h"
#include "../framework/service/CachedUserData.h"
#include "../framework/service/HttpClient.h"
#include "json.hpp"


RegisterWindow::RegisterWindow(QWidget *parent) :
    QDialog(parent), ui(new Ui::RegisterWindow) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(Image::ImageCreateUser));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->size());

    connect(ui->registerBtn,&QPushButton::clicked, this, &RegisterWindow::onRegisterBtnClicked);

    ui->label->setPixmap(QPixmap::fromImage(QImage(Image::ImageCreateUser).scaled({32,32})));
    ui->label_2->setPixmap(QPixmap::fromImage(QImage(Image::ImagePassword).scaled({32,32})));
    ui->label_3->setPixmap(QPixmap::fromImage(QImage(Image::ImagePassword).scaled({32,32})));

}

RegisterWindow::~RegisterWindow() {
    delete ui;
}

void RegisterWindow::onRegisterBtnClicked(bool checked) {
    if(ui->userNameEditor->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Error","User Name cannot be empty!");
        return;
    }
    if(ui->passwordEditor->text().trimmed().isEmpty() || ui->repeatPasswordEditor->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Error","Password cannot be empty!");
        return;
    }
    if(ui->passwordEditor->text().trimmed() != ui->repeatPasswordEditor->text().trimmed()) {
        QMessageBox::warning(this,"Error","Password and Repeated Password are not equal!");
        return;
    }

    auto response = HttpClient::getInstance().userRegister(ui->userNameEditor->text().trimmed().toStdString(), ui->passwordEditor->text().trimmed().toStdString(), this);
    if(!response.empty()){
        QMessageBox::information(this,"Info","Register Successfully!");
        accept();
    }
}
