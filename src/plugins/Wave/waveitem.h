#ifndef WAVEITEM_H
#define WAVEITEM_H

#include <QQuickItem>

class WaveItem : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(WaveItem)

public:
    explicit WaveItem(QQuickItem *parent = nullptr);
    ~WaveItem() override;
};

#endif // WAVEITEM_H
