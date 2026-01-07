QT += core gui widgets printsupport

CONFIG += c++20 app_bundle

SOURCES += \
    ./Sources/App.cpp \
    ./Sources/Correlation.cpp \
    ./Sources/DataManager.cpp \
    ./Sources/Dialog_Params.cpp \
    ./Sources/GfxScene.cpp \
    ./Sources/Main.cpp \
    ./Sources/Menu_Data.cpp \
    ./Sources/Menu_Model.cpp \
    ./Sources/UserContext.cpp \
    ./Sources/Dialog_UserName.cpp

HEADERS += \
    ./Include/Correlation.hpp \
    ./Include/DataManager.hpp \
    ./Include/Dialog_Params.hpp \
    ./Include/GfxScene.hpp \
    ./Include/Main.hpp \
    ./Include/Menu_Data.hpp \
    ./Include/Menu_Model.hpp \
    ./Include/UserContext.hpp \
    ./Include/Dialog_UserName.hpp \
    ./Include/WellData.hpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
