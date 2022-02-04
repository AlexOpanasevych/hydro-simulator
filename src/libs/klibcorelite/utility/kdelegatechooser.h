#ifndef KDELEGATECHOOSER_H
#define KDELEGATECHOOSER_H

#include "kdelegate.h"

#include <QQuickItem>
#include <kmacro.h>

class KLIBCORELITE_EXPORT KDelegateChooser : public QQuickItem {
    Q_OBJECT
    K_OBJECT
    K_QML_TYPE(KDelegateChooser)

    QList<KDelegate *> m_delegates;
    K_LIST_PROPERTY(KDelegate, delegates, m_delegates)
    K_READONLY_PROPERTY(QObject*, currentDelegate, currentDelegate, setCurrentDelegate, currentDelegateChanged, nullptr)
public:
    explicit KDelegateChooser(QQuickItem *parent = nullptr);

signals:


};

#endif // KDELEGATECHOOSER_H
