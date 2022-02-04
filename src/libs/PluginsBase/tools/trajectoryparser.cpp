#include "trajectoryparser.h"
#include <QDebug>

TrajectoryParser::TrajectoryParser(QObject *parent) : QObject(parent)
{
    m_cmds = new QList<Cmd*>();
}

QList<Cmd *> *TrajectoryParser::parse(QString text, bool *isError, QString *errorString)
{
    removeCmds();

    auto splitted = text.toUpper().remove("\n").split(";", Qt::SkipEmptyParts);

    if(splitted.isEmpty()) {
        if(isError != nullptr)
            *isError = true;
        if(errorString != nullptr)
            *errorString = "error: while splitting lines";
        return {};
    }

    for(auto cmd : splitted) {
        Cmd *newCmd = new Cmd();
        if(newCmd->serialize(cmd))
        {
            if(!newCmd->hasAllParams())
                newCmd->fillParams();
            m_cmds->push_back(newCmd);
        }
    }

    if(!m_cmds->isEmpty()) {

        if(!m_cmds->at(0)->hasAllParams()) {
            if(isError != nullptr)
                *isError = true;
            if(errorString != nullptr)
                *errorString = "exception: first cmd has not all params";
        }

    }
    return m_cmds;
}

void TrajectoryParser::removeCmds()
{
    for (auto cmd : *m_cmds)
    {
        cmd->clear();
    }
    qDeleteAll(m_cmds->begin(), m_cmds->end());
    m_cmds->clear();
}
