#ifndef EBFRAMELIST_H
#define EBFRAMELIST_H

#include "ebframeitem.h"

class EbFrameList
{
public:
    EbFrameList(QWidget* parent);
    virtual ~EbFrameList(void);

    void timerCheckState(void);
    DialogWorkFrame * getWorkFrame(void) const;
    DialogChatBase::pointer getDialogChatBase(mycp::bigint nCallId, bool bRemove, bool bAutoCall);
    bool addUnreadMsg(eb::bigint nCallId, eb::bigint nMsgId);
    bool setUnreadMsg(eb::bigint nCallId, size_t nUnreadMsgCount);
//    void setUnreadMsg(CFrameWndInfo::FRAME_WND_TYPE nType, size_t nUnreadMsgCount);	// for FRAME_WND_WORK_FRAME
    bool existFrameItem(eb::bigint nCallId) const;

    void onUserLineStateChange(eb::bigint nGroupCode, eb::bigint nUserId, EB_USER_LINE_STATE nLineState);
    void onMemberInfo(const EB_MemberInfo* pMemberInfo, bool bChangeLineState);
    void onGroupInfo(const EB_GroupInfo* groupInfo);
    void onRemoveGroup(const EB_GroupInfo* groupInfo);
    void onRemoveMember(const EB_GroupInfo* groupInfo, eb::bigint nMemberId, mycp::bigint memberUserId);
    void onMemberHeadChange(const EB_MemberInfo * pMemberInfo);
    void onContactHeadChange(const EB_ContactInfo* pContactInfo);

protected:
    void addFrameItem(const EbFrameItem::pointer& frameItem,bool bShow, bool bAutoCall, bool bForceAddToList = false,int nInsertOffset=-1);
    void showByCallId(mycp::bigint callId);
    bool showByIndex(int index);
    DialogWorkFrame* showWorkFrame(void);
    void clickedLeftButton(const QPushButton* leftButton, const QPoint& pt);
    void showFirst(void);
    void closeItem(const DialogChatBase* chatBase);

    void checkShowHideCloseButton(const QPoint& pt);    /// 定期检查显示或隐藏关闭按钮
    void onResize(const QRect& rect);
    /// * 用于关闭某个ITEM，上下移动按钮位置
    void onMove(void);
    /// 主要用于移某些 parent=0 窗口
    void onMoveEvent(void);
    size_t size(void) const {return m_list.size();}

    /// 定期检查并关闭 ITEM对应窗口
    void checkCloseItem(void);

protected:
    QWidget* m_pParent;
    int m_leftWidth;
    CLockList<EbFrameItem::pointer> m_list;
    CLockList<EbFrameItem::pointer> m_hide;
    CLockList<EbFrameItem::pointer> m_close;

};

#endif // EBFRAMELIST_H
