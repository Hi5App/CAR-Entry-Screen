#include <QMessageBox>
#include <QBuffer>
#include <QFileDialog>
#include <QStandardPaths>
#include "editorusersettings.h"
#include "mainwindow.h"
#include "ui_editorusersettings.h"
#include "../framework/service/CachedUserData.h"
#include "../framework/defination/ImageDefination.h"
#include "../framework/service/HttpClient.h"
#include "json.hpp"

EditorUserSettings::EditorUserSettings(MainWindow* mainwindow) :
    QDialog(), ui(new Ui::EditorUserSettings) {
    ui->setupUi(this);
    setWindowIcon(QIcon(Image::ImageUser));
    m_mainwindow = mainwindow;

    ui->ChangeHeadPhoto->setIcon(QIcon(Image::ImageEdit));
    connect(ui->ChangeHeadPhoto,&QPushButton::clicked,this,[this](){
        QFileDialog dialog;
        dialog.setWindowTitle("Select your head photo");
        dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        dialog.setNameFilter(tr("File(*.png)"));
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setViewMode(QFileDialog::Detail);
        if(dialog.exec()){
            if(dialog.selectedFiles().size() < 1){
                return;
            }
            QString fileName = dialog.selectedFiles()[0];
            QPixmap pixmap;
            pixmap.load(fileName);
            pixmap = pixmap.scaled(QSize(128,128),Qt::KeepAspectRatioByExpanding);
            ui->HeadPhoto->setPixmap(pixmap);
        }
    });
    connect(ui->CancelBtn,&QPushButton::clicked,this,[this](){
        reject();
    });
    connect(ui->OKBtn,&QPushButton::clicked,this,[this](){
        if(ui->Description->text().trimmed().isEmpty()){
            QMessageBox::warning(this,"Error","Description cannot be empty!");
            return;
        }
        if(ui->Password->text().trimmed().isEmpty()){
            QMessageBox::warning(this,"Error","Password cannot be empty!");
            return;
        }
        QBuffer buffer;
        if(buffer.open(QIODevice::WriteOnly)){
            ui->HeadPhoto->pixmap().save(&buffer,"png");
        }

        auto json = HttpClient::getInstance().userEditInfo(ui->Password->text().trimmed().toStdString(), ui->Description->text().trimmed().toStdString(), buffer.data().toBase64().toStdString(), this);
        if(!json.empty()){
            CachedUserData::getInstance().CachedUserMetaInfo.setUserMetaInfo(json["UserInfo"]);
            CachedUserData::getInstance().Password = json["UserInfo"]["Password"];
            QPixmap pixmap;
            auto rawdata = json["UserInfo"]["HeadPhotoBinData"];
            QByteArray byteArray(reinterpret_cast<const char*>(rawdata.dump().c_str()), rawdata.dump().length());
            // Decode QByteArray from Base64
            QByteArray decodedData = QByteArray::fromBase64(byteArray);
            //        auto data = QByteArray::fromStdString(rawdata);
            pixmap.loadFromData(decodedData);
            pixmap = pixmap.scaled(QSize(128,128),Qt::KeepAspectRatioByExpanding);
            m_mainwindow->setAccountBtnIcon(pixmap);

            QMessageBox::information(this,"Notice","Update user info success!");
            accept();
        }
    });
    getUserMetaInfo();
}

EditorUserSettings::~EditorUserSettings() {
    delete ui;
}

void EditorUserSettings::getUserMetaInfo() {
    auto json = HttpClient::getInstance().getUserByName(this);
    if(!json.empty()) {
        CachedUserData::getInstance().CachedUserMetaInfo.setUserMetaInfo(json["UserInfo"]);

        ui->Id->setText(QString::fromStdString(json["UserInfo"]["Base"]["Id"]));
        ui->Id->setReadOnly(true);
        ui->Uuid->setText(QString::fromStdString(json["UserInfo"]["Base"]["Uuid"]));
        ui->Uuid->setReadOnly(true);
        ui->DataAccessModelVersion->setText(QString::fromStdString(json["UserInfo"]["Base"]["DataAccessModelVersion"]));
        ui->DataAccessModelVersion->setReadOnly(true);
        ui->Name->setText(QString::fromStdString(json["UserInfo"]["Name"]));
        ui->Name->setReadOnly(true);
        ui->Description->setText(QString::fromStdString(json["UserInfo"]["Description"]));
        ui->Password->setText(QString::fromStdString(json["UserInfo"]["Password"]));
        ui->CreateTime->setDateTime(QDateTime::fromString(QString::fromStdString(json["UserInfo"]["CreateTime"]), Qt::ISODateWithMs));
        ui->CreateTime->setReadOnly(true);
        QPixmap pixmap;
        auto rawdata = json["UserInfo"]["HeadPhotoBinData"];
        QByteArray byteArray(reinterpret_cast<const char*>(rawdata.dump().c_str()), rawdata.dump().length());
        // Decode QByteArray from Base64
        QByteArray decodedData = QByteArray::fromBase64(byteArray);
//        auto data = QByteArray::fromStdString(rawdata);
        pixmap.loadFromData(decodedData);
        pixmap = pixmap.scaled(QSize(128,128),Qt::KeepAspectRatioByExpanding);
        ui->HeadPhoto->setPixmap(pixmap);

        auto responseGetPermissionGroup = HttpClient::getInstance().getPermissionGroupByUuid(json["UserInfo"]["PermissionGroupUuid"], this);

        ui->PermissionGroup->setText(QString::fromStdString(responseGetPermissionGroup["PermissionGroup"]["Name"]));
        ui->PermissionGroup->setReadOnly(true);

        ui->UserId->setText(QString::number(int(json["UserInfo"]["UserId"])));
        ui->UserId->setReadOnly(true);
    }
}
