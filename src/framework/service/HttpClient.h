#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "json.hpp"
#include "CachedUserData.h"
#include <string>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QMessageBox>
#include <QEventLoop>

using json = nlohmann::json;

class HttpClient : public QObject
{
    Q_OBJECT
public:
    HttpClient(HttpClient&) = delete;
    HttpClient& operator=(HttpClient&) = delete;

    void initialize(const std::string& serverIp, const std::string& port) {
        m_serverIp = serverIp;
        m_port = port;
        QString header = "http://";
        QString mid = ":";
        QString end = "/proto.DBMS/";
        QString prefix = header + QString::fromStdString(m_serverIp) + mid + QString::fromStdString(m_port) + end;

        urlForGetUserByName = prefix + "GetUserByName";
        urlForUserRegister = prefix + "CreateUser";
        urlForUserLogin = prefix + "UserLogin";
        urlForUserLogout = prefix + "UserLogout";
        urlForUserEditInfo = prefix + "UpdateUser";
        urlForGetPermissionGroupByUuid = prefix + "GetPermissionGroupByUuid";
    }

    static HttpClient& getInstance() {
        static HttpClient instance;
        return instance;
    }

    void setCommonRequestField(json& request_json) {
        json ApiVersion = {
            {"ApiVersion", m_apiVersion}
        };

        request_json["metaInfo"] = ApiVersion;
    }

    bool defaultErrorHandler(const std::string& actionName, int code, QString reason, json& response_json, QWidget* parent) {
        if(code == 200){
            bool status = response_json["metaInfo"]["Status"];
            if(status){
                return true;
            }
            else{
                std::string msg = response_json["metaInfo"]["Message"];
                QMessageBox::critical(parent, "Info", QString::fromStdString(actionName + " Failed!" + msg));
                return false;
            }
        }
        else {
            QMessageBox::critical(parent, "Info", QString::fromStdString(actionName + " Failed!" + reason.toStdString()));
            return false;
        }
        return false;
    }

    json getUserByName(QWidget* parent){
        QNetworkRequest request;
        request.setUrl(urlForGetUserByName);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        json request_json;
        setCommonRequestField(request_json);
        json UserVerifyInfo;
        UserVerifyInfo["UserName"] = CachedUserData::getInstance().UserName;
        UserVerifyInfo["UserPassword"] = CachedUserData::getInstance().Password;
        request_json["UserVerifyInfo"] = UserVerifyInfo;
        request_json["UserName"] = CachedUserData::getInstance().UserName;
        std::string request_jsonString = request_json.dump();
        QByteArray byteArray(request_jsonString.c_str(), request_jsonString.length());
        QNetworkReply* reply = accessManager.post(request, byteArray);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        json emptyJson;

        if(code == 0){
            QMessageBox::critical(parent, "Info", QString::fromStdString(std::string(__func__) + " Failed!" + reason.toStdString()));
            return emptyJson;
        }

        QByteArray tempArray = reply->readAll();
        std::string jsonString = tempArray.toStdString();
        // 解析 JSON 字符串为 JSON 对象
        json response_json = json::parse(jsonString);

        reply->close();
        reply->deleteLater();

        if(defaultErrorHandler(__func__, code, reason, response_json, parent)){
            return response_json;
        }
        return emptyJson;
    }


    json userLogin(std::string userName, std::string password, QWidget* parent){
        QNetworkRequest request;
        request.setUrl(urlForUserLogin);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        json request_json;
        setCommonRequestField(request_json);
        request_json["UserName"] = userName;
        request_json["Password"] = password;
        std::string request_jsonString = request_json.dump();
        QByteArray byteArray(request_jsonString.c_str(), request_jsonString.length());
        QNetworkReply* reply = accessManager.post(request, byteArray);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        json emptyJson;

        if(code == 0){
            QMessageBox::critical(parent, "Info", QString::fromStdString(std::string(__func__) + " Failed!" + reason.toStdString()));
            return emptyJson;
        }

        QByteArray tempArray = reply->readAll();
        std::string jsonString = tempArray.toStdString();
        // 解析 JSON 字符串为 JSON 对象
        json response_json = json::parse(jsonString);
        reply->close();
        reply->deleteLater();

        if(defaultErrorHandler(__func__, code, reason, response_json, parent)){
            return response_json;
        }
        return emptyJson;
    }

    json userRegister(std::string userName, std::string password, QWidget* parent){
        QNetworkRequest request;
        request.setUrl(urlForUserRegister);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        json request_json;
        setCommonRequestField(request_json);
        json UserVerifyInfo;
        UserVerifyInfo["UserName"] = CachedUserData::getInstance().UserName;
        UserVerifyInfo["Password"] = CachedUserData::getInstance().Password;
        request_json["UserVerifyInfo"] = UserVerifyInfo;
        json UserInfo;
        UserInfo["Name"] = userName;
        UserInfo["Password"] = password;
        request_json["UserInfo"] = UserInfo;
        std::string request_jsonString = request_json.dump();
        QByteArray byteArray(request_jsonString.c_str(), request_jsonString.length());
        QNetworkReply* reply = accessManager.post(request, byteArray);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        json emptyJson;

        if(code == 0){
            QMessageBox::critical(parent, "Info", QString::fromStdString(std::string(__func__) + " Failed!" + reason.toStdString()));
            return emptyJson;
        }

        QByteArray tempArray = reply->readAll();
        std::string jsonString = tempArray.toStdString();
        // 解析 JSON 字符串为 JSON 对象
        json response_json = json::parse(jsonString);

        reply->close();
        reply->deleteLater();

        if(defaultErrorHandler(__func__, code, reason, response_json, parent)){
            return response_json;
        }
        return emptyJson;
    }

    json userLogout(QWidget* parent){
        QNetworkRequest request;
        request.setUrl(urlForUserLogout);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        json request_json;
        setCommonRequestField(request_json);
        json UserVerifyInfo;
        UserVerifyInfo["UserName"] = CachedUserData::getInstance().UserName;
        UserVerifyInfo["UserPassword"] = CachedUserData::getInstance().Password;
        request_json["UserVerifyInfo"] = UserVerifyInfo;
        std::string request_jsonString = request_json.dump();
        QByteArray byteArray(request_jsonString.c_str(), request_jsonString.length());
        QNetworkReply* reply = accessManager.post(request, byteArray);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        json emptyJson;

        if(code == 0){
            QMessageBox::critical(parent, "Info", QString::fromStdString(std::string(__func__) + " Failed!" + reason.toStdString()));
            return emptyJson;
        }

        QByteArray tempArray = reply->readAll();
        std::string jsonString = tempArray.toStdString();
        // 解析 JSON 字符串为 JSON 对象
        json response_json = json::parse(jsonString);

        reply->close();
        reply->deleteLater();

        if(defaultErrorHandler(__func__, code, reason, response_json, parent)){
            return response_json;
        }
        return emptyJson;
    }

    json userEditInfo(std::string password, std::string description, std::string headPhotoBinData, QWidget* parent){
        QNetworkRequest request;
        request.setUrl(urlForUserEditInfo);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        json request_json;
        setCommonRequestField(request_json);
        json UserVerifyInfo;
        UserVerifyInfo["UserName"] = CachedUserData::getInstance().UserName;
        UserVerifyInfo["UserPassword"] = CachedUserData::getInstance().Password;
        request_json["UserVerifyInfo"] = UserVerifyInfo;

        json UserInfo;
        json Base;
        Base["Id"] = CachedUserData::getInstance().CachedUserMetaInfo.Id;
        Base["DataAccessModelVersion"] = CachedUserData::getInstance().CachedUserMetaInfo.DataAccessModelVersion;
        Base["Uuid"] = CachedUserData::getInstance().CachedUserMetaInfo.Uuid;
        UserInfo["Base"] = Base;
        UserInfo["Name"] = CachedUserData::getInstance().CachedUserMetaInfo.Name;
        UserInfo["Password"] = password;
        UserInfo["Description"] = description;
        UserInfo["CreateTime"] = CachedUserData::getInstance().CachedUserMetaInfo.CreateTime;
        UserInfo["HeadPhotoBinData"] = headPhotoBinData;
        UserInfo["UserId"] = CachedUserData::getInstance().CachedUserMetaInfo.UserId;
        UserInfo["PermissionGroupUuid"] = CachedUserData::getInstance().CachedUserMetaInfo.UserPermissionGroupUuid;

        request_json["UserInfo"] = UserInfo;

        std::string request_jsonString = request_json.dump();
        QByteArray byteArray(request_jsonString.c_str(), request_jsonString.length());
        QNetworkReply* reply = accessManager.post(request, byteArray);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        json emptyJson;

        if(code == 0){
            QMessageBox::critical(parent, "Info", QString::fromStdString(std::string(__func__) + " Failed!" + reason.toStdString()));
            return emptyJson;
        }

        QByteArray tempArray = reply->readAll();
        std::string jsonString = tempArray.toStdString();
        // 解析 JSON 字符串为 JSON 对象
        json response_json = json::parse(jsonString);

        reply->close();
        reply->deleteLater();

        if(defaultErrorHandler(__func__, code, reason, response_json, parent)){
            return response_json;
        }
        return emptyJson;
    }

    json getPermissionGroupByUuid(std::string PermissionGroupUuid, QWidget* parent){
        QNetworkRequest request;
        request.setUrl(urlForGetPermissionGroupByUuid);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        json request_json;
        setCommonRequestField(request_json);
        json UserVerifyInfo;
        UserVerifyInfo["UserName"] = CachedUserData::getInstance().UserName;
        UserVerifyInfo["UserPassword"] = CachedUserData::getInstance().Password;
        request_json["UserVerifyInfo"] = UserVerifyInfo;

        request_json["PermissionGroupUuid"] = PermissionGroupUuid;

        std::string request_jsonString = request_json.dump();
        QByteArray byteArray(request_jsonString.c_str(), request_jsonString.length());
        QNetworkReply* reply = accessManager.post(request, byteArray);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        json emptyJson;

        if(code == 0){
            QMessageBox::critical(parent, "Info", QString::fromStdString(std::string(__func__) + " Failed!" + reason.toStdString()));
            return emptyJson;
        }

        QByteArray tempArray = reply->readAll();
        std::string jsonString = tempArray.toStdString();
        // 解析 JSON 字符串为 JSON 对象
        json response_json = json::parse(jsonString);

        reply->close();
        reply->deleteLater();

        if(defaultErrorHandler(__func__, code, reason, response_json, parent)){
            return response_json;
        }
        return emptyJson;
    }


private:
    HttpClient(){

    }

    std::string m_serverIp;
    std::string m_port;
    std::string m_apiVersion = "2024.05.06";
    QNetworkAccessManager accessManager;
    QString urlForGetUserByName;
    QString urlForUserRegister;
    QString urlForUserLogin;
    QString urlForUserLogout;
    QString urlForUserEditInfo;
    QString urlForGetPermissionGroupByUuid;
};

#endif // HTTPCLIENT_H
