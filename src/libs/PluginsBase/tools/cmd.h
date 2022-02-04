#ifndef CMD_H
#define CMD_H

#include <QString>
#include <QList>
#include <pluginsbase_global.h>
#include <QSet>
#include <QMap>

const QSet<QString> keywords = {"FORWARD", "ROTATE", "FIRE"}; // you can add some new comm easily
const QMap<QString, QMap<QString, double>> defaults = { // same
  {"FORWARD",
   {
       {"V", 0}, {"D", 0}, {"Z", 0}, {"ACC", 5}, {"DZ", 0}
   }
  },
  {"ROTATE",
   {
       {"A", 0}, {"R", 0}
   }
  },
  {"FIRE",
   {
       {"LB", 0}, {"RB", 0}, {"P", 0}, {"FRA", 0}, {"SRA", 0}, {"DT", 15000}
   }
  }
};

struct PLUGINSBASE_EXPORT Param{
    QString name;
    double value;
    QString modifier;
    bool isValid;
};

class PLUGINSBASE_EXPORT Cmd
{
public:
    Cmd();
    Cmd(const Cmd & other);
    ~Cmd();
    void clear();
    bool serialize(QString text);
    QString name() const { return m_name; }

    QList<Param *> params() const;
    Param * getParameter(int index) const;
    Param * getParameter(QString name) const;
    int getLength() const;
    bool hasParameter(QString name) const;
    bool fillParams();
    bool hasAllParams() const;
private:
    QString m_name;
    QList<Param *> m_params;
    int m_length = 0;

};

#endif // CMD_H
