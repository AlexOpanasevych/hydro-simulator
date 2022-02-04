#include "discretegraphdrawer.h"

#include <math/kmath.h>

QVector<double> DiscreteGraphDrawer::values() const {
    return m_values;
}

void DiscreteGraphDrawer::setValues(const QVector<double> &values) {
    m_values = values;
}

void DiscreteGraphDrawer::setSight(int value) {
    this->sight = value;
}

QPoint DiscreteGraphDrawer::position() const {
    return m_position;
}

void DiscreteGraphDrawer::setPosition(const QPoint &position) {
    m_position = position;
}

DiscreteGraphDrawer::DiscreteGraphDrawer(int width, int top, int bottom, double min, double max) {
    this->width = width;
    this->top = top;
    this->bottom = bottom;
    this->max = max;
    this->min = min;
    generator.setCnt(500);
}
#include <QtDebug>

void DiscreteGraphDrawer::paint(MonitorPainter *mp) {
    if(values().size() > 0) {
        auto pixels = reinterpret_cast<uint32_t*>(mp->monitor()->drawImage.bits());
        int spacing = width / values().size();
        for(int i = 0; i < values().size(); ++i) {
            int x = i * spacing;
            int height = -static_cast<int>(KMath::instance()->dmap(values()[i], min, max, 0, top - bottom));

            if(i == sight) {
                mp->drawPrettyPoint(pixels, x + position().x(), bottom + position().y() - 4, 1.0, integrator.calc(generator.proceed()) + 4);
            }
            mp->drawPrettyLine(pixels, x + position().x(), bottom + height + position().y() , x + position().x(), bottom + position().y(), 1);
        }
    }
}
