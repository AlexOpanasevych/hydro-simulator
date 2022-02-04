#include "propamplifier.h"

//#include <src/lib/ktools/kmonoobserver.h>
//#include <src/lib/ktools/kpropertycontext.h>

std::function<double (QVariant)> PropAmplifier::propertyConverter() const {
    return m_propertyConverter;
}

void PropAmplifier::setPropertyConverter(const std::function<double (QVariant)> &propertyConverter) {
    m_propertyConverter = propertyConverter;
}

PropAmplifier::PropAmplifier(QObject *parent) : Amplifier(parent) {
//    KMonoObserver::bindTo(this)
//            .exec([this](QVariant value) {
//        setGainLevel(propertyConverter()(value));
//    })
//            .observe(&PropAmplifier::propertyIdChanged);
//    connect(this, &PropAmplifier::propertyIdChanged, this, [this](QString value) {
//        if(KPropertyContext::getInstance()->contains(value)) {
//            setGainLevel(propertyConverter()(KPropertyContext::getInstance()->getProperty<KProperty>(value)->variant()));
//        }
//    });
}

double PropAmplifier::gain(double signal) {
    return std::min(signal * gainLevel() * gainLevelMultiplier(), limit());
}
