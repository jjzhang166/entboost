#ifndef EBDEFINES_H
#define EBDEFINES_H

typedef enum EB_VIEW_MODE {
    EB_VIEW_MAIN_FRAME,    /// 主界面
    EB_VIEW_SELECT_USER    /// 选择用户
}EB_VIEW_MODE;

typedef enum EB_SORT_ITEMS_FLAG {
    EB_DISABLE_SORT,        /// 禁排序
    EB_AUTO_SORT,           /// 自动排序
    EB_FORCE_SORT           /// 强制排序
}EB_SORT_ITEMS_FLAG;

typedef enum EB_SEARCH_FROM_FLAG{
    EB_SEARCH_FROM_SEARCH_BAR,
    EB_SEARCH_FROM_MAIN_FRAME,
    EB_SEARCH_FROM_SELECT_USER
}EB_SEARCH_FROM_FLAG;

#endif // EBDEFINES_H
