#-------------------------------------------------
#
# Project created by QtCreator 2017-05-02T15:27:38
#
#-------------------------------------------------

QT       += core gui network
#QT += webengine
QT += webenginewidgets
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

Release {
TARGET = "../../build/entboost"
}
Debug {
TARGET = "../../build/entboostd"
}
#TARGET = ebc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += _QT_MAKE_
DEFINES += _QT_USES_WEBENGINE_

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
INCLUDEPATH += F:/THIRDPARTY/bodb/inc
INCLUDEPATH += "F:/THIRDPARTY/zlib-1.2.8"
INCLUDEPATH += F:/THIRDPARTY/boost_1_62_0
INCLUDEPATH += "F:/THIRDPARTY/sqlite-3081002"
#INCLUDEPATH += D:/openssl/include
INCLUDEPATH += F:/THIRDPARTY/openssl-1.0.2k/inc32
INCLUDEPATH += "D:/git/mycp/src"
INCLUDEPATH += "D:/git/mycp/src/ThirdParty"
#INCLUDEPATH += "F:/THIRDPARTY/cef_binary_3.2623.1395.g3034273_windows32"
#INCLUDEPATH += "F:/THIRDPARTY/zlib-1.2.11"
INCLUDEPATH += "../../../libchatroom"
INCLUDEPATH += "../../../libpopusermanager"
INCLUDEPATH += "../../../include"

    Release {
LIBS += -L"D:/git/mycp/src/build" -lCGCLibQt -lCGCClassQt
LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-vc140-mt-1_62 -llibboost_thread-vc140-mt-1_62 -llibboost_filesystem-vc140-mt-1_62
LIBS += -lWs2_32 -lshlwapi
#LIBS += -lWs2_32 -lshlwapi -lWinmm
LIBS += -L"../build" -llibebcm
LIBS += -L"../build" -llibebum
LIBS += -L"F:/THIRDPARTY/sqlite-3081002/lib" -lsqlite3s
LIBS += -L"F:/THIRDPARTY/zlib-1.2.8/contrib/vstudio/vc14/lib" -lzlibstat
    }
    Debug {
LIBS += -L"D:/git/mycp/src/build" -lCGCLibQtd -lCGCClassQtd
LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-vc140-mt-gd-1_62 -llibboost_thread-vc140-mt-gd-1_62 -llibboost_filesystem-vc140-mt-gd-1_62
LIBS += -lWs2_32 -lshlwapi
LIBS += -L"../build" -llibebcmd
LIBS += -L"../build" -llibebumd
LIBS += -L"F:/THIRDPARTY/sqlite-3081002/lib" -lsqlite3sd
LIBS += -L"F:/THIRDPARTY/zlib-1.2.8/contrib/vstudio/vc14/lib" -lzlibstatd
    }
LIBS += -L"F:/THIRDPARTY/openssl-1.0.2k/vc2015" -llibeay32 -lssleay32
}
else:unix: {
INCLUDEPATH += "/Users/akee/src/boost_1_62_0"
#INCLUDEPATH += "F:/THIRDPARTY/sqlite-3081002/inc"
INCLUDEPATH += "/Users/akee/src/mycp/src"
INCLUDEPATH += "/Users/akee/src/mycp/src/ThirdParty"
#INCLUDEPATH += "/usr/local/opt/zlib/include"
INCLUDEPATH += "/usr/local/include/sqlite3"
INCLUDEPATH += "../../../libchatroom"
INCLUDEPATH += "../../../libpopusermanager"
INCLUDEPATH += "../../../include"

LIBS += "../build/libCGCClass.a"
LIBS += "../build/libCGCLib.a"
#LIBS += "/Users/akee/src/mycp/src/proj/QT5.8/lib/libCGCClass.a"
#LIBS += "/Users/akee/src/mycp/src/proj/QT5.8/lib/libCGCLib.a"
LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_system.a"
LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_thread.a"
LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_filesystem.a"
#LIBS += -lWs2_32 -lshlwapi -lWinmm
LIBS += -L"../build" -llibebcm
LIBS += -L"../build" -llibebum

##LIBS += "/usr/local/opt/zlib/lib/libz.a"
##LIBS += -L"/usr/local/opt/zlib/lib" -lz
#LIBS += -L"/usr/lib" -lz
##LIBS += -L"/usr/local/lib" -lsqlite3
#LIBS += "/usr/local/lib/libsqlite3.a"
##LIBS += "/usr/local/lib/libcrypto.a"
##LIBS += "/usr/local/lib/libssl.a"
#LIBS += -L"/usr/local/lib" -lcrypto -lssl
##LIBS += -L"/usr/local/lib" -llibeay32 -lssleay32

}

SOURCES += main.cpp\
        mainwindow.cpp \
        EbDialogBase.cpp \
    dialoglogin.cpp \
    ../../../include/ebc_public.cpp \
    iconhelper.cpp \
    ebclientapp.cpp \
    dialogconnectsetting.cpp \
    ebclocales.cpp \
    httpfiledownload.cpp \
    ebmessagebox.cpp \
    dialogregister.cpp \
    dialogmainframe.cpp \
    dialogframelist.cpp \
    dialogchatbase.cpp \
    ebtextedit.cpp \
    ebwidgetchatinput.cpp \
    ebtextbrowser.cpp \
    ebwidgetchatright.cpp \
    ebframeitem.cpp \
    ebframelist.cpp \
    dialogemotionselect.cpp \
    ebccallinfo.cpp \
    dialogmessagetip.cpp \
    eblistwidgetitem.cpp \
    ebwidgetmygroup.cpp \
    ebtreewidgetitem.cpp \
    eblabel.cpp \
    dialogmemberinfo.cpp \
    dialogchangehead.cpp \
    dialogmycenter.cpp \
    ebwidgetaccountinfo.cpp \
    ebwidgetmysetting.cpp \
    ebwidgetmodifypassword.cpp \
    dialoggroupinfo.cpp \
    ebwidgetsearchbar.cpp \
    ebwidgetsearchresult.cpp \
    dialogworkframe.cpp \
    ebwidgetworkview.cpp \
    ebworkitem.cpp \
    ebworklist.cpp \
    ebwebengineview.cpp \
    ebwebenginepage.cpp \
    ebwebengineprofile.cpp \
    ebwebenginedownloaditem.cpp \
    eblineedit.cpp \
    eblistwidget.cpp \
    ebwebengineurlrequestinterceptor.cpp \
    ebframefindtext.cpp \
    ebwidgetappbar.cpp \
    ebappbuttonitem.cpp \
    ebwebengineurlschemehandler.cpp \
    ebtextblockuserdata.cpp \
    ebframechattoolbar.cpp \
    ebwidgetuserinfo.cpp \
    ebwidgetuserlist.cpp \
    ebwidgetiteminfo.cpp \
    ebcontextmenu.cpp \
    ebwidgetfiletranlist.cpp \
    ebwidgetfiletranitem.cpp \
    ebwidgetchatrecord.cpp \
    ebdialogfilemanager.cpp

HEADERS  += mainwindow.h \
    EbDialogBase.h \
    dialoglogin.h \
    ../../../include/ebc_public.h \
    iconhelper.h \
    ebclientapp.h \
    dialogconnectsetting.h \
    ebclocales.h \
    httpfiledownload.h \
    ebmessagebox.h \
    dialogregister.h \
    dialogmainframe.h \
    dialogframelist.h \
    dialogchatbase.h \
    ebtextedit.h \
    ebwidgetchatinput.h \
    ebtextbrowser.h \
    ebwidgetchatright.h \
    ebframeitem.h \
    ebframelist.h \
    dialogemotionselect.h \
    ebccallinfo.h \
    dialogmessagetip.h \
    eblistwidgetitem.h \
    ebwidgetmygroup.h \
    ebtreewidgetitem.h \
    eblabel.h \
    dialogmemberinfo.h \
    dialogchangehead.h \
    dialogmycenter.h \
    ebwidgetaccountinfo.h \
    ebwidgetmysetting.h \
    ebwidgetmodifypassword.h \
    dialoggroupinfo.h \
    ebwidgetsearchbar.h \
    ebwidgetsearchresult.h \
    dialogworkframe.h \
    ebwidgetworkview.h \
    ebworkitem.h \
    ebworklist.h \
    ebwebengineview.h \
    ebwebenginepage.h \
    ebwebengineprofile.h \
    ebwebenginedownloaditem.h \
    eblineedit.h \
    eblistwidget.h \
    ebwebengineurlrequestinterceptor.h \
    ebframefindtext.h \
    ebwidgetappbar.h \
    ebappbuttonitem.h \
    ebwebengineurlschemehandler.h \
    ebtextblockuserdata.h \
    ebframechattoolbar.h \
    ebwidgetuserinfo.h \
    ebwidgetuserlist.h \
    ebwidgetiteminfo.h \
    ebcontextmenu.h \
    ebwidgetfiletranlist.h \
    ebwidgetfiletranitem.h \
    ebwidgetchatrecord.h \
    ebdialogfilemanager.h

FORMS    += mainwindow.ui \
    dialoglogin.ui \
    dialogconnectsetting.ui \
    ebmessagebox.ui \
    dialogregister.ui \
    dialogmainframe.ui \
    dialogframelist.ui \
    dialogchatbase.ui \
    dialogemotionselect.ui \
    dialogmessagetip.ui \
    dialogmemberinfo.ui \
    dialogchangehead.ui \
    dialogmycenter.ui \
    dialoggroupinfo.ui \
    dialogworkframe.ui \
    ebdialogfilemanager.ui

RESOURCES += \
    entboost.qrc

RC_FILE = entboost.rc
