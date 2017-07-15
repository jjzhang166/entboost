#include "mainwindow.h"
#include <QtCore/QCoreApplication>
#include "dialoglogin.h"
#include <QObject>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDesktopServices>
#include <QDir>
#include <QFontDatabase>
#include <QTranslator>
#include "ebclientapp.h"
#include "dialogmainframe.h"
//#include <QtWebEngine/QtWebEngine>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <ebwebengineurlschemehandler.h>

//void TestHostAddressList(void)
//{
//    QList<QHostAddress> list = QNetworkInterface::allAddresses();
//    foreach (QHostAddress address, list)
//    {
//        if(address.protocol() == QAbstractSocket::IPv4Protocol)
//        {
//            //IPv4地址
//            const QString sIp = address.toString();
//            if (sIp.contains("127.0.") || sIp.contains("0.0.0.0"))
//            {
//                continue;
//            }

//        }
//    }
//}
//void TestStandardPaths(void)
//{
//    QString sStringTemp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//    std::string m_sAppDataPath = sStringTemp.toStdString();
//    //m_sAppDataPath += "\\entboost";
//    //if (!QFileInfo::isDir(m_sAppDataPath))
//    QDir pDir1(m_sAppDataPath.c_str());
//    if (!pDir1.exists()) {
//        pDir1.mkdir(m_sAppDataPath.c_str());
//    }
//}

//void TestDirFile(void)
//{
//    {
//        QDir pDir1;
//        pDir1.mkdir("d:\\tn\\qt_dir\\mkdir1");
//    }
////    QFile::copy("d:\\tn\\qt_dir\\111.txt", "d:\\tn\\qt_dir\\mkdir1\\");
//    QFile::copy("d:\\tn\\qt_dir\\111.txt", "d:\\tn\\qt_dir\\222.txt");
//    QFile::remove("d:\\tn\\qt_dir\\222.txt");
//    QDir pDir2;
//    pDir2.rmdir("d:\\tn\\qt_dir\\mkdir1");
//}

//QTranslator g_tran;//必须设置成全局变量，如果是局部变量，设置将失效。
//void setLanguage(int lan)//全局函数，可供其他文件调用
//{
//    bool ok = false;
//    if (lan == 1)//中文
//    {
//        ok = g_tran.load("qt_zh_CN.qm", ":/lan/");//这里使用Qt资源文件,把.qm文件拷贝到项目路径/lan之下
//    }
//    else if (lan == 2)//英文
//    {
//        ok = g_tran.load("", ":/lan/");
//    }

//    QCoreApplication::installTranslator(&g_tran);
//    //或者写成qApp->installTranslator(&g_tran);
//}

bool findSysFontFamily(void)
{
    bool ret = false;
    QFontDatabase pFontDatabase;
    foreach (const QString &family, pFontDatabase.families(QFontDatabase::SimplifiedChinese)) {
//        qDebug()<<family;
        const std::wstring sFamily(family.toStdWString());
        if ( sFamily==theFontFamily1 ) {
            theFontFamily = sFamily;
            ret = true;
        }
        else if ( sFamily==theFontFamily0 ) {
            theFontFamily = sFamily;
            return true;
        }
    }
    return ret;
}

//#include <Q
//void getCPUInfo()
//{
////    SYSTEM_INFO sysInfo;
////    GetSystemInfo(&sysInfo);

////    m_pageSize->setText(QString("分页大小:\t%1").arg(sysInfo.dwPageSize));
////    char buff[32];
////    sprintf(buff, "%p", sysInfo.lpMinimumApplicationAddress);
////    m_minAddress->setText(QString("最小寻址:\t%1").arg(buff));
////    sprintf(buff, "%p", sysInfo.lpMaximumApplicationAddress);
////    m_maxAddress->setText(QString("最大寻址:\t%1").arg(buff));
////    m_mask->setText(QString("掩码:\t\t%1").arg(sysInfo.dwActiveProcessorMask));
////    m_processorNum->setText(QString("处理器个数:\t%1").arg(sysInfo.dwNumberOfProcessors));
////    m_processorType->setText(QString("类型:\t\t%1").arg(sysInfo.dwProcessorType));
////    m_processorLevel->setText(QString("等级:\t\t%1").arg(sysInfo.wProcessorLevel));
////    m_processorVersion->setText(QString("版本:\t\t%1").arg(sysInfo.wProcessorRevision));

//    QString lpRootPathName = "C:\\";
//    LPTSTR lpVolumeNameBuffer=new TCHAR[12];//磁盘卷标
//    DWORD nVolumeNameSize=12;// 卷标的字符串长度
//    DWORD VolumeSerialNumber;//硬盘序列号
//    DWORD MaximumComponentLength;// 最大的文件长度
//    LPTSTR lpFileSystemNameBuffer=new TCHAR[10];// 存储所在盘符的分区类型的长指针变量
//    DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
//    DWORD FileSystemFlags;// 文件系统的一此标志
//    GetVolumeInformation((LPTSTR)lpRootPathName.utf16(),
//                         lpVolumeNameBuffer, nVolumeNameSize,
//                         &VolumeSerialNumber, &MaximumComponentLength,
//                         &FileSystemFlags,
//                         lpFileSystemNameBuffer, nFileSystemNameSize);
//    qDebug() << VolumeSerialNumber;

//}

inline bool checkCreateDir(const QString & dirName)
{
    QDir pDir1(dirName);
    if (!pDir1.exists()) {
        return pDir1.mkdir(dirName);
    }
    return true;
}
//#define USES_CEF
int main(int argc, char *argv[])
{
//    getCPUInfo();
//    return 1;
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

//    QtWebEngine::initialize();

//#ifdef USES_CEF
////    CefExecuteProcess()
//    int result = CefInit(argc, argv);
//    if (result >= 0) {
//        return result;
//    }
//    CefLoadPlugins(IsWow64());
//#endif

//    const QString sQssFile = a.applicationDirPath()+"/datas/ebc.qss";
//    QFile qssFile(sQssFile);
//    if (qssFile.open(QFile::ReadOnly)) {
////        const QString qss = QLatin1String(qssFile.readAll());
//        const QString qss = QString::fromUtf8(qssFile.readAll());   // ** ok
//        a.setStyleSheet(qss);
//        qssFile.close();
//    }

    ///
    if ( findSysFontFamily() ) {
        QFont font = a.font();
        font.setFamily(QString::fromStdWString(theFontFamily));
        a.setFont(font);
    }

    theApp = EbClientApp::create();

//    setLanguage(1);//调用全局函数
    //加载Qt标准对话框的中文翻译文件
    QTranslator tranMain;
    if (tranMain.load(QString(":/qm/qt_zh_CN.qm"))) {
        a.installTranslator(&tranMain);
    }
    QTranslator tranWidgets;
    if (tranWidgets.load(":/qm/qt_zh_CN_widgets.qm")) {
        a.installTranslator(&tranWidgets);
    }

//    TestDirFile();
//    TestStandardPaths();
//    TestHostAddressList();

//    QString path;
//    QDir dir;
//    //path=dir.currentPath();
//    path = QCoreApplication::applicationDirPath();
//    path = QCoreApplication::applicationFilePath();

    /// 加载默认中文
    const QString localFileName = theApp->getAppLocalesPath()+"/zh-CN.json";
    theLocales.loadLocaleFile(localFileName.toStdString());
    if (!theApp->initApp()) {
        return 1;
    }

    DialogLogin pDlgLogin;
    const int nret = pDlgLogin.exec();
    if (nret==QDialog::Rejected) {
        return 0;
    }

    /// 登录成功
    /// 设置 chrome 缓存路径
    const QString m_sysAppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    checkCreateDir(m_sysAppDataLocation);
    QString m_sCefCachePath = m_sysAppDataLocation + "/cef_cache_temp";
    checkCreateDir(m_sCefCachePath);
    if ( theApp->isLogonVisitor() ) {
        m_sCefCachePath += "/visitor";
    }
    else {
        char lpszBuffer[24];
        sprintf(lpszBuffer,"/%lld", theApp->logonUserId() );
        m_sCefCachePath += lpszBuffer;
    }
    checkCreateDir(m_sCefCachePath);
//    qputenv(“QTWEBENGINE_REMOTE_DEBUGGING”, 9000);
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler( QByteArray(thePlayVoice), new EbWebEngineUrlSchemeHandler() );
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler( QByteArray(theCallAccount), new EbWebEngineUrlSchemeHandler() );
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler( QByteArray(theCallGroup), new EbWebEngineUrlSchemeHandler() );
    QWebEngineProfile::defaultProfile()->setCachePath( m_sCefCachePath );
    QWebEngineProfile::defaultProfile()->setHttpCacheType( QWebEngineProfile::DiskHttpCache );
    QWebEngineSettings::defaultSettings()->setAttribute( QWebEngineSettings::HyperlinkAuditingEnabled, true );
//    QWebEngineSettings::defaultSettings()->setAttribute( QWebEngineSettings::PluginsEnabled, true );

    /// 不设置大小，由系统内部自动管理
//    const int nMaximumSize = 100*1024*1024; // 100MB
//    QWebEngineProfile::defaultProfile()->setHttpCacheMaximumSize( nMaximumSize );
//    QWebEngineProfile::defaultProfile()->settings()->setDefaultTextEncoding();
//    QWebEngineSettings::globalSettings()

    DialogMainFrame mainFrame;
    const int ret = mainFrame.exec();
#ifdef USES_CEF
    CefQuit()
#endif
    MainWindow w;
//    w.show();
//    const int ret = a.exec();
    // exit
//    theEBAppClient.EB_UnInit();
    theApp.reset();

    return ret;
}
