#ifndef TRAJECTORYPARSER_H
#define TRAJECTORYPARSER_H

#include <QObject>
#include <pluginsbase_global.h>
#include <tools/iparser.h>
#include <QSet>
#include <QMap>

class PLUGINSBASE_EXPORT TrajectoryParser : public QObject, public IParser
{
    Q_OBJECT
public:
    explicit TrajectoryParser(QObject *parent = nullptr);

    QList<Cmd*> *parse(QString text, bool *isError = nullptr, QString *errorString = nullptr) override;
private:
    void removeCmds();
    QList<Cmd*> *m_cmds = nullptr;
};



#endif // TRAJECTORYPARSER_H
