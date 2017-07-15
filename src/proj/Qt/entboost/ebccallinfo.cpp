#include <QtGui>
#include "ebccallinfo.h"

EbcCallInfo::EbcCallInfo()
    : m_bOffLineUser(false)
{
    m_tLastTime = time(0);
}
EbcCallInfo::pointer EbcCallInfo::create(void)
{
    return EbcCallInfo::pointer(new EbcCallInfo());
}
