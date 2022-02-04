#ifndef UNIFIEDCMDPARSER_H
#define UNIFIEDCMDPARSER_H

#include <set>
#include <string>
#include <map>
#include <vector>
#include <QString>
#include "core_global.h"

class CORE_LIBRARY_EXPORT UnifiedCmdParser
{
    std::set<QString> m_keywords;

    std::map<QString, QString> m_rules;

//    QString lineSplitter = ";";
//    QString tokenRelation = "";
//    QString tokenSplitter = " ";
//    QString comSymbol = "#";
//    QString multipleStartComSymbol = "##";
//    QString multipleEndComSymbol = "##";
    std::vector<std::map<QString, std::pair<QString, QString>>> parsed; // multiset of maps which contains keyword as first argument, and pair which contains type as string and value as string
    std::string code;
    int numberOfLines = 0;
public:
    UnifiedCmdParser(std::string code);
    UnifiedCmdParser();
    std::set<QString> keywords() const;
    void registerKeyword(QString keyword);
    void registerKeywords(std::initializer_list<QString>);
    void loadCode(std::string code);
    void loadRules(std::initializer_list<std::pair<QString, QString>> rules);
    void parse();
    double getNumber(int numberLine, QString parameter, bool *ok = nullptr);
    double getString(int numberLine, QString parameter, bool *ok = nullptr);
private:
    std::pair<QString, std::pair<QString, QString>> parseToken(QString token);
};

#endif // UNIFIEDCMDPARSER_H
