#ifndef EBTEXTBROWSER_H
#define EBTEXTBROWSER_H

#include <ebclientapp.h>
#include <QTextBrowser>
#include <QMouseEvent>

class EbFrameChatToolBar;
class EbTextBlockUserData;

class EbTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit EbTextBrowser(const EbcCallInfo::pointer& pCallInfo,QWidget* parent = Q_NULLPTR);
    virtual ~EbTextBrowser();

    void setCallInfo(const EbcCallInfo::pointer& pCallInfo);    /// 主要用于更新 CALLID
    void insertUrl(const QString & url);
    void insertPlainTextEb(const QString & text,const QColor & color);
    void addRichMsg(bool saveHistory, bool bReceive,const CCrRichInfo* pCrMsgInfo,EB_STATE_CODE nState=EB_STATE_OK,QString* sOutFirstMsg1=0,QString* sOutFirstMsg2=0);
    void addFileMsg(bool bReceive, const CCrFileInfo *fileInfo);
    void writeMsgDate(time_t tMsgTime);
    void moveTextBrowserToStart(void);
    void moveTextBrowserToEnd(void);
    void addLineString(eb::bigint msgId,const QString& sText);
    void updateMsgText(eb::bigint msgId, const QString &newText, int blockFrom, int blockTo=-1);
    void setMsgReceiptFlag(eb::bigint msgId, int receiptFlag);
    void loadHistoryMsg(int loadLastCount);
    void loadMsgRecord(const char * sql, bool desc);

public slots:
    void onAnchorClicked(const QUrl &);
    void onDeleteMessage(qint64 msgId);
//    void onCopyAvailable(bool b);

protected:
    void findBlockFromPosition(const QPoint & pos);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void timerEvent(QTimerEvent *);
//    void drawBlock(void);
//    virtual void paintEvent(QPaintEvent *e);

    EbTextBlockUserData * updateBlockMsgId(eb::bigint msgId);
    void getFromToName(bool bReceive, eb::bigint fromUserId, eb::bigint toUserId, tstring& pOutFromUserName, tstring & pOutToUserName);
    void writeTitle(bool writeLeft,eb::bigint nMsgId,bool bPrivate,eb::bigint nFromUid,const tstring& sFromName,eb::bigint nToUid,const tstring& sToName,time_t tMsgTime, int nReadFlag,QString *pOutWindowText=0);
    void writeFileMessage(eb::bigint msgId,eb::bigint resourceId, const char* filePath,eb::bigint fileSize,QString *pOutMsgText=0);
    void writeVoiceMessage(const char* voiceFile,QString *pOutMsgText=0);
    bool writeCardDataMessage( bool bReceive,mycp::bigint msgId, const char* cardData,QString *pOutMsgText=0);
    void addChatMsgBlock(eb::bigint msgId, bool alignLeft);

private:
    EbcCallInfo::pointer m_callInfo;
    int m_timerIdFindTextBlockFromPosition;
    EbFrameChatToolBar * m_chatToolBar;
    QDateTime m_tLastMsgDayTime;
//    CLockMap<mycp::bigint,std::string> m_cardInfoList;
    CLockMap<mycp::bigint,bool> m_pPrevReceivedFileMsgIdList;   /// msgid->
};

#endif // EBTEXTBROWSER_H
