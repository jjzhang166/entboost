#ifndef DIALOGFRAMELIST_H
#define DIALOGFRAMELIST_H

#include "ebclientapp.h"
#include <EbDialogBase.h>
#include "ebframelist.h"

namespace Ui {
class DialogFrameList;
}
class DialogChatBase;

const int const_frame_height = 36;
const int const_border_left = 140;//135;
const int const_hide_border_offiice = 36;                        // 专业办公模式
const int const_hide_border_chat = const_hide_border_offiice+6;    // 经典聊天模式


class DialogFrameList : public EbDialogBase
        , public EbFrameList
{
    Q_OBJECT

public:
    explicit DialogFrameList(QWidget *parent = 0);
    ~DialogFrameList();

    void updateLocaleInfo(void);
    DialogWorkFrame * showWorkFrame(void);
    void addFrameItem(const EbFrameItem::pointer& frameItem,bool bShow, bool bAutoCall, bool bForceAddToList = false);
    void showFrameItem(mycp::bigint callId);
    void showFrameList(bool show=true);

public slots:
    void onClickedPushButtonShowWorkFrame(void);
    void onClickedPushButtonShowHideLeft(void);
    void onClickedButtonLeft(bool checked);
    void onClickedCloseFromDialogChatBase(void);

    void onCheckedFrameitem(const EbFrameItem*);
    void onWorkItemSizeChange(bool fromAboutBlank,int size);
protected:
    virtual void contextMenuEvent(QContextMenuEvent *);
    virtual void moveEvent(QMoveEvent *event);
    virtual void resizeEvent(QResizeEvent *);
//    virtual bool event(QEvent *event);
//    virtual void customEvent(QEvent *e);
    void checkOneSecond(void);
    virtual void timerEvent(QTimerEvent *event);

private:
    void updateShowHideIcon(void);
    void updateSize(void);

private:
    Ui::DialogFrameList *ui;
    bool m_showedLeft;
    int m_timerIdCheckState;
    QMenu * m_menuContext;
    QAction * m_actionShowIconOnly;
};

#endif // DIALOGFRAMELIST_H
