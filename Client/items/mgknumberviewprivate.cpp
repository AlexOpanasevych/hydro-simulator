#include "mgknumberviewprivate.h"


MGKNumberViewPrivate::MGKNumberViewPrivate()
{
    m_handler = StreamConnector::instance()->bindToStream("position", [this](const QVariant& data) {
        this->setValue(data);
    });
}

MGKNumberViewPrivate::~MGKNumberViewPrivate()
{
//    StreamConnector::instance()->unbindFromStream("position", m_handler);
    m_handler = nullptr;
}
