#include "ebwidgetsearchresult.h"
#include "eblabel.h"

EbWidgetSearchResult::EbWidgetSearchResult(QWidget *parent) : QWidget(parent)
  , m_listWidgetResults(0)
{
    ///
    this->setWindowFlags( Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint );
//    this->setStyleSheet("QWidget:{border: none;}");

    m_listWidgetResults = new EbListWidget(this);
    connect( m_listWidgetResults,SIGNAL(keyPressEnter(const QListWidgetItem*)),this,SLOT(onKeyPressEnterItem(const QListWidgetItem*)) );
//    m_listWidgetResults->setMouseTracking(true);
//    connect( m_listWidgetResults,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(onItemClicked(QListWidgetItem*)) );
    m_menu = new QMenu(this);
    m_actionCopy = m_menu->addAction( "Copy" );
    m_actionCopy->setData( QVariant((int)EB_COMMAND_COPY) );
    connect( m_actionCopy, SIGNAL(triggered()), this, SLOT(onClickedContextMenu()) );
    /// ----------------------
    m_menu->addSeparator();
    m_actionDelete = m_menu->addAction( "Delete" );
    m_actionDelete->setData( QVariant((int)EB_COMMAND_DELETE_ITEM_RES) );
    connect( m_actionDelete, SIGNAL(triggered()), this, SLOT(onClickedContextMenu()) );

    updateLocaleInfo();
}

void EbWidgetSearchResult::search(const char *key)
{
    m_listWidgetResults->clear();
    tstring sSearchKey(key);
    CSqliteCdbc::escape_string_in(sSearchKey);
    const int nLimit = 20;//GetWorkFrameShowed()?25:20;
    char sql[2048];
    sprintf(sql, "select url,title,strftime('%%Y-%%m-%%d %%H:%%M',last_time,'localtime') "
                 "FROM url_record_t WHERE url LIKE '%%%s%%' OR title LIKE '%%%s%%' ORDER BY last_time DESC LIMIT %d",
            sSearchKey.c_str(),sSearchKey.c_str(),nLimit);
    bool sendSearchFirst = false;
    int nCookie = 0;
    theApp->m_sqliteUser->select(sql, nCookie);
    cgcValueInfo::pointer pRecord = theApp->m_sqliteUser->first(nCookie);
    while (pRecord.get()!=NULL) {
        tstring url(pRecord->getVector()[0]->getStrValue());
        tstring title(pRecord->getVector()[1]->getStrValue());
        const tstring lastTime(pRecord->getVector()[2]->getStrValue());
        pRecord = theApp->m_sqliteUser->next(nCookie);

//        CSqliteCdbc::escape_string_out(url);
//        CSqliteCdbc::escape_string_out(title);
        /// rgb(0,162,232)=00a2e8, rgb(128,128,128)=808080
        const QString label = QString("<font color=#00a2e8>&nbsp;%1</font> - <font color=#808080>%2</font>").arg(url.c_str()).arg(title.c_str());
        QListWidgetItem * pItem0 = new QListWidgetItem("", m_listWidgetResults);
        pItem0->setData( Qt::UserRole,QVariant(url) );
        m_listWidgetResults->addItem(pItem0);
        EbLabel * labelItem = new EbLabel(m_listWidgetResults);
        labelItem->setText(label);
        labelItem->setToolTip( lastTime.c_str() );
        labelItem->setProperty("url",QVariant(url));
        connect( labelItem,SIGNAL(clicked()),this,SLOT(onClickedItemLabel()) );
//        connect( labelItem,SIGNAL(keyPressEnter()),this,SLOT(onKeyPressEnterItemLabel()) );
        m_listWidgetResults->setItemWidget( pItem0,labelItem );
        if (!sendSearchFirst) {
            sendSearchFirst = true;
            m_listWidgetResults->setCurrentRow(0);
            emit searchFirst( QString(url.c_str()) );
        }
    }
    theApp->m_sqliteUser->reset(nCookie);
    checkShowHide();
}

void EbWidgetSearchResult::setFocusSelectFirst()
{
    if  ( this->isVisible() && m_listWidgetResults->count()>0 ) {
        m_listWidgetResults->setCurrentRow(0);
        m_listWidgetResults->setFocus();
    }
}

void EbWidgetSearchResult::updateLocaleInfo()
{
    m_actionCopy->setText( theLocales.getLocalText("search-result.context-menu.copy.text","Copy") );
    m_actionCopy->setToolTip( theLocales.getLocalText("context-menu.copy.tooltip","") );
    m_actionDelete->setText( theLocales.getLocalText("search-result.context-menu.delete.text","Delete") );
    m_actionDelete->setToolTip( theLocales.getLocalText("context-menu.delete.tooltip","") );

}

void EbWidgetSearchResult::onKeyPressEnterItem(const QListWidgetItem *item)
{
    const QString url = item->data(Qt::UserRole).toString();
    if ( !url.isEmpty() ) {
        emit clickedSearchResultUrl(url);
    }
}

void EbWidgetSearchResult::onClickedItemLabel()
{
    const EbLabel * labelItem = (EbLabel*)sender();
    const QString url = labelItem->property("url").toString();
    if ( !url.isEmpty() ) {
        emit clickedSearchResultUrl(url);
    }
}

//void EbWidgetSearchResult::onKeyPressEnterItemLabel()
//{
//    onClickedItemLabel();
//}

void EbWidgetSearchResult::onClickedContextMenu()
{
    const QAction * action = (QAction*)sender();
    const EB_COMMAND_ID commandId = (EB_COMMAND_ID)action->data().toInt();
    switch (commandId)
    {
    case EB_COMMAND_COPY: {
        /// 复制 URL
        QListWidgetItem * item = m_listWidgetResults->currentItem();
        if (item==0) break;
        QClipboard *board = QApplication::clipboard();
        board->setText( item->data(Qt::UserRole).toString() );
        break;
    }
    case EB_COMMAND_DELETE_ITEM_RES: {
        /// 删除 URL
        const int row = m_listWidgetResults->currentRow();
        if (row<0) break;
        QListWidgetItem * item = m_listWidgetResults->takeItem(row);
        if (item==0) break;
        const QString url = item->data(Qt::UserRole).toString();
        const QString sql = QString("DELETE FROM url_record_t WHERE url='%1'").arg(url);
        theApp->m_sqliteUser->execute( sql.toStdString().c_str() );
        m_listWidgetResults->removeItemWidget(item);
        delete item;
        checkShowHide();
        break;
    }
    default:
        break;
    }
}

//void EbWidgetSearchResult::onItemClicked(QListWidgetItem *item)
//{
//    const QString url = item->data(Qt::UserRole).toString();
//    if ( !url.isEmpty() ) {
//        emit clickedSearchResultUrl(url);
//    }
//}

void EbWidgetSearchResult::resizeEvent(QResizeEvent *e)
{
    m_listWidgetResults->setGeometry( 0,0,width(),height() );

    QWidget::resizeEvent(e);
}

void EbWidgetSearchResult::contextMenuEvent(QContextMenuEvent *e)
{
    const QListWidgetItem * item = m_listWidgetResults->currentItem();
    if (item!=0) {
        this->m_menu->exec(e->globalPos());
        /// 没有使用
//        const QString url = item->data(Qt::UserRole).toString();
//        if ( !url.isEmpty() ) {
//            const QString tooltip = QString("%1 %2").arg(theLocales.getLocalText("context-menu.copy.text","")).arg(url);
//            m_actionCopy->setToolTip(tooltip);
//            this->m_menu->exec(e->globalPos());
//        }
    }
    QWidget::contextMenuEvent(e);
}

void EbWidgetSearchResult::checkShowHide()
{
    const int count = m_listWidgetResults->count();
    const int w = width();
    const int h = MIN(count,20)*26+2;   /// 26
    resize(w,h);
    if (count==0) {
        this->hide();
    }
    else if ( !this->isVisible() ){
        this->show();
        this->raise();
    }
}

