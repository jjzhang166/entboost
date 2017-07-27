#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T10:21:32
#
#-------------------------------------------------

QT       -= core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
CONFIG(debug,debug|release) {
TARGET = "../../build/ebcmd"
}
else {
TARGET = "../../build/ebcm"
}

}
else {
CONFIG(debug,debug|release) {
TARGET = "../build/ebcmd"
}
else {
TARGET = "../build/ebcm"
}
}

#OBJECTS_DIR = "../../build/"
#TARGET = libebcm
TEMPLATE = lib

#DEFINES += LIBEBCM_LIBRARY
DEFINES += _QT_MAKE_
DEFINES += CHATROOM_EXPORTS

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
INCLUDEPATH += F:/THIRDPARTY/zlib-1.2.8
INCLUDEPATH += F:/THIRDPARTY/boost_1_62_0
INCLUDEPATH += F:/THIRDPARTY/openssl-1.0.2k/inc32
INCLUDEPATH += "D:/dev/mycp/src"
INCLUDEPATH += "D:/dev/mycp/src/ThirdParty"
INCLUDEPATH += "F:/THIRDPARTY/zlib-1.2.8"

 Release {
LIBS += -L"D:/dev/mycp/src/build" -lCGCLibQt -lCGCClassQt
LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-vc140-mt-1_62 -llibboost_thread-vc140-mt-1_62 -llibboost_filesystem-vc140-mt-1_62
LIBS += -L"F:/THIRDPARTY/zlib-1.2.8/contrib/vstudio/vc14/lib" -lzlibstat
# libboost_system-mgw53-mt-1_62.a,libboost_thread-mgw53-mt-1_62.a,libboost_filesystem-mgw53-mt-1_62.a
LIBS += -lWs2_32 -lshlwapi -lWinmm
  }
  Debug {
LIBS += -L"D:/dev/mycp/src/build" -lCGCLibQtd -lCGCClassQtd
LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-vc140-mt-gd-1_62 -llibboost_thread-vc140-mt-gd-1_62 -llibboost_filesystem-vc140-mt-gd-1_62
LIBS += -lWs2_32 -lshlwapi -lWinmm
LIBS += -L"F:/THIRDPARTY/zlib-1.2.8/contrib/vstudio/vc14/lib" -lzlibstatd
  }
## 使用静态库会报错
LIBS += -L"F:/THIRDPARTY/openssl-1.0.2k/vc2015" -llibeay32 -lssleay32
#LIBS += -L"F:/THIRDPARTY/zlib-1.2.11/lib" -lzlib1

}
else:unix: {
INCLUDEPATH += "/Users/akee/src/boost_1_62_0"
#INCLUDEPATH += "/usr/local/ssl/include"
#INCLUDEPATH += "/usr/local/include"
#INCLUDEPATH += "/usr/include"
INCLUDEPATH += "/Users/akee/src/mycp/src"
INCLUDEPATH += "/Users/akee/src/mycp/src/ThirdParty"
#INCLUDEPATH += "/usr/local/opt/zlib/include"

CONFIG(debug,debug|release) {
LIBS += "/Users/akee/src/mycp/build/libCGCClassQtd.a"
LIBS += "/Users/akee/src/mycp/build/libCGCLibQtd.a"
}
else {
LIBS += "/Users/akee/src/mycp/build/libCGCClassQt.a"
LIBS += "/Users/akee/src/mycp/build/libCGCLibQt.a"
}

#LIBS += -L"/Users/akee/src/mycp/src/proj/QT5.8/lib" -llibCGCLib.a -llibCGCClass.a
LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_system.a"
LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_thread.a"
LIBS += "/Users/akee/src/boost_1_62_0/stage/lib/libboost_filesystem.a"
#LIBS += -L"/Users/akee/src/boost_1_62_0/stage/lib" -llibboost_system -llibboost_thread -llibboost_filesystem
#LIBS += -lWs2_32 -lshlwapi -lWinmm

#LIBS += "/usr/local/opt/zlib/lib/libz.a"
#LIBS += -L"/usr/local/opt/zlib/lib" -lz
#LIBS += "/usr/local/lib/libz.a"
#LIBS += -L"/usr/local/lib" -lz
LIBS += -L"/usr/lib" -lz

LIBS += -L"/usr/lib" -lcrypto -lssl
#LIBS += -L"/usr/local/ssl/lib" -lcrypto -lssl
#LIBS += "/usr/local/ssl/lib/libcrypto.a"
#LIBS += "/usr/local/ssl/lib/libssl.a"
#LIBS += -L"/usr/local/lib" -lcrypto -lssl
#LIBS += -L"/usr/local/lib" -llibeay32 -lssleay32

}

#win32:CONFIG(release, debug|release):{
## libCGCClass.a,libCGCLib.a
#LIBS += -L"C:/Program Files (x86)/MYCP/src/build" -llibCGCLib -llibCGCClass
##LIBS += -L"C:/Program Files (x86)/MYCP/src/build" -llibCGCClass
##LIBS += -L"C:/Program Files (x86)/MYCP/src/build" -llibCGCLib
## libboost_system-mgw53-mt-1_62.a,libboost_thread-mgw53-mt-1_62.a,libboost_filesystem-mgw53-mt-1_62.a
#LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-mgw53-mt-1_62 -llibboost_thread-mgw53-mt-1_62 -llibboost_filesystem-mgw53-mt-1_62
## libeay32.lib,ssleay32.lib,
#LIBS += -L"D:\openssl\lib" -llibeay32 -lssleay32
#LIBS += -lWs2_32 -lshlwapi -lWinmm
#}
#else:win32:CONFIG(debug, debug|release):{
#LIBS += -L"C:/Program Files (x86)/MYCP/src/build" -llibCGCLib -llibCGCClass
#LIBS += -L"F:/THIRDPARTY/boost_1_62_0/stage/lib" -llibboost_system-mgw53-mt-1_62 -llibboost_thread-mgw53-mt-1_62 -llibboost_filesystem-mgw53-mt-1_62
#LIBS += -L"D:\openssl\lib" -llibeay32 -lssleay32
#LIBS += -lWs2_32 -lshlwapi -lWinmm
#}
#else:unix:{
#LIBS += -L$$PWD/../zlib64/ -lzdll
#}

#win32:CONFIG(release, debug|release): LIBS += -L"F:/THIRDPARTY/zlib-1.2.11/lib" -lzlib1
#else:win32:CONFIG(debug, debug|release): LIBS += -L"F:/THIRDPARTY/zlib-1.2.11/lib" -lzlib1
#else:unix: LIBS += -L$$PWD/../zlib64/ -lzdll


#  LIBS+=-L/usr/lib  -lz -lpcap -lm -lnsl  -lmysqlclient


SOURCES += ../../../libchatroom/chatroom.cpp \
    ../../../libchatroom/md5.cpp \
    ../../../include/POPCChatManager.cpp \
    ../../../include/SendFileThread.cpp

HEADERS += ../../../libchatroom/chatroom.h \
    ../../../libchatroom/md5.h \
    ../../../libchatroom/pophandler.h \
    ../../../libchatroom/resource.h \
    ../../../include/POPCChatManager.h \
    ../../../include/SendFileThread.h \
    ../../../include/chatroomhandle.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
