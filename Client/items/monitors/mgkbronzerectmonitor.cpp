#include "mgkbronzerectmonitor.h"

#include <items/painters/bronzecsnrpainter.h>
#include <math.h>

MGKBronzeRectMonitor::MGKBronzeRectMonitor(QQuickItem * parent) : MGKRectMonitorPrivate(parent)
{
    setActivePainter(new BronzeCSNRPainter(this));

    connect(this, &MGKBronzeRectMonitor::bronzeModeChanged, this, &MGKBronzeRectMonitor::onModeChanged);

}

void MGKBronzeRectMonitor::paint(QPainter *painter)
{
    MGKRectMonitorPrivate::paint(painter);
}

void MGKBronzeRectMonitor::onModeChanged()
{
    switch(m_bronzeMode) {
    case BronzeMode::GLCycle:
        break;
    case BronzeMode::Search:
        break;
    case BronzeMode::RSGL:
        break;
    case BronzeMode::StepGL:
        break;
    }
}
