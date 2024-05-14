#pragma once
#include <string>
#include "UserMetaInfo.h"

class CachedUserData {
public:
    static CachedUserData& getInstance() {
        static CachedUserData instance;
        return instance;
    }

    UserMetaInfo CachedUserMetaInfo;

    std::string UserName;
    std::string Password;

private:
    CachedUserData();
};
