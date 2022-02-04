#include "raytracer.h"
#include <src/raytracer/simulation.h>
#include <src/raytracer/simulationgraphic.h>
#include <QQuickWidget>
#include <QQmlContext>
#include <src/lib/ktools/kmonoobserver.h>
#include <src/lib/utility/kadditional.h>

RayTracer::RayTracer(QWidget *parent) :
    QWidget(parent)
{
    KMonoObserver::bindTo(this)
            .exec([this](QVariant value) {
        selfVesselDepth = value.toDouble();
    })
            .observe("selfVesselDestinationDepth");

    connect(this, &RayTracer::platinaSoundModeChanged, this, [this](){
        if (platinaSoundMode())
        {
            calculateWave(4);
            simulation->repaint();
        }
        else
        {
            calculateWave(selfVesselDepth);
            simulation->repaint();
        }
    });

    simulation = new Simulation();
    soundPlot = new QCustomPlot(this);
    wavePlot = new QCustomPlot(this);
    qml = new QQuickWidget(this);
    qml->rootContext()->setContextProperty("simulation", simulation);
    qml->setSource(KAdditional::absolutePath("../resourses/raytracer/ControlPanel.qml"));
    simulation->setDepth(0);
    simulation->start();

    simulationGraphic = new SimulationGraphic(soundPlot, wavePlot, simulation);
    simulationGraphic->repaint();

    CustomButton *button = new CustomButton("^", this);
    button->setOrientation(CustomButton::VerticalBottomToTop);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    connect(button, &QPushButton::clicked, this, [this, button](){
        if (widgetState)
        {
            button->setOrientation(CustomButton::VerticalBottomToTop);
        }
        else
        {
            button->setOrientation(CustomButton::VerticalTopToBottom);
        }
        emit buttonClicked();
    });

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(button, 0, 0, 0, 1);
    layout->addWidget(wavePlot, 0, 1, 11, 20);
    layout->addWidget(soundPlot, 11, 12, 7, 9);
    layout->addWidget(qml, 11, 1, 7, 11);
    this->setLayout(layout);
}

RayTracer::~RayTracer()
{
    delete simulationGraphic;
    delete simulation;
    delete soundPlot;
    delete wavePlot;
}

RayTracer *RayTracer::instance = nullptr;
RayTracer *RayTracer::getInstance()
{
    if (instance == nullptr) instance = new RayTracer();
    return instance;
}

bool RayTracer::isReflected(double distance, double depth)
{
    if (simulation->hydrologyState())
    {
        if (platinaSoundMode() != prevSoundMode)
        {
            if (platinaSoundMode())
            {
                calculateWave(4);
            }
            else
            {
                calculateWave(depth);
            }
            prevSoundMode = platinaSoundMode();
        }
        return simulation->isReflected(distance, depth);
    }
    return true;
}

bool RayTracer::isRecieved(double distance, double depth)
{
    Q_UNUSED(depth)

    if (simulation->hydrologyState())
    {
        qDebug() << distance << depth;
        if (platinaSoundMode())
        {
            submarineChoosed(distance, depth);
            simulation->calculateBackWave();
            submarineChoosed(distance, 4);
            return simulation->isRecieved(distance, 4);
        }
        else
        {
            submarineChoosed(distance, depth);
            simulation->calculateBackWave();
            submarineChoosed(distance, selfVesselDepth);
            return simulation->isRecieved(distance, selfVesselDepth);

        }
    }
    return true;
}

void RayTracer::calculateWave(double depth)
{
    simulation->setDepth(depth);
    simulation->calculateWave();
}

bool RayTracer::getWidgetState() const
{
    return widgetState;
}

void RayTracer::setWidgetState(bool value)
{
    widgetState = value;
}

void RayTracer::submarineChoosed(double distance, double depth)
{
    if (simulation->hydrologyState())
        simulation->setSubmarinePos(QPointF(distance, depth));
}

void RayTracer::setSubmarinePosition(double distance, double depth)
{
    if (simulation->hydrologyState())
    {
        simulation->setSubmarinePos(QPointF(distance, depth));
        simulation->repaint();
    }
}

