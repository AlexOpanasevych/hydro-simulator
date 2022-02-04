#include "bronzetucpainter.h"

//#include <src/lib/network/sweepstreamcontext.h>
#include <math.h>
#include <algorithm>
//#include <src/client/mgkconstants.h>
//#include <src/lib/ktools/kmonoobserver.h>
//#include <src/client/graphics/discretegraphdrawer.h>

#include <items/discretegraphdrawer.h>


//#include <src/client/modules/bronzemodule.h>
//#include <src/lib/utility/kbusautounregisterer.h>

void BronzeTucPainter::doplerProcessorInit() {
//    doplerProcessor.setMode(XnPressureConvertor::FrequencyProcessing);
//    doplerProcessor.setChannalCount(15);
//    doplerProcessor.setMinVelocity(math->fromKnots(-24));
//    doplerProcessor.setMaxVelocity(math->fromKnots(24));
    //KMonoObserver::bindTo(this).exec(&doplerProcessor, &XnPressureConvertor::setXnCount).observe("bronzeXnCount");
//    connect(this, QOverload<int>::of(&BronzeTucPainter::bronzeXnCountChanged), &doplerProcessor, QOverload<int>::of(&XnPressureConvertor::setXnCount));
//    //KMonoObserver::bindTo(this).exec(&doplerProcessor, &XnPressureConvertor::setXnShiftAngle).observe("bronzeXnShiftAngle");
//    connect(this, QOverload<double>::of(&BronzeTucPainter::bronzeXnShiftAngleChanged), &doplerProcessor, QOverload<double>::of(&XnPressureConvertor::setXnShiftAngle));
//    //KMonoObserver::bindTo(this).exec(&doplerProcessor, &XnPressureConvertor::setXnRotation).observe("bronzeXnRotation");
//    connect(this, QOverload<double>::of(&BronzeTucPainter::bronzeXnRotationChanged), &doplerProcessor, QOverload<double>::of(&XnPressureConvertor::setXnRotation));
//    //KMonoObserver::bindTo(this).exec(&doplerProcessor, &XnPressureConvertor::setVesselRotation).observe("ownCourse");
//    connect(this, QOverload<double>::of(&BronzeTucPainter::ownCourseChanged), &doplerProcessor, QOverload<double>::of(&XnPressureConvertor::setVesselRotation));
//    KMonoObserver::bindTo(this).exec(&doplerProcessor, &XnPressureConvertor::setNoiseAmplitude).observe("noiseCoeficient");
//    connect(this, QOverload<double>::of(&BronzeTucPainter::), &doplerProcessor, QOverload<double>::of(&XnPressureConvertor::setXnShiftAngle));
}

BronzeTucPainter::BronzeTucPainter(MGKAbstractMonitor * monitor) : MonitorPainter(monitor) {
//    doplerProcessorInit();
//    SweepStream *stream = SweepStreamContext::instance()->getSweepStream(MgkConstants::TargetsSweepStream);
//    stream->listen();
//    connect(stream, &SweepStream::sweepReady, this, [this](QVector<double> &sweep) {
//        targets = sweep;
//    });

    m_handler = StreamConnector::instance()->bindToStream("bronze", [this](const QVariant& data){
        auto map = data.toMap();
        targets = map["sweep"].value<QVector<QPair<double, double>>>();
    });
//    tucNoiseGenerator.setCnt(400);

//    KBusAutoUnregisterer *unregisterer = new KBusAutoUnregisterer(this);
//    unregisterer->setHandle(KBus::instance()->registerHandler("platinaWaveRecieved", [this](QVariant value) {
//        QVariantMap object = value.toMap();
//        double angle = object["angle"].toDouble();
//        double emissionTime = object["emissionTime"].toDouble();
//        double intensity = object["intensity"].toDouble();
//        double radialVelocity = object["radialVelocity"].toDouble();
//        doplerProcessor.addPressure(angle, intensity * m_bronzeTPAssistGain, radialVelocity, emissionTime);
//    }));

//    rangeDistribution.setProcessor([](double value) {
//        return std::pow(KMath::instance()->gaus(value), 0.4);
//    });


//    doplerProcessor.gainIn(&TOAtinuator, &Amplifier::gainOut);
//    TOAtinuator.gainIn(&RU2Atinuator, &Amplifier::gainOut);
//    RU2Atinuator.gainIn(&RU1Atinuator, &Amplifier::gainOut);
//    RU1Atinuator.gainIn(&amplifier, &Amplifier::gainOut);

//    TOAtinuator.setPropertyId("bronzeGainTP");
//    RU2Atinuator.setPropertyId("bronzeRU2");
//    RU1Atinuator.setPropertyId("bronzeRU1");
//    amplifier.setPropertyId("dispBronzeDoplerGain");

    //tucMemory.setFadingValue(8);
}

void BronzeTucPainter::paint(QPainter *painter) {
//    const double D = 0.6;
//    const double f = 5000;
//    const double c = 1500;
//    const double GAIN = 20;

//    auto noise = [this]() -> double {
//        return tucNoiseIntegrator.calc(tucNoiseGenerator.proceed());
//    };
//    auto epnoise = [this]() -> double {
////        auto sweep = doplerProcessor.sweep();
////        auto v = std::vector<double>(sweep.begin(), sweep.end());
//        //return tucMemory.proceed(*std::max_element(v.begin(), v.end()) * 10);
//    };
    for (int  i = 0; i < 100; ++i) {
//        double t = i / (100. * f);
//        double delta = 0;
//        double sum = 0;

//        if(targets.size() > 0 && (m_bronzeMode == BronzeMode::SearchMode || m_bronzeMode == BronzeMode::RSMode)) {
//            for(QPair<double, double> target : targets) {
//                lastTargetBearing = target.first;
//                double degreesDifference = KMath::instance()->degreesDifference(target.first, KMath::instance()->constrainDegrees(m_bronzeBearing - m_ownCourse));
//                double peleng = (M_PI / 180) * degreesDifference;
//                double module = qAbs(degreesDifference);
//                module = qMin(module, 5.0);
//                double radius = TOAtinuator.gain(KMath::instance()->dmap(module, 0, 5, 1, 0.1));
//                double dl = D*sin(peleng);
//                double phi = 2 * M_PI * f * dl / c;

////                delta += 2 * sin(phi / 2) * cos(2 * M_PI * f * t + phi / 2 + M_PI / 2.3) * target.second * GAIN * radius + noise();
////                sum += 2 * cos(phi / 2) * sin(2 * M_PI * f * t + phi / 2) * target.second * GAIN * radius + noise();
//            }
//        } else {
//            double degreesDifference = KMath::instance()->degreesDifference(lastTargetBearing, KMath::instance()->constrainDegrees(m_bronzeBearing - m_ownCourse));
//            double peleng = (M_PI / 180) * degreesDifference;
//            double dl = D * sin(peleng);
//            double phi = 2 * M_PI * f * dl / c;

//            delta += 2 * sin(phi / 2) * cos(2 * M_PI * f * t + phi / 2 + M_PI / 2.3) * m_bronzeCurrentPressure * GAIN * epnoise() + noise();
//            sum += 2 * cos(phi / 2) * sin(2 * M_PI * f * t + phi / 2) * m_bronzeCurrentPressure * GAIN * epnoise() + noise();
//        }
//        setPixel(static_cast<int>(monitor()->width() / 2 + delta), static_cast<int>(monitor()->height() / 2 + sum), painter);
    }

//    if(
//        m_bronzeMode == BronzeMode::RSGLMode ||
//        m_bronzeMode == BronzeMode::StepMode ||
//        m_bronzeMode == BronzeMode::CicleGLMode ||
//        m_bronzeMode == BronzeMode::CircleStopMode
//    ) {
//        DiscreteGraphDrawer doplerDrawer(monitor()->width() - 20, 25, 0, 0, 1);
//        doplerDrawer.setPosition({ 20, 280 });
////        doplerProcessor.proceed();
//        doplerDrawer.setSight(m_bronzeSelectedChannel);
//        //doplerDrawer.setValues(arrayMemory.proceed(rangeDistribution.proceed(doplerProcessor.sweep())));
//        doplerDrawer.paint(this, painter);
//    }
}
