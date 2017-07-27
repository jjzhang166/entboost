#-------------------------------------------------
#
# Project created by QtCreator 2017-04-29T11:00:32
#
#-------------------------------------------------

#QT       += core gui network
QT       -= gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
CONFIG(debug,debug|release) {
TARGET = "../../build/ebumd"
}
else {
TARGET = "../../build/ebum"
}
}
else {
CONFIG(debug,debug|release) {
TARGET = "../build/ebumd"
}
else {
TARGET = "../build/ebum"
}
}
TEMPLATE = lib

#DEFINES += LIBEBUM_LIBRARY
DEFINES += _QT_MAKE_
DEFINES += EB_APPCLIENT_EXPORTS
DEFINES += USES_CHATROOM
#DEFINES += USES_VIDEOROOM
#DEFINES += USES_REMOTE_DESKTOP

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


win32 {
INCLUDEPATH += "F:/THIRDPARTY/zlib-1.2.8"
INCLUDEPATH += F:/THIRDPARTY/boost_1_62_0
INCLUDEPATH += "F:/THIRDPARTY/sqlite-3081002"
INCLUDEPATH += F:/THIRDPARTY/openssl-1.0.2k/inc32
INCLUDEPATH += "D:/dev/mycp/src"
INCLUDEPATH += "D:/dev/mycp/src/ThirdParty"
INCLUDEPATH += "../../../libchatroom"
INCLUDEPATH += "../../../include"

    Release {
LIBS += -L"D:/git/mycp/src/build" -lCGCLibQt -lCGCClassQt
LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-vc140-mt-1_62 -llibboost_thread-vc140-mt-1_62 -llibboost_filesystem-vc140-mt-1_62
LIBS += -lWs2_32 -lshlwapi
LIBS += -L"../build" -lebcm
LIBS += -L"F:/THIRDPARTY/sqlite-3081002/lib" -lsqlite3s
LIBS += -L"F:/THIRDPARTY/zlib-1.2.8/contrib/vstudio/vc14/lib" -lzlibstat
    }
    Debug {
LIBS += -L"D:/dev/mycp/src/build" -lCGCLibQtd -lCGCClassQtd
LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-vc140-mt-gd-1_62 -llibboost_thread-vc140-mt-gd-1_62 -llibboost_filesystem-vc140-mt-gd-1_62
LIBS += -lWs2_32 -lshlwapi
#LIBS += -lWs2_32 -lshlwapi -lWinmm -lole32 -lOleaut32 -lWbemuuid
LIBS += -L"../build" -lebcmd
LIBS += -L"F:/THIRDPARTY/sqlite-3081002/lib" -lsqlite3sd
LIBS += -L"F:/THIRDPARTY/zlib-1.2.8/contrib/vstudio/vc14/lib" -lzlibstatd
    }
LIBS += -L"F:/THIRDPARTY/openssl-1.0.2k/vc2015" -llibeay32 -lssleay32

}
else:unix: {
INCLUDEPATH += "/Users/akee/src/boost_1_62_0"
#INCLUDEPATH += "F:/THIRDPARTY/sqlite-3081002/inc"
#INCLUDEPATH += D:/openssl/include
INCLUDEPATH += "/Users/akee/src/mycp/src"
INCLUDEPATH += "/Users/akee/src/mycp/src/ThirdParty"
#INCLUDEPATH += "/usr/local/opt/zlib/include"
INCLUDEPATH += "/usr/local/include/sqlite3"
INCLUDEPATH += "../../../libchatroom"
INCLUDEPATH += "../../../include"

CONFIG(debug,debug|release) {
LIBS += "/Users/akee/src/mycp/build/libCGCClassQtd.a"
LIBS += "/Users/akee/src/mycp/build/libCGCLibQtd.a"
LIBS += -L"../build" -lebcmd
}
else {
LIBS += "/Users/akee/src/mycp/build/libCGCClassQt.a"
LIBS += "/Users/akee/src/mycp/build/libCGCLibQt.a"
LIBS += -L"../build" -lebcm
}

LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_system.a"
LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_thread.a"
LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_filesystem.a"
#LIBS += -L"/Users/akee/src/boost_1_62_0/stage/lib" -llibboost_system -llibboost_thread -llibboost_filesystem
#LIBS += -lWs2_32 -lshlwapi -lWinmm
#LIBS += -L"/Users/akee/src/entboost/src/proj/Qt5.8/lib" -lebcm

#LIBS += "/usr/local/opt/zlib/lib/libz.a"
#LIBS += -L"/usr/local/opt/zlib/lib" -lz
LIBS += -L"/usr/lib" -lz
#LIBS += -L"/usr/local/lib" -lsqlite3
LIBS += "/usr/local/lib/libsqlite3.a"
#LIBS += "/usr/local/lib/libcrypto.a"
#LIBS += "/usr/local/lib/libssl.a"
LIBS += -L"/usr/lib" -lcrypto -lssl
#LIBS += -L"/usr/local/lib" -lcrypto -lssl
#LIBS += -L"/usr/local/lib" -llibeay32 -lssleay32

}

#win32:CONFIG(release, debug|release):{
## libCGCClass.a,libCGCLib.a
#LIBS += -L"C:/Program Files (x86)/MYCP/src/build" -llibCGCLib -llibCGCClass
## libboost_system-mgw53-mt-1_62.a,libboost_thread-mgw53-mt-1_62.a,libboost_filesystem-mgw53-mt-1_62.a
#LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-mgw53-mt-d-1_62 -llibboost_thread-mgw53-mt-d-1_62 -llibboost_filesystem-mgw53-mt-d-1_62
##LIBS += -L"C:/Program Files (x86)/MYCP/src/build" -llibCGCClass
##LIBS += -L"C:/Program Files (x86)/MYCP/src/build" -llibCGCLib
## libeay32.lib,ssleay32.lib,
#LIBS += -L"D:\openssl\lib" -llibeay32 -lssleay32
#LIBS += -lWs2_32 -lshlwapi -lWinmm
#LIBS += -L"F:/THIRDPARTY/chatroom/lib_qt/release" -lliblibebcm
## sqlite3.lib,sqlite3.dll
#LIBS += -L"F:/THIRDPARTY/sqlite-3081002/lib" -lsqlite3
#}
#else:win32:CONFIG(debug, debug|release):{
#LIBS += -L"C:/Program Files (x86)/MYCP/src/build" -llibCGCLib -llibCGCClass
#LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-mgw53-mt-1_62 -llibboost_thread-mgw53-mt-1_62 -llibboost_filesystem-mgw53-mt-1_62
#LIBS += -L"D:\openssl\lib" -llibeay32 -lssleay32
#LIBS += -lWs2_32 -lshlwapi -lWinmm
#LIBS += -L"F:/THIRDPARTY/chatroom/lib_qt/debug" -lliblibebcm
#LIBS += -L"F:/THIRDPARTY/sqlite-3081002/lib" -lsqlite3d
#}
#else:unix:{
#LIBS += -L$$PWD/../zlib64/ -lzdll
#}

#win32:CONFIG(release, debug|release): LIBS += -L"F:/THIRDPARTY/zlib-1.2.11/lib" -lzlib1
#else:win32:CONFIG(debug, debug|release): LIBS += -L"F:/THIRDPARTY/zlib-1.2.11/lib" -lzlib1
#else:unix: LIBS += -L$$PWD/../zlib64/ -lzdll

SOURCES += ../../../libpopusermanager/EBAppClient.cpp \
    ../../../libpopusermanager/UserManagerApp.cpp \
    ../../../include/EBCAppCenter.cpp \
    ../../../include/POPCLogonCenter.cpp \
    ../../../include/POPCUserManager.cpp \
    ../../../include/EBParseSetting.cpp

HEADERS += ../../../libpopusermanager/EBAppClient.h \
    ../../../libpopusermanager/UserManagerApp.h \
    ../../../include/EBCAppCenter.h \
    ../../../include/POPCLogonCenter.h \
    ../../../include/POPCUserManager.h \
    ../../../include/EBParseSetting.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
