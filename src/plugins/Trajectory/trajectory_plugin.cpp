#include "trajectory_plugin.h"
#include <qqml.h>

#include <qsgItems/segmentitem.h>
#include <qsgItems/traceitem.h>
#include <qsgItems/trajectoryitem.h>

void TrajectoryPlugin::registerTypes(const char *uri)
{
    // @uri Trajectory
    qmlRegisterType<TrajectoryItem>(uri, 1, 0, "TrajectoryItem");
    qmlRegisterType<TraceItem>(uri, 1, 0, "TraceItem");
    qmlRegisterType<SegmentItem>(uri, 1, 0, "SegmentItem");
}

