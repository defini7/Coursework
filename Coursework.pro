QT       += core gui widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    App.cpp \
    DataLoader.cpp \
    Dialog_Params.cpp \
    Main.cpp \
    Menu_Data.cpp \
    Menu_Model.cpp \
    UserContext.cpp \
    UserNameDialog.cpp \
    WellData.cpp

HEADERS += \
    DataLoader.hpp \
    Dialog_Params.hpp \
    Main.hpp \
    Menu_Data.hpp \
    Menu_Model.hpp \
    UserContext.hpp \
    UserNameDialog.hpp \
    WellData.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
