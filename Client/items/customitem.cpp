#include "customitem.h"
#include <QRandomGenerator>

CustomItem::CustomItem(QQuickItem *parent) : QNanoQuickItem(parent)
{

    updateTimer = new QTimer();
    connect(updateTimer, &QTimer::timeout, this, &CustomItem::update);
//    updateTimer->start(100);

//    setupProcessor();

    elapsedTimer.start();
}

QNanoQuickItemPainter *CustomItem::createItemPainter() const
{
    return new CustomItemPainter;
}

void CustomItemPainter::paint(QNanoPainter *p)
{
    // Paint the background circle
//    p->beginPath();
//    p->circle(width()*0.5, height()*0.5, width()*0.46);
//    QNanoRadialGradient gradient1(width()*0.5, height()*0.4, width()*0.6, width()*0.2);
//    gradient1.setStartColor("#808080");
//    gradient1.setEndColor("#404040");
//    p->setFillStyle(gradient1);
//    p->fill();
//    p->setStrokeStyle("#202020");
//    p->setLineWidth(width()*0.02);
//    p->stroke();
//    // Hello text
//    p->setTextAlign(QNanoPainter::ALIGN_CENTER);
//    p->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
//    QNanoFont font1(QNanoFont::DEFAULT_FONT_BOLD_ITALIC);
//    font1.setPixelSize(width()*0.08);
//    p->setFont(font1);
//    p->setFillStyle("#B0D040");
//    p->fillText("", width()*0.5, height()*0.4);
//    // QNanoPainter text
//    QNanoFont font2(QNanoFont::DEFAULT_FONT_THIN);
//    font2.setPixelSize(width()*0.18);
//    p->setFont(font2);
//    p->fillText("", width()*0.5, height()*0.5);
    if(needToRedrawImage) {
        p->beginPath();
        p->drawImage(backgroundImage, margin, margin, 400 - margin, 400 - margin);
        p->drawImage(linealImage, margin, margin, 400 - margin, 50);
        p->drawImage(frameImage, 0, 0, 400 + margin, 400 + margin);
        needToRedrawImage = false;
    }
//    if(needToRedrawPoints) {
        p->setLineWidth(1);
        p->setLineJoin(QNanoPainter::JOIN_ROUND);
        p->setLineCap(QNanoPainter::CAP_ROUND);
        p->setFillStyle(QNanoColor("#000000"));

    //    for(int i = 0; i < 10; i++) {
    //        qDebug() << "arc created";
    //        p->beginPath();
    //        p->circle(QRandomGenerator::global()->bounded(50, 400), QRandomGenerator::global()->bounded(50, 400), 1);
    //        p->fill();
    //    }
//        needToRedrawPoints = false;
//    }

}

void CustomItemPainter::synchronize(QNanoQuickItem *item)
{
    auto customItem = dynamic_cast<CustomItem*>(item);

    if(customItem->m_backgroundImageSource != backgroundImage.filename()) {
        backgroundImage.setFilename( customItem->m_backgroundImageSource );
        backgroundOrigin.setFilename( customItem->m_backgroundImageSource );
        needToRedrawImage = true;
    }

    if(customItem->m_linealImageSource != linealImage.filename()) {
        linealImage.setFilename(customItem->m_linealImageSource);
        needToRedrawImage = true;
    }

    if(customItem->m_frameImageSource != frameImage.filename()) {
        frameImage.setFilename(customItem->m_frameImageSource);
        needToRedrawImage = true;
    }

    if(!customItem->sweepQueue.isEmpty()) {
        while(customItem->sweepQueue.isEmpty()) {
            auto pressures = customItem->sweepQueue.dequeue();
            points.append(pressures);
        }
    }
}
