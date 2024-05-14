cmake_minimum_required(VERSION 3.5)

project(CAR-WS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 Qt5 COMPONENTS
#    Core
#    Gui
#    Widgets
#    Svg
#    OpenGL
#    OpenGLWidgets
#    REQUIRED)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Svg
    Network
    REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(
        ThirdParty/nlohmann_json
        src/styles
)

link_directories(
        .
)

set(QRESOURCE
        Resource/swcdbmsclientresources.qrc
)

file(GLOB_RECURSE SRC_LIST
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.ui"
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.hpp"
)

set(PROJECT_HEADERS
        src/styles/acss_globals.h
        src/styles/QtAdvancedStylesheet.h
)

set(PROJECT_SOURCES
        main.cpp
        
        src/styles/QtAdvancedStylesheet.cpp

        logo.rc

#        src/ui/mainwindow.cpp
#        src/ui/mainwindow.h
#        src/ui/mainwindow.ui
#        src/ui/loginwindow.cpp
#        src/ui/loginwindow.h
#        src/ui/loginwindow.ui
#        src/ui/registerwindow.cpp
#        src/ui/registerwindow.h
#        src/ui/registerwindow.ui
#        src/ui/editorusersettings.cpp
#        src/ui/editorusersettings.h
#        src/ui/editorusersettings.ui

#        src/framework/config/AppConfig.cpp
#        src/framework/config/AppConfig.h
#        src/framework/service/CachedUserData.cpp
#        src/framework/service/CachedUserData.h
#        src/framework/service/UserMetaInfo.cpp
#        src/framework/service/UserMetaInfo.h
#        src/framework/service/HttpClient.cpp
#        src/framework/service/HttpClient.h
#        src/framework/defination/TypeDef.cpp
#        src/framework/defination/TypeDef.h
#        src/framework/defination/ImageDefination.h
)

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

add_executable(CAR-WS ${QRESOURCE} ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${SRC_LIST})

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CAR-WS APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
#else()
#    if(ANDROID)
#        add_library(CAR-WS SHARED
#            ${PROJECT_SOURCES}
#        )
## Define properties for Android with Qt 5 after find_package() calls as:
##    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else()
#        add_executable(CAR-WS
#            ${PROJECT_SOURCES}
#        )
#    endif()
#endif()

target_link_libraries(CAR-WS PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Svg
    Qt::Network
)

set_target_properties(CAR-WS PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS CAR-WS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CAR-WS)
endif()
