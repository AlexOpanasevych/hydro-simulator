#include "simulationgraphic.h"

SimulationGraphic::SimulationGraphic(QCustomPlot *sSPlot, QCustomPlot *rPlot, Simulation *simul, QObject *parent): QObject(parent)
{
    soundSpeedPlot = sSPlot;
    rayPlot = rPlot;
    simulation = simul;

    connect(simulation, &Simulation::repaint, this, [this](){
        rescale = true;
        repaint();
    });
}

void SimulationGraphic::paint()
{
    //rayCurve = simulation->getRayCurve();
    rayCurve = simulation->getRayCurve();
    graphList = simulation->getGraphPointsList();
    //graphList = simulation->getGraphPointsList();

    rayPlot->clearGraphs();
    rayPlot->clearMask();
    rayPlot->clearFocus();
    rayPlot->clearPlottables();
    rayPlot->clearItems();

    rayPlot->xAxis->setLabel("Дистанцiя(м)");
    rayPlot->yAxis->setLabel("Глибина(м)");
    //sound
    rayPlot->yAxis->setRangeReversed(true);

    //lines
    QCPGraph *prevGraph = nullptr;

    for (int i = 0; i < graphList->size() ; i++)
    {
        auto rCurve  = new QCPGraph(rayPlot->xAxis, rayPlot->yAxis);

        rCurve->setData(graphList->at(i).first, graphList->at(i).second);
        if (i == 0)
        {
            prevGraph = rCurve;
        }
        else
        {
            QBrush shadowBrush(Qt::black, Qt::Dense7Pattern);
            prevGraph->setBrush(shadowBrush);
            prevGraph->setChannelFillGraph(rCurve);

            prevGraph = rCurve;
        }
    }

    //bottom
    auto rCurve  = new QCPCurve(rayPlot->xAxis, rayPlot->yAxis);
    rCurve->setPen(QColor(Qt::red));
    rCurve->setData(rayCurve->first, rayCurve->second);
    qDebug() << simulation->submarinePos();
    if (simulation->isReflected(simulation->submarinePos().x(), simulation->submarinePos().y()))
    {
        //if (simulation->submarinePos().y() < 5)
        //{
        //    simulation->setSubmarinePos(QPointF(simulation->submarinePos().x(), 5));
        //}
        simulation->calculateBackWave();
        backRayCurve = simulation->getBackRayCurve();
        backGraphList = simulation->getBackGraphPointsList();

        QCPGraph *prevGraph = nullptr;

        for (int i = 0; i < graphList->size() ; i++)
        {
            auto rCurve  = new QCPGraph(rayPlot->xAxis, rayPlot->yAxis);
            rCurve->setPen(QColor(Qt::red));
            rCurve->setData(backGraphList->at(i).first, backGraphList->at(i).second);
            if (i == 0)
            {
                prevGraph = rCurve;
            }
            else
            {
                QBrush shadowBrush(Qt::red, Qt::DiagCrossPattern);
                prevGraph->setBrush(shadowBrush);
                prevGraph->setChannelFillGraph(rCurve);
                prevGraph = rCurve;
            }
        }
    }

    if (rescale)
    {
        rayPlot->rescaleAxes();
        rayPlot->xAxis->rescale();
        rayPlot->yAxis->setRange(0, 300);
        rescale = false;
        rayPlot->replot();
    }

    rayPlot->setInteraction(QCP::Interaction::iRangeDrag, true);
    rayPlot->setInteraction(QCP::Interaction::iRangeZoom , true);
}

void SimulationGraphic::paintSpeed()
{
    soundSpeedCurve = simulation->getSoundSpeedCurve();

    soundSpeedPlot->clearGraphs();
    soundSpeedPlot->clearPlottables();
    soundSpeedPlot->clearItems();
    soundSpeedPlot->xAxis->setLabel("Швидкiсть(м/с)");
    soundSpeedPlot->yAxis->setLabel("Глибина(м)");

    soundSpeedPlot->xAxis->setVisible(true);
    soundSpeedPlot->xAxis2->setVisible(false);
    soundSpeedPlot->yAxis->setRangeReversed(true);

    auto sspCurve  = new QCPCurve(soundSpeedPlot->xAxis, soundSpeedPlot->yAxis);
    sspCurve->setPen(QColor(Qt::red));
    sspCurve->setData(soundSpeedCurve->first, soundSpeedCurve->second);
    soundSpeedPlot->rescaleAxes();
    soundSpeedPlot->xAxis->setRange(1450, 1540);
    soundSpeedPlot->yAxis->setRange(0, 300);
    soundSpeedPlot->replot();
    soundSpeedPlot->setInteraction(QCP::Interaction::iRangeDrag, true);
    soundSpeedPlot->setInteraction(QCP::Interaction::iRangeZoom , true);


}

void SimulationGraphic::paintSubmarine()
{
    auto subGraph  = new QCPGraph(rayPlot->xAxis, rayPlot->yAxis);
    QPointF subPos = simulation->submarinePos();
    QVector<double> subX = {0, subPos.x(), subPos.x()};
    QVector<double> subY = {subPos.y(), subPos.y(), 0};
    subGraph->setData(subX, subY);

    QPen DotPen;
    DotPen.setColor(QColor(255, 30, 40));
    DotPen.setStyle(Qt::DotLine);
    DotPen.setWidthF(2);
    subGraph->setPen(DotPen);
    rayPlot->replot();

}

void SimulationGraphic::repaint()
{
    paintSpeed();
    paint();
    paintSubmarine();
}
