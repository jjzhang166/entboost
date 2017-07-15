#ifndef EBWIDGETCHATINPUT_H
#define EBWIDGETCHATINPUT_H

#include "ebclientapp.h"
#include <QWidget>
#include <QPushButton>
class EbTextEdit;

class EbWidgetChatInput : public QWidget
{
    Q_OBJECT
public:
    explicit EbWidgetChatInput(const EbcCallInfo::pointer& pCallInfo,QWidget *parent = 0);

    void setFocusInput(void);
    void setCallInfo(const EbcCallInfo::pointer& pCallInfo);    /// 主要用于更新 CALLID
    void onUserExit(eb::bigint nFromAccount);
    void onReceiveRich(void);

signals:
    void clickedClose(void);

public slots:
    void onDropSendFileTextEdit(const QString &filePath);
    void onClickedPushButtonEmotion(void);
    void onClickedPushButtonMsgRecord(void);
    void onClickedPushButtonClose(void);
    void onClickedPushButtonSend(void);

    void onSelectedResourceEmotion(const QIcon& icon,mycp::bigint resourceId,const QString& resourceCmInfo,const QString & resourceFilePath);

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void contextMenuEvent(QContextMenuEvent *e);

private:
    EbTextEdit* m_textEditInput;
    QPushButton* m_pushButtonEmotion;
    QPushButton* m_pushButtonMsgRecord;
    QPushButton* m_pushButtonClose;
    QPushButton* m_pushButtonSend;
    QMenu * m_menuTextEdit;
    EbcCallInfo::pointer m_callInfo;
};

#endif // EBWIDGETCHATINPUT_H
