#ifndef USERMETAINFO_H
#define USERMETAINFO_H

#include <string>
#include "json.hpp"

class UserMetaInfo
{
public:
    UserMetaInfo();

    std::string Id;
    std::string DataAccessModelVersion;
    std::string Uuid;
    std::string Name;
    std::string Password;
    std::string Description;
    std::string CreateTime;
    std::string HeadPhotoBinData;
    std::string UserPermissionGroupUuid;
    int UserId;

    void setUserMetaInfo(nlohmann::json json){
        Id = json["Base"]["Id"];
        DataAccessModelVersion = json["Base"]["DataAccessModelVersion"];
        Uuid = json["Base"]["Uuid"];
        Name = json["Name"];
        Password = json["Password"];
        Description = json["Description"];
        CreateTime = json["CreateTime"];
        HeadPhotoBinData = json["HeadPhotoBinData"];
        UserPermissionGroupUuid = json["PermissionGroupUuid"];
        UserId = json["UserId"];
    }
};

#endif // USERMETAINFO_H
