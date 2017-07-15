#ifndef EBWORKLIST_H
#define EBWORKLIST_H

#include <ebworkitem.h>
#include <stl/locklist.h>

class EbWorkList
{
public:
    EbWorkList(QWidget * parent);
    virtual ~EbWorkList(void);

    bool isEmpty(void) const {return m_list.empty();}
    void setCheckedFocus(void);

    EbWidgetUserInfo * widgetUserInfo(void) const;
    EbWidgetUserList * widgetUserList(void) const;
    EbWidgetFileTranList * widgetTranFile(void) const;

protected:
    void addWorkItem(bool saveUrl,int topHeight,int leftOffset, const EbWorkItem::pointer& workItem,int nInsertOffset=-1);
    void showByIndex(int index, bool bSearchFocus);
    void setItemText(EbWorkItem::WORK_ITEM_TYPE itemType, const QString &text);
    void clickedTopButton(int leftOffset, const QPushButton* topButton, const QPoint& pt);
    void showFirst(void);
    int indexOf(const EbWidgetWorkView * view) const;
    int indexOf(EbWorkItem::WORK_ITEM_TYPE type, bool sendClose=false) const;
    void closeItem(int leftOffset, const EbWidgetWorkView * view);
    void closeItem(int leftOffset, int index);
//    void closeItem(int leftOffset, EbWorkItem::WORK_ITEM_TYPE type);
    void clear(void);
//    void closeItem(const DialogChatBase* chatBase);

    void checkShowHideCloseButton(const QPoint& pt);    /// 定期检查显示或隐藏关闭按钮
    int onResize(const QRect& rect, int topHeight, int leftOffset);
    /// * 用于关闭某个ITEM，左右移动按钮位置
    int onMove(int leftOffset);
//    int topButtonWidth(void) const;

    void back(void);
    void forward(void);
    bool canGoBack() const;
    bool canGoForward() const;
    void refreshOrStop(void);
    void saveUrlHistory(void);
//    void stop(void);
protected:
    virtual void onItemSizeChange(const EbWorkItem::pointer& fromWorkItem,int size,int right) {}

protected:
    QWidget* m_pParent;
    CLockList<EbWorkItem::pointer> m_list;
};

#endif // EBWORKLIST_H
