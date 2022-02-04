#ifndef PROPAMPLIFIER_H
#define PROPAMPLIFIER_H

//#include "src/lib/utility/propertyhelper.h"
#include "amplifier.h"
#include <QString>
#include <QVariant>
#include <kmacro.h>

class PropAmplifier : public Amplifier {
    Q_OBJECT
//    K_AUTO_PROPERTY(QString, propertyId, propertyId, setPropertyId, propertyIdChanged, "")
    K_AUTO_PROPERTY(double, gainLevel, gainLevel, setGainLevel, gainLevelChanged, 1)
    K_AUTO_PROPERTY(double, limit, limit, setLimit, limitChanged, std::numeric_limits<double>::max())
    K_AUTO_PROPERTY(double, gainLevelMultiplier, gainLevelMultiplier, setGainLevelMultiplier, gainLevelMultiplierChanged, 1)
    std::function<double(QVariant)> m_propertyConverter = [](QVariant value){ return value.toDouble(); };
public:
    PropAmplifier(QObject *parent = nullptr);

    // Amplifier interface
public:
    double gain(double signal);
    std::function<double (QVariant)> propertyConverter() const;
    void setPropertyConverter(const std::function<double (QVariant)> &propertyConverter);
};

#endif // PROPAMPLIFIER_H
