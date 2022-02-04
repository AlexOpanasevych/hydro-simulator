#ifndef DOMENMAP_H
#define DOMENMAP_H

#include <QObject>
#include <QQuickItem>
#include <libs/klibcorelite/kmacro.h>

class DomenMap : public QObject
{
    Q_OBJECT
    K_QML_TYPE(DomenMap)
public:
    explicit DomenMap(QObject *parent = nullptr);

public slots:
    bool remove(QQuickItem *);
    bool remove(QObject *);
    void insert(QObject*, QQuickItem *);
    QQuickItem* get(QObject *);
    void clear();

private:
    QMap<QObject*, QQuickItem *> m_map;

signals:
    void removed();
};

#endif // DOMENMAP_H
