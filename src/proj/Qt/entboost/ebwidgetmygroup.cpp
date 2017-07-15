#include "ebwidgetmygroup.h"
#include "iconhelper.h"
#include "ebtreewidgetitem.h"
#include "ebmessagebox.h"

EbWidgetMyGroup::EbWidgetMyGroup(QWidget *parent) : QWidget(parent)
  , m_timerIdCheckState(0)
  , m_contextMenu(0)
{
    //    m_treeWidgetMyGroup->setRootIsDecorated( false ); ///去掉虚线边框（这是去掉根ITEM前面图标）
    m_treeWidgetMyGroup = new QTreeWidget(this);
//    m_treeWidgetMyGroup->setSortingEnabled(false);
//    m_treeWidgetMyGroup->header()->setSortIndicatorShown(false);
//    m_treeWidgetMyGroup->header()->setSortIndicator(0, Qt::AscendingOrder);
    m_treeWidgetMyGroup->setFrameStyle(QFrame::NoFrame); //去掉边框
    m_treeWidgetMyGroup->setHeaderHidden(true);
    m_treeWidgetMyGroup->setIconSize(const_tree_icon_size);
    m_treeWidgetMyGroup->setExpandsOnDoubleClick(false);
    m_treeWidgetMyGroup->setMouseTracking(false);
    m_treeWidgetMyGroup->setFrameShape(QFrame::NoFrame);
    m_treeWidgetMyGroup->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeWidgetMyGroup->setAllColumnsShowFocus(false);
    m_treeWidgetMyGroup->setWordWrap(true);
    m_treeWidgetMyGroup->setColumnCount(1);
    //    m_treeWidgetMyGroup->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    connect( m_treeWidgetMyGroup,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(onItemClicked(QTreeWidgetItem*,int)) );
    connect( m_treeWidgetMyGroup,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(onItemClicked(QTreeWidgetItem*,int)) );
    m_treeWidgetMyGroup->setMouseTracking(true);
    connect( m_treeWidgetMyGroup,SIGNAL(itemEntered(QTreeWidgetItem*,int)),this,SLOT(onItemEntered(QTreeWidgetItem*,int)) );
    //    connect( m_treeWidgetMyGroup,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)) );

    /// “打开会话”按钮
    m_pushButtonCall = new QPushButton(this);
    m_pushButtonCall->setParent( m_treeWidgetMyGroup );
    m_pushButtonCall->setCheckable(false);
    m_pushButtonCall->setAutoDefault(false);
    m_pushButtonCall->setFlat(true);
    m_pushButtonCall->setVisible(false);
    m_pushButtonCall->setObjectName("CallButton");
    m_pushButtonCall->setToolTip( theLocales.getLocalText("main-frame.button-call.tooltip","open chat") );
    connect( m_pushButtonCall, SIGNAL(clicked()),this,SLOT(onClickedPushButtonCall()) );
    IconHelper::Instance()->SetIcon(m_pushButtonCall,QChar(0xf27a),12 );
    /// “修改我的名片”
    m_pushButtonEdit = new QPushButton(this);
    m_pushButtonEdit->setParent( m_treeWidgetMyGroup );
    m_pushButtonEdit->setCheckable(false);
    m_pushButtonEdit->setAutoDefault(false);
    m_pushButtonEdit->setFlat(true);
    m_pushButtonEdit->setVisible(false);
    m_pushButtonEdit->setObjectName("CallButton");
    m_pushButtonEdit->setToolTip( theLocales.getLocalText("main-frame.button-edit.tooltip","edit member info") );
    connect( m_pushButtonEdit, SIGNAL(clicked()),this,SLOT(onClickedPushButtonEdit()) );
    IconHelper::Instance()->SetIcon(m_pushButtonEdit,QChar(0xf2c3),12 );

    /// for test data
    //    QTreeWidgetItem * topItem0 = new QTreeWidgetItem(m_treeWidgetMyGroup, QStringList(QString("Top Item 0")));
    //    QTreeWidgetItem * topItem1 = new QTreeWidgetItem(m_treeWidgetMyGroup, QStringList(QString("Top Item 1")));
    //    QTreeWidgetItem * topItem2 = new QTreeWidgetItem(m_treeWidgetMyGroup, QStringList(QString("Top Item 2")));
    //    m_treeWidgetMyGroup->insertTopLevelItem( 0,topItem0 );
    //    m_treeWidgetMyGroup->insertTopLevelItem( 1,topItem1 );
    //    m_treeWidgetMyGroup->insertTopLevelItem( 2,topItem2 );

    //    const QSize const_tree_icon_size(32,32);
    ////    QIcon icon;
    //    for (int i=0; i<3; i++) {
    //        QTreeWidgetItem * topItem0Sub0 = new QTreeWidgetItem(topItem0, QStringList(QString("Top Item 0 Sub")));
    //        topItem0Sub0->setIcon( 0, QIcon( QPixmap::fromImage(QImage(QStringLiteral(":/res/imgtip"))).scaled(const_tree_icon_size,Qt::IgnoreAspectRatio, Qt::SmoothTransformation) ) );

    //        topItem0->addChild(topItem0Sub0);
    //        QTreeWidgetItem * topItem1Sub0 = new QTreeWidgetItem(topItem1, QStringList(QString("Top Item 1 Sub")));
    //        topItem1Sub0->setIcon( 0, QIcon( QPixmap::fromImage(QImage(QStringLiteral(":/res/imgerror"))).scaled(const_tree_icon_size,Qt::IgnoreAspectRatio, Qt::SmoothTransformation) ) );
    //        topItem1->addChild(topItem1Sub0);

    //    }

    m_timerIdCheckState = this->startTimer(200);
}

EbWidgetMyGroup::~EbWidgetMyGroup()
{
    if (m_timerIdCheckState!=0) {
        killTimer(m_timerIdCheckState);
        m_timerIdCheckState = 0;
    }
//    m_actionList.clear();
}

void EbWidgetMyGroup::SetMemberInfo(const EbWidgetItemInfo::pointer& pGroupItemInfo, const EB_MemberInfo* memberInfo, SORT_ITEMS_FLAG nSortItems)
{
    if (pGroupItemInfo.get()==0 || memberInfo==0 || memberInfo->m_sMemberCode==0) {
        return;
    }

    int nSubType = 0;
    if (theApp->isEnterpriseCreateUserId(memberInfo->m_nMemberUserId))
        nSubType = 11;
    else if (theApp->m_ebum.EB_IsGroupCreator(memberInfo->m_sGroupCode, memberInfo->m_nMemberUserId))
        nSubType = 10;
    else if (theApp->m_ebum.EB_IsGroupManager(memberInfo->m_sGroupCode, memberInfo->m_nMemberUserId))
        nSubType = 10;
    else if (theApp->m_ebum.EB_IsGroupAdminLevel(memberInfo->m_sGroupCode, memberInfo->m_nMemberUserId))
        nSubType = 9;
    else if (theApp->m_ebum.EB_GetLogonUserId()==memberInfo->m_nMemberUserId)
        nSubType = 1;
    else
        nSubType = 0;

    QString sText;
    if (memberInfo->m_sJobTitle.empty())
        sText = memberInfo->m_sUserName.c_str();
    else
        sText = QString("%1 - %2").arg(memberInfo->m_sUserName.c_str()).arg(memberInfo->m_sJobTitle.c_str());
    EbWidgetItemInfo::pointer itemInfo;
    if (!m_pEmpItemInfo.find(memberInfo->m_sMemberCode, itemInfo)) {
        EbTreeWidgetItem * hEmpItem = new EbTreeWidgetItem(pGroupItemInfo->m_hItem, QStringList(sText));
        itemInfo = EbWidgetItemInfo::create(EbWidgetItemInfo::ITEM_TYPE_MEMBER,hEmpItem);
        hEmpItem->m_itemInfo = itemInfo;
        pGroupItemInfo->m_hItem->addChild(hEmpItem);
        m_pEmpItemInfo.insert(memberInfo->m_sMemberCode, itemInfo);
        if (nSortItems==AUTO_SORT) {
            nSortItems=FORCE_SORT;
        }
    }
    else {
        itemInfo->m_hItem->setText(1,sText);
        if ( nSortItems==AUTO_SORT &&
             (memberInfo->m_nLineState!=(EB_USER_LINE_STATE)itemInfo->m_dwItemData ||
              itemInfo->m_nIndex!=memberInfo->m_nDisplayIndex ||
              itemInfo->m_sName!=memberInfo->m_sUserName) ||
             itemInfo->m_nSubType!=nSubType) {
            nSortItems=FORCE_SORT;
        }
    }

    itemInfo->updateMemberInfo(memberInfo);

    /// ?? 这里要实现，状况改变
    if (nSortItems==FORCE_SORT) {
        pGroupItemInfo->m_hItem->sortChildren( 1,Qt::AscendingOrder );
    }
}

void EbWidgetMyGroup::onGroupInfo(const EB_GroupInfo * pGroupInfo)
{
    std::vector<EB_MemberInfo> pOutMemberInfoList;
    if (!theApp->m_ebum.EB_GetGroupMemberInfoList(pGroupInfo->m_sGroupCode,pOutMemberInfoList)) {
        return;
    }

    QString sText = QString("%1(%2)").arg(pGroupInfo->m_sGroupName.c_str()).arg(pGroupInfo->m_nEmpCount);
    bool bSortItems = false;
    EbWidgetItemInfo::pointer pDepItemInfo;
    if ( !m_pDepItemInfo.find(pGroupInfo->m_sGroupCode, pDepItemInfo) ) {
        bSortItems = true;
        EbTreeWidgetItem * hGroupItem = new EbTreeWidgetItem(m_treeWidgetMyGroup, QStringList(sText));
        pDepItemInfo = EbWidgetItemInfo::create(EbWidgetItemInfo::ITEM_TYPE_GROUP,hGroupItem);
        pDepItemInfo->m_sEnterpriseCode = pGroupInfo->m_sEnterpriseCode;
        pDepItemInfo->m_sGroupCode = pGroupInfo->m_sGroupCode;
        pDepItemInfo->m_nSubType = pGroupInfo->m_nGroupType;
        pDepItemInfo->m_dwItemData = 0;
        hGroupItem->m_itemInfo = pDepItemInfo;
        m_treeWidgetMyGroup->addTopLevelItem(hGroupItem);
        m_pDepItemInfo.insert(pGroupInfo->m_sGroupCode,pDepItemInfo);
    }
    else {
        if (pDepItemInfo->m_nIndex!=pGroupInfo->m_nDisplayIndex || pDepItemInfo->m_sName != pGroupInfo->m_sGroupName)
            bSortItems = true;
    }
    pDepItemInfo->m_nBigId = pGroupInfo->m_nMyEmpId;
    pDepItemInfo->m_sName = pGroupInfo->m_sGroupName;
    pDepItemInfo->m_nIndex = pGroupInfo->m_nDisplayIndex;
    /// **需要加载，否则会导致成员显示不完整；
    for (size_t i=0;i<pOutMemberInfoList.size(); i++) {
        const EB_MemberInfo& pMemberInfo = pOutMemberInfoList[i];
        if (pMemberInfo.m_sMemberCode==0) continue;
        SetMemberInfo(pDepItemInfo, &pMemberInfo,DISABLE_SORT);
    }
    if ( !pOutMemberInfoList.empty() ) {
        pDepItemInfo->m_hItem->sortChildren( 0,Qt::AscendingOrder );
    }
    const int nMemberSize = pGroupInfo->m_nEmpCount;
    const int nOnlineSize = theApp->m_ebum.EB_GetGroupOnlineSize(pGroupInfo->m_sGroupCode,1);
    if (nOnlineSize>=0)
        sText = QString("%1 [%2/%3]").arg(pGroupInfo->m_sGroupName.c_str()).arg(nOnlineSize).arg(nMemberSize);
    else
        sText = QString("%1 [%2]").arg(pGroupInfo->m_sGroupName.c_str()).arg(nMemberSize);
    pDepItemInfo->m_hItem->setText(0,sText);
    if (bSortItems) {
        m_treeWidgetMyGroup->sortItems( 0,Qt::AscendingOrder );
    }

//    if (m_treeDepartment.GetSelectedItem()==NULL)
//    {
//        KillTimer(TIMERID_CHECK_SELECT_FIRST);
//        SetTimer(TIMERID_CHECK_SELECT_FIRST,2000,NULL);
//    }

}

void EbWidgetMyGroup::onMemberInfo(const EB_MemberInfo *pMemberInfo,bool bChangeLineState)
{
    EbWidgetItemInfo::pointer pDepItemInfo;
    if (!m_pDepItemInfo.find(pMemberInfo->m_sGroupCode,pDepItemInfo)) {
        return;
    }
    /// 已经加载成员
    if (pDepItemInfo->m_dwItemData==0)
        pDepItemInfo->m_dwItemData = 1;
    SetMemberInfo(pDepItemInfo, pMemberInfo);
    if (bChangeLineState) {
        CEBString sGroupName;
        if (theApp->m_ebum.EB_GetGroupName(pMemberInfo->m_sGroupCode,sGroupName)) {
            int nMemberSize = 0;
            int nOnlineSize = 0;
            theApp->m_ebum.EB_GetGroupMemberSize(pMemberInfo->m_sGroupCode,1,nMemberSize,nOnlineSize);
            QString sText;
            if (nOnlineSize>=0)
                sText = QString("%1 [%2/%3]").arg(sGroupName.c_str()).arg(nOnlineSize).arg(nMemberSize);
            else
                sText = QString("%1 [%2]").arg(sGroupName.c_str()).arg(nMemberSize);
            pDepItemInfo->m_hItem->setText(0,sText);
        }
    }
}

void EbWidgetMyGroup::onRemoveGroup(const EB_GroupInfo *pGroupInfo)
{
    //EB_GroupInfo pGroupInfo;
    //if (theApp->m_ebum.EB_GetGroupInfo(sDepCode,&pGroupInfo))
    {
        std::vector<eb::bigint> pOutMemberCodeList;
        if (theApp->m_ebum.EB_GetGroupMemberCodeList(pGroupInfo->m_sGroupCode,pOutMemberCodeList)) {
            for (size_t i=0;i<pOutMemberCodeList.size(); i++) {
                eb::bigint nMemberCode = pOutMemberCodeList[i];
                deleteMemberInfo( pGroupInfo,nMemberCode,true );
            }
        }
    }

    EbWidgetItemInfo::pointer pDepItemInfo;
    if (m_pDepItemInfo.find(pGroupInfo->m_sGroupCode, pDepItemInfo, true)) {
        QTreeWidgetItem * item = m_treeWidgetMyGroup->takeTopLevelItem( m_treeWidgetMyGroup->indexOfTopLevelItem(pDepItemInfo->m_hItem) );
        if (item!=0) {
            delete item;
        }
    }
}

void EbWidgetMyGroup::deleteMemberInfo(const EB_GroupInfo *pGroupInfo, eb::bigint nMemberCode, bool fromDeleteGroup)
{
    eb::bigint nGroupCode = pGroupInfo->m_sGroupCode;
    EbWidgetItemInfo::pointer pDepItemInfo;
    if (m_pDepItemInfo.find(nGroupCode, pDepItemInfo)) {
        EbWidgetItemInfo::pointer pEmpItemInfo;
        if (m_pEmpItemInfo.find(nMemberCode, pEmpItemInfo, true)) {
            pDepItemInfo->m_hItem->removeChild(pEmpItemInfo->m_hItem);
        }
        if (!fromDeleteGroup) {
            int nMemberSize = 0;
            int nOnlineSize = 0;
            theApp->m_ebum.EB_GetGroupMemberSize(nGroupCode,1,nMemberSize,nOnlineSize);
            const QString sText = QString("%1 [%2/%3]").arg(pGroupInfo->m_sGroupName.c_str()).arg(nOnlineSize).arg(nMemberSize);
            pDepItemInfo->m_hItem->setText(0,sText);
        }
    }
}

//void EbWidgetMyGroup::deleteGroupInfo(const EB_GroupInfo *pGroupInfo)
//{
//    {
//        std::vector<eb::bigint> pOutMemberCodeList;
//        if (theApp->m_ebum.EB_GetGroupMemberCodeList(pGroupInfo->m_sGroupCode,pOutMemberCodeList))
//        {
//            for (size_t i=0;i<pOutMemberCodeList.size(); i++)
//            {
//                eb::bigint nMemberCode = pOutMemberCodeList[i];
//                DeleteEmployeeInfo(pGroupInfo,nMemberCode,true);
//            }
//        }
//    }

//    EbWidgetItemInfo::pointer pDepItemInfo;
//    if (m_pDepItemInfo.find(pGroupInfo->m_sGroupCode, pDepItemInfo, true))
//    {
//        m_treeDepartment.DeleteItem(pDepItemInfo->m_hItem);
//    }
//}

void EbWidgetMyGroup::resizeEvent(QResizeEvent * e)
{
    m_treeWidgetMyGroup->setGeometry( 0,0,width(),height() );

    QWidget::resizeEvent(e);
}

void EbWidgetMyGroup::timerEvent(QTimerEvent *event)
{
    if (m_timerIdCheckState!=0 && event->timerId()==m_timerIdCheckState) {
        if (m_pushButtonCall->isVisible()) {
            /// 实现定期自动判断当前鼠标位置，并刷新 call 按钮
            const QRect& rectTreeWidget = m_treeWidgetMyGroup->geometry();
            const QRect rectToDialog(m_treeWidgetMyGroup->pos(),rectTreeWidget.size());
            const QPoint pointMouseToDialog = this->mapFromGlobal(this->cursor().pos());
            if (!rectToDialog.contains(pointMouseToDialog)) {
                m_pushButtonCall->setVisible(false);
            }
        }
    }

    QWidget::timerEvent(event);
}

void EbWidgetMyGroup::createMenuData()
{
    if (m_contextMenu==0) {
        m_contextMenu = new EbContextMenu( EbContextMenu::MyGroup, this );
    }
}

void EbWidgetMyGroup::contextMenuEvent(QContextMenuEvent *e)
{
    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->itemAt(e->pos());
    if (item==0) {
        return;
    }
    createMenuData();
    const EbWidgetItemInfo::pointer itemInfo = item->m_itemInfo;
    if (!m_contextMenu->updateMenuItem(itemInfo)) {
        return;
    }

//    // 应用功能菜单
//    CNewMenu pPopupMenuApps;
//    if (nFuncLocation != EB_FUNC_LOCATION_UNKNOWN)
//    //if (theApp.m_nSelectUserId > 0 || theApp.m_nSelectGroupId > 0)
//    {
//        pPopupMenuApps.CreatePopupMenu();
//#ifdef USES_EBCOM_TEST
//        _variant_t pSubscribeFuncList = theEBClientCore->EB_GetSubscribeFuncList(nFuncLocation);
//        if (pSubscribeFuncList.vt!=VT_EMPTY && pSubscribeFuncList.parray != NULL)
//        {
//            CComSafeArray<VARIANT> m_sa(pSubscribeFuncList.parray);
//            for (ULONG i=0;i<m_sa.GetCount();i++)
//            {
//                CComVariant var = m_sa.GetAt(i);
//                if (var.vt != VT_DISPATCH)
//                    continue;
//                CComPtr<IEB_SubscribeFuncInfo> pSubscribeFuncInfo;
//                var.pdispVal->QueryInterface(__uuidof(IEB_SubscribeFuncInfo),(void**)&pSubscribeFuncInfo);
//                if (pSubscribeFuncInfo == NULL) continue;
//                theApp.m_pSubscribeFuncList.push_back(EB_SubscribeFuncInfo(pSubscribeFuncInfo));
//            }
//        }
//#else
//        theApp->m_ebum.EB_GetSubscribeFuncList(nFuncLocation,theApp.m_pSubscribeFuncList);
//#endif
//        if (!theApp.m_pSubscribeFuncList.empty())
//        {
//            for (size_t i=0;i<theApp.m_pSubscribeFuncList.size();i++)
//            {
//                const EB_SubscribeFuncInfo & pSubscribeFuncInfo = theApp.m_pSubscribeFuncList[i];
//                bool bAppendMenuError = true;
//                if (::PathFileExists(pSubscribeFuncInfo.m_sResFile.c_str()))
//                {
//                    CImageEx pImageEx;
//                    pImageEx.LoadFilename(pSubscribeFuncInfo.m_sResFile.c_str());
//                    const int nWidth = pImageEx.GetWidth();
//                    const int nHeight = pImageEx.GetHeight();
//                    if (nWidth>0 && nHeight>0)
//                    {
//                        HGDIOBJ hBitmap = NULL;
//                        if ((nWidth/nHeight)>2)
//                        {
//                            if (nHeight>const_default_menu_image_size)
//                                hBitmap = pImageEx.Copy(0,0,const_default_menu_image_size,const_default_menu_image_size);
//                            else
//                                hBitmap = pImageEx.Copy(0,0,nHeight,nHeight);
//                        }else
//                        {
//                            if (nHeight!=const_default_menu_image_size)
//                                hBitmap = pImageEx.StrctchCopy(0,0,const_default_menu_image_size,const_default_menu_image_size);
//                            else
//                                hBitmap = pImageEx.Copy(0,0,const_default_menu_image_size,const_default_menu_image_size);
//                        }
//                        if (hBitmap!=NULL)
//                        {
//                            CBitmap pBitmap;
//                            pBitmap.Attach(hBitmap);
//                            pPopupMenuApps.AppendODMenu(pSubscribeFuncInfo.m_sFunctionName.c_str(),MF_BYCOMMAND,EB_COMMAND_SUBSCRIBE_FUNC+i,&pBitmap);
//                            bAppendMenuError = false;
//                        }
//                    }
//                }
//                if (bAppendMenuError)
//                    pPopupMenuApps.AppendMenu(MF_BYCOMMAND,EB_COMMAND_SUBSCRIBE_FUNC+i,pSubscribeFuncInfo.m_sFunctionName.c_str());
//            }
//            m_menu2.AppendMenu(MF_SEPARATOR);
//            m_menu2.InsertODMenu(-1,_T("集成应用"),MF_POPUP|MF_BYPOSITION,(UINT)pPopupMenuApps.m_hMenu,IDB_BITMAP_SELECT_COLOR);
//        }
//    }

    m_contextMenu->exec( e->globalPos() );
}

//void EbWidgetMyGroup::onCallItem(QTreeWidgetItem* aitem)
//{
//    if (aitem==0) return;
//    const EbTreeWidgetItem *item = (EbTreeWidgetItem*)aitem;
//    if (item->m_itemInfo->m_nItemType == EbWidgetItemInfo::ITEM_TYPE_MEMBER) {
//        theApp->m_pAutoCallFromUserIdList.remove(item->m_itemInfo->m_nUserId);
//        theApp->m_ebum.EB_CallMember(item->m_itemInfo->m_sMemberCode,0);
//    }
//    else if (item->m_itemInfo->m_nItemType == EbWidgetItemInfo::ITEM_TYPE_GROUP) {
//        theApp->m_pAutoCallGroupIdList.remove(item->m_itemInfo->m_sGroupCode);
//        theApp->m_ebum.EB_CallGroup(item->m_itemInfo->m_sGroupCode);
//    }
//}

//void EbWidgetMyGroup::onEditItem(QTreeWidgetItem *aitem)
//{
//    if (aitem==0) return;
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)aitem;
//    EB_MemberInfo pMemberInfo;
//    if (theApp->m_ebum.EB_GetMemberInfoByUserId(&pMemberInfo,item->m_itemInfo->m_sGroupCode,item->m_itemInfo->m_nUserId)) {
//        theApp->editMemberInfo(&pMemberInfo);
//    }
//}

void EbWidgetMyGroup::onItemClicked(QTreeWidgetItem *item, int column)
{
    const EbTreeWidgetItem* itemEb = (EbTreeWidgetItem*)item;
    const EbWidgetItemInfo::ITEM_TYPE itemType = itemEb->m_itemInfo->m_nItemType;
    if (itemType==EbWidgetItemInfo::ITEM_TYPE_GROUP) {
        theApp->m_ebum.EB_LoadGroup(itemEb->m_itemInfo->m_sGroupCode,true);
        if (item->isExpanded()) {
            m_treeWidgetMyGroup->collapseItem(item);
        }
        else {
            m_treeWidgetMyGroup->expandItem(item);
        }
    }
}
void EbWidgetMyGroup::onItemEntered(QTreeWidgetItem *item, int column)
{
    if (!m_treeWidgetMyGroup->hasFocus()) {
        m_treeWidgetMyGroup->setFocus();
    }
//    m_treeWidgetMyGroup->setCurrentItem(item);
    /// 滚动条能正常显示
    const QRect rectItem = m_treeWidgetMyGroup->visualItemRect(item);
    const QPoint pointItem = m_treeWidgetMyGroup->mapToParent(rectItem.topRight());
    const int y = pointItem.y();
    /// -2（配合下面的 y+1）实现删除按钮显示时，保留ITEM边框，
    const int buttonSize = rectItem.height()-2;
    const EbTreeWidgetItem* itemEb = (EbTreeWidgetItem*)item;
    if ( itemEb->m_itemInfo->m_nUserId==theApp->m_ebum.EB_GetLogonUserId() ) {
        m_pushButtonEdit->setGeometry( pointItem.x()-buttonSize,y+1,buttonSize,buttonSize );
        m_pushButtonEdit->setProperty("track-item",QVariant((quint64)item));
        m_pushButtonEdit->setVisible(true);
        m_pushButtonCall->hide();
    }
    else {
        m_pushButtonCall->setGeometry( pointItem.x()-buttonSize,y+1,buttonSize,buttonSize );
        m_pushButtonCall->setProperty("track-item",QVariant((quint64)item));
        m_pushButtonCall->setVisible(true);
        m_pushButtonEdit->hide();
    }
}

void EbWidgetMyGroup::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
//    const QRect rectItem = m_treeWidgetMyGroup->visualItemRect(current);
//    const QPoint pointItem = m_treeWidgetMyGroup->mapToParent(rectItem.topRight());
//    const int rowIndexItem = 0;
////    const int rowIndexItem = listWidgetLoginRecords->row(item);
//    const QRect& rect = m_treeWidgetMyGroup->geometry();
//    const int y = rect.top()+rowIndexItem*rectItem.height()+0;  // 0表示 m_treeWidgetMyGroup 没有边框
//    // -2（配合下面的 buttonSize-1和y+1）实现删除按钮显示时，保留ITEM边框，
//    const int buttonSize = rectItem.height()-2;
//    m_pushButtonCall->setGeometry( rect.right()-buttonSize-1,y+1,buttonSize,buttonSize );
//    m_pushButtonCall->setProperty("call-item",QVariant(rowIndexItem));
//    m_pushButtonCall->setVisible(true);
}

void EbWidgetMyGroup::onClickedPushButtonCall(void)
{
    const EbTreeWidgetItem * item = (EbTreeWidgetItem*)m_pushButtonCall->property("track-item").toULongLong();
    if (item!=0) {
        createMenuData();
        m_contextMenu->onCallItem(item->m_itemInfo);
    }
//    onCallItem((QTreeWidgetItem*)m_pushButtonCall->property("track-item").toULongLong());
    m_pushButtonCall->hide();
    m_pushButtonCall->setProperty("track-item",QVariant((quint64)0));
    m_treeWidgetMyGroup->setFocus();
}

void EbWidgetMyGroup::onClickedPushButtonEdit()
{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_pushButtonEdit->property("track-item").toULongLong();
//    if (item==0) {
//        return;
//    }
    m_pushButtonEdit->hide();
    m_pushButtonEdit->setProperty("track-item",QVariant((quint64)0));
    m_treeWidgetMyGroup->setFocus();
//    onEditItem((QTreeWidgetItem*)item);
    const EbTreeWidgetItem * item = (EbTreeWidgetItem*)m_pushButtonCall->property("track-item").toULongLong();
    if (item!=0) {
        createMenuData();
        m_contextMenu->onEditItem(item->m_itemInfo);
    }
}

//void EbWidgetMyGroup::onTriggeredActionNewGroup()
//{
//    theApp->newGroupInfo(EB_GROUP_TYPE_GROUP);
//}

//void EbWidgetMyGroup::onTriggeredActionEditGroup()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_sGroupCode==0) return;
//    theApp->editGroupInfo(item->m_itemInfo->m_sGroupCode);
//}

//void EbWidgetMyGroup::onTriggeredActionDeleteGroup()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_GROUP) return;

//    EB_GroupInfo pGroupInfo;
//    if (!theApp->m_ebum.EB_GetGroupInfo(item->m_itemInfo->m_sGroupCode,&pGroupInfo)) {
//        return;
//    }
//    if ( pGroupInfo.m_nCreateUserId != theApp->logonUserId() ) {
//        /// 不是部门创建者，不能删除
//        return;
//    }
//    else if (pGroupInfo.m_nGroupType == EB_GROUP_TYPE_DEPARTMENT			/// 企业部门
//        && theApp->m_ebum.EB_GetGroupMemberSize(item->m_itemInfo->m_sGroupCode,1)>0) {		/// 不为空，不能删除
//        return;
//    }
////    if (this->m_pViewContactInfo && m_pViewContactInfo->IsWindowVisible()) {
////        m_pViewContactInfo->HideReset();
////    }

//    QString title = theLocales.getLocalText("message-box.delete-group.title","Delete Group");
//    if (title.isEmpty())
//        title = theApp->productName();
//    else {
//        title.replace( "[GROUP_TYPE_NAME]", theLocales.getGroupTypeName((int)pGroupInfo.m_nGroupType)->name().c_str() );
//    }
//    /// 确定解散：\r\n%s 吗？
//    QString text = theLocales.getLocalText("message-box.delete-group.text","Confirm Delete Group?");
//    text.replace( "[GROUP_NAME]", pGroupInfo.m_sGroupName.c_str() );
//    text.replace( "[GROUP_ID]", QString::number(pGroupInfo.m_sGroupCode) );
//    const int ret = EbMessageBox::doExec( 0,title, QChar::Null, text, EbMessageBox::IMAGE_QUESTION );
//    if (ret==QDialog::Accepted) {
//        theApp->m_ebum.EB_DeleteGroup(item->m_itemInfo->m_sGroupCode);
//    }
//}

//void EbWidgetMyGroup::onTriggeredActionExitGroup()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_GROUP) return;

//    EB_GroupInfo pGroupInfo;
//    if (!theApp->m_ebum.EB_GetGroupInfo(item->m_itemInfo->m_sGroupCode,&pGroupInfo)) {
//        return;
//    }
//    else if ( pGroupInfo.m_nCreateUserId == theApp->logonUserId() ) {
//        /// 部门创建者，不能退出
//        return;
//    }
//    QString title = theLocales.getLocalText("message-box.exit-group.title","Exit Group");
//    if (title.isEmpty())
//        title = theApp->productName();
//    else {
//        title.replace( "[GROUP_TYPE_NAME]", theLocales.getGroupTypeName((int)pGroupInfo.m_nGroupType)->name().c_str() );
//    }
//    /// 确定退出：\r\n%s 吗？
//    QString text = theLocales.getLocalText("message-box.exit-group.text","Confirm Exit Group?");
//    text.replace( "[GROUP_NAME]", pGroupInfo.m_sGroupName.c_str() );
//    text.replace( "[GROUP_ID]", QString::number(pGroupInfo.m_sGroupCode) );
//    const int ret = EbMessageBox::doExec( 0,title, QChar::Null, text, EbMessageBox::IMAGE_QUESTION );
//    if (ret==QDialog::Accepted) {
//        theApp->m_ebum.EB_ExitGroup(item->m_itemInfo->m_sGroupCode);
//    }
//}

//void EbWidgetMyGroup::onTriggeredActionGroupAddForbidSpeech()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_GROUP) return;
//    /// 群组禁言
//    theApp->m_ebum.EB_SetGroupForbidSpeech(item->m_itemInfo->m_sGroupCode,true);

//}

//void EbWidgetMyGroup::onTriggeredActionGroupDelForbidSpeech()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_GROUP) return;
//    /// 解除群组禁言
//    theApp->m_ebum.EB_SetGroupForbidSpeech(item->m_itemInfo->m_sGroupCode,false);
//}

//void EbWidgetMyGroup::onTriggeredActionEditMember()
//{
//    onEditItem( m_treeWidgetMyGroup->currentItem() );
//}

//void EbWidgetMyGroup::onTriggeredActionDeleteMember()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_MEMBER) return;

//    EB_GroupInfo pGroupInfo;
//    if (!theApp->m_ebum.EB_GetGroupInfo(item->m_itemInfo->m_sGroupCode,&pGroupInfo))
//        return;
//    if (item->m_itemInfo->m_nUserId == pGroupInfo.m_nCreateUserId)
//    {
//        // 不能移除管理者自己
//        return;
//    //}else if (pGroupInfo.m_nCreateUserId!=theApp.GetLogonUserId())
//    //{
//    //	CDlgMessageBox::EbDoModal(this,"移除成员：","没有操作权限！",CDlgMessageBox::IMAGE_WARNING);
//    //	return;
//    }

//    QString title = theLocales.getLocalText("message-box.delete-member.title","移除成员");
//    if (title.isEmpty())
//        title = theApp->productName();
//    /// 确定移除：\r\n%s(%s) 吗？
//    QString text = theLocales.getLocalText("message-box.delete-member.text","Confirm Delete Member?");
//    text.replace( "[USER_ACCOUNT]", item->m_itemInfo->m_sAccount.string().c_str() );
//    char lpszUserId[24];
//    sprintf(lpszUserId,"%lld", item->m_itemInfo->m_nUserId);
//    text.replace( "[USER_ID]", lpszUserId );
//    const int ret = EbMessageBox::doExec( 0,title, QChar::Null, text, EbMessageBox::IMAGE_QUESTION );
//    if (ret==QDialog::Accepted) {
//        theApp->m_ebum.EB_DeleteMember(item->m_itemInfo->m_sMemberCode);
//    }
//}

//void EbWidgetMyGroup::onTriggeredActionMemberAddAdminLevel()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_MEMBER) return;
//    theApp->m_ebum.EB_AddGroupAdminLevel(item->m_itemInfo->m_sGroupCode,item->m_itemInfo->m_nUserId);
//}

//void EbWidgetMyGroup::onTriggeredActionMemberDelAdminLevel()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_MEMBER) return;
//    theApp->m_ebum.EB_DelGroupAdminLevel(item->m_itemInfo->m_sGroupCode,item->m_itemInfo->m_nUserId);

//}

//void EbWidgetMyGroup::onTriggeredActionMemberAddForbidSpeech()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_MEMBER) return;
//    const QAction * action = (QAction*)sender();
//    const int forbidMinutes = action->data().toInt();
//    theApp->m_ebum.EB_SetMemberForbidSpeech(item->m_itemInfo->m_sGroupCode,item->m_itemInfo->m_nUserId, true, forbidMinutes);

//}

//void EbWidgetMyGroup::onTriggeredActionMemberDelForbidSpeech()
//{
//    const EbTreeWidgetItem* item = (EbTreeWidgetItem*)m_treeWidgetMyGroup->currentItem();
//    if (item==0 || item->m_itemInfo->m_nItemType!=EbWidgetItemInfo::ITEM_TYPE_MEMBER) return;
//    theApp->m_ebum.EB_SetMemberForbidSpeech(item->m_itemInfo->m_sGroupCode,item->m_itemInfo->m_nUserId, false);
//}

//void EbWidgetMyGroup::onTriggeredActionCallUser()
//{
//    onCallItem( m_treeWidgetMyGroup->currentItem() );
//}

//void EbWidgetMyGroup::onTriggeredActionViewMsgRecord()
//{

//}

//void EbWidgetMyGroup::onTriggeredActionViewGroupShare()
//{

//}

