#include "unifiedcmdparser.h"
#include <QStringList>
#include <QDebug>
std::set<QString> UnifiedCmdParser::keywords() const
{
    return m_keywords;
}

void UnifiedCmdParser::registerKeyword(QString keyword)
{
    m_keywords.insert(keyword);
}

void UnifiedCmdParser::registerKeywords(std::initializer_list<QString> keywords)
{
    for (auto str : keywords) {
        m_keywords.insert(str);
    }
}

void UnifiedCmdParser::loadCode(std::string code)
{
    this->code = code;
}

void UnifiedCmdParser::loadRules(std::initializer_list<std::pair<QString, QString>> rules)
{
    for (auto rule : rules) {
        m_rules[rule.first] = rule.second;
    }
}

void UnifiedCmdParser::parse()
{
    QString codeC;
    codeC = codeC.fromStdString(code);
    auto splitted = codeC.split(m_rules["lineSplitter"]);
    QStringListIterator i(splitted);
    while(i.hasNext()) {
        QString line = i.next();
        if(line.contains(QRegExp(QString("\\s*%1\\s*").arg(m_rules["comSymbol"])))) {
            qDebug() << "it's comment";
            parsed.push_back({{"comment", {"", line}}});
        }
        if(line.contains(QRegExp(QString("%1((.|\n)*)%2").arg(m_rules["multipleStartComSymbol"]).arg(m_rules["multipleEndComSymbol"])))) {
            qDebug() << "it's multiple comment";
            auto indexMCom = line.indexOf(m_rules["multipleEndComSymbol"], 1);
            QString multilineComment = line.left(indexMCom + m_rules["multipleEndComSymbol"].length());
            line = line.mid(m_rules["multipleEndComSymbol"].length() + indexMCom);
            parsed.push_back({{"multiple comment", {"", multilineComment}}});
            qDebug() << parsed.back();
        }
        auto tokens = line.split(m_rules["tokenSplitter"], Qt::SkipEmptyParts);
        for (auto token : tokens) {
            auto result = parseToken(token);
            qDebug() << result;
            parsed.push_back({result});
        }
        numberOfLines++;
    }
}

double UnifiedCmdParser::getNumber(int numberLine, QString parameter, bool *ok)
{
    if(parsed[numberLine][parameter].first == "number") {
        if(ok!= nullptr)
            *ok = true;
        return parsed[numberLine][parameter].second.toDouble();
    }
    else {
        qDebug() << "not a number";
        if(ok != nullptr)
            *ok = false;
        return -1;
    }
}

std::pair<QString, std::pair<QString, QString>> UnifiedCmdParser::parseToken(QString token)
{
    for (auto keyword : m_keywords) {
        if (token.startsWith(keyword)) {
            QString value = token.mid(keyword.length());
            if(!value.startsWith(m_rules["tokenRelation"])) {
                qDebug() << "token relation failed";
                return {"ERROR", {"error", "token relation failed"}};
            }
            else {
                value = value.mid(m_rules["tokenRelation"].length());
            }
            if(value.contains(QRegExp("^\\d+.\\d+$"))){
                qDebug() << "it's number";
                return {keyword, {"number", value}};
            }
            if(value.contains(QRegExp("'([^\\\"]|\\\")*'"))) {
                qDebug() << "it's string";
                return {keyword, {"string", value}};
            }
        }
    }
    return {"ERROR", {"error", "no keyword found"}};
}


UnifiedCmdParser::UnifiedCmdParser(std::string code) : code(code)
{
}

UnifiedCmdParser::UnifiedCmdParser()
{
    m_rules.insert({"lineSplitter", ";"});
    m_rules.insert({"tokenRelation", "="});
    m_rules.insert({"tokenSplitter", " "});
    m_rules.insert({"comSymbol", "//"});
    m_rules.insert({"multipleStartComSymbol", "/*"});
    m_rules.insert({"multipleEndComSymbol", "*/"});
}
