#include "cmd.h"

#include <QRegularExpression>
#include <QDebug>

Cmd::Cmd()
{
    
}

Cmd::Cmd(const Cmd &other)
{
    if(!this->m_params.isEmpty())
        clear();
    for(auto param : other.m_params) {
        Param * newParam = new Param;
        newParam->name = param->name;
        newParam->value = param->value;
        newParam->isValid = param->isValid;
        newParam->modifier = param->modifier;
        this->m_params.push_back(newParam);
    }
}

Cmd::~Cmd()
{
    clear();
}

void Cmd::clear()
{
    if(!m_params.isEmpty()){
        qDeleteAll(m_params.begin(), m_params.end());
        m_params.clear();
    }

}

bool Cmd::serialize(QString text)
{
    m_length = text.length();
    auto splitted = text.split(" ");
    if(keywords.contains(splitted[0])) {
        this->m_name = splitted[0];
        for (int i = 1; i < splitted.size(); ++i) {
            QRegularExpression cmdMatch;
            cmdMatch.setPattern("(?<paramName>[a-zA-Z]+)(?<modifier>\\+|\\-)?(?<value>(0|([1-9]\\d*))(\\.(\\d+))?)");
            QRegularExpressionMatch match = cmdMatch.match(splitted[i]);
            Param * newParam = new Param;
            newParam->isValid = true;
            if(!match.captured("paramName").isEmpty())
                newParam->name = match.captured("paramName");
            if(!match.captured("modifier").isEmpty()) {
                newParam->modifier = match.captured("modifier");
            }
            if (!match.captured("value").isEmpty())
                newParam->value = match.captured("value").toDouble();
            this->m_params.push_back(newParam);
        }
        return true;
    }
    else return false;

}

QList<Param *> Cmd::params() const
{
    return m_params;
}

Param * Cmd::getParameter(int index) const
{
    return m_params[index];
}

Param * Cmd::getParameter(QString name) const
{
    for(auto param : m_params) {
        if(param->name == name) {
            return param;
        }
    }
    return {};
}

int Cmd::getLength() const
{
    return m_length;
}

bool Cmd::hasParameter(QString name) const
{
    for (auto param : m_params) {
        if(param->name == name) {
            return true;
        }
    }
    return false;
}

bool Cmd::fillParams()
{
    auto keywordDefaults = defaults.find(this->m_name);
    if(keywordDefaults != defaults.end()) {
        qDebug() << "keyword defaults : " << keywordDefaults.value();
        for(auto def : keywordDefaults->toStdMap()) {
            if(!hasParameter(def.first)) {
                Param * newParam = new Param;
                newParam->name = def.first;
                qDebug() << newParam->name;
                newParam->value = def.second;
                newParam->isValid = false;
                m_params.push_back(newParam);
            }
        }
    }
    return true;
}

bool Cmd::hasAllParams() const
{
    auto command = defaults.find(m_name);
    if(command != defaults.end()) {
        for (auto param : command->toStdMap()) {
            if(!hasParameter(param.first)) {
                return false;
            }
        }
        return true;
    }
    return false;
}
