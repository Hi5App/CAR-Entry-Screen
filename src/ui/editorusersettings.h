#pragma once

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class EditorUserSettings; }
QT_END_NAMESPACE

class MainWindow;

class EditorUserSettings : public QDialog {
Q_OBJECT

public:
    explicit EditorUserSettings(MainWindow* mainwindow);
    ~EditorUserSettings() override;

    std::string getName() {
        return "EditorUserSettings";
    }

//    MetaInfoType getMetaInfoType() override {
//        return MetaInfoType::eUserMetaInfo;
//    }

//    bool save() override {
//        return false;
//    }

    void getUserMetaInfo();

private:
    MainWindow* m_mainwindow;
    Ui::EditorUserSettings *ui;
};
