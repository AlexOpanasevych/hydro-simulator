#include "mgkrecorderprivate.h"

#include <QSGGeometryNode>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QSGImageNode>
#include <QSGFlatColorMaterial>
//#include <QTimer>
#include <QPainter>
#include <math/kmath.h>
#include <QRandomGenerator>

MGKRecorderPrivate::MGKRecorderPrivate(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
    connect(this, QOverload<QString>::of(&MGKRecorderPrivate::backgroundImageSourceChanged), this, [this](QString bImageSrc) {
        backgroundImage = QImage(bImageSrc);
        qDebug() << (!backgroundImage.isNull() ? "loaded" : "not loaded") << bImageSrc;
        backgroundOrigin = QImage(bImageSrc);
        backgroundMirrored = backgroundImage.mirrored();
    });
    connect(this, QOverload<QString>::of(&MGKRecorderPrivate::linealImageSourceChanged), this, [this](QString lImageSrc) {
        linealImage = QImage(lImageSrc);
        qDebug() << (!linealImage.isNull() ? "loaded" : "not loaded") << lImageSrc;
    });
    connect(this, QOverload<QString>::of(&MGKRecorderPrivate::frameImageSourceChanged), this, [this](QString fImageSrc) {
        frameImage = QImage(fImageSrc);
        qDebug() << (!frameImage.isNull() ? "loaded" : "not loaded") << fImageSrc;
    });

    updateTimer = new QTimer();
    connect(updateTimer, &QTimer::timeout, this, &MGKRecorderPrivate::updateRecorder);
    updateTimer->start(100);

    setupProcessor();

    elapsedTimer.start();

}

QSGNode *MGKRecorderPrivate::updatePaintNode(QSGNode * oldNode, QQuickItem::UpdatePaintNodeData * data)
{
    Q_UNUSED(data)
    QSGImageNode * node = dynamic_cast<QSGImageNode*>(oldNode);

    if(!oldNode) {
        qDebug() << "init";
//        node = new QSGGeometryNode();


        QSGImageNode * backgroundImageNode = window()->createImageNode();
        {
            QSGTexture *texture = window()->createTextureFromImage(backgroundImage, QQuickWindow::TextureIsOpaque);
            qDebug() << "textureSize:" << texture->textureSize();
            if (!texture) {
                qDebug() << "something went wrong";
                return oldNode;
            }
            backgroundImageNode->setTexture(texture);
            backgroundImageNode->setRect(x(), y(), 400, 400);
            backgroundImageNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
        }
        node = backgroundImageNode;

        QSGImageNode * mirroredBackgroundNode = window()->createImageNode();


        {

            QSGTexture *texture = window()->createTextureFromImage(backgroundMirrored, QQuickWindow::TextureIsOpaque);
            qDebug() << "textureSize:" << texture->textureSize();
            if (!texture) {
                qDebug() << "something went wrong";
                return oldNode;
            }
            mirroredBackgroundNode->setTexture(texture);
            mirroredBackgroundNode->setRect(0, (backgroundGlobalPosition + backgroundImage.height()), 400, 400);
            mirroredBackgroundNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
//            QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 2);
//            geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
//            node->setGeometry(geometry);
//            node->setFlag(QSGNode::OwnsGeometry);
//            node->setFlag(QSGNode::OwnsOpaqueMaterial);

//            QSGTexture *texture = window()->createTextureFromImage(linealImage, QQuickWindow::TextureIsOpaque);
//            auto material = new QSGOpaqueTextureMaterial;
//            material->setTexture(texture);
//            material->setFiltering(QSGTexture::Linear);
//            material->setMipmapFiltering(QSGTexture::Linear);
//            backgroundImageNode->setMaterial(material);
        }
        node->appendChildNode(mirroredBackgroundNode);


        QSGImageNode * linealImageNode = window()->createImageNode();


        {

            QSGTexture *texture = window()->createTextureFromImage(linealImage/*.copy(margin, margin, 400 + margin, linealImage.height())*/, QQuickWindow::TextureIsOpaque);
            qDebug() << "textureSize:" << texture->textureSize();
            if (!texture) {
                qDebug() << "something went wrong";
                return oldNode;

            }
            linealImageNode->setTexture(texture);
            linealImageNode->setRect(0, 0, 400, linealImage.height() * 0.5);
            linealImageNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
//            QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 2);
//            geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
//            node->setGeometry(geometry);
//            node->setFlag(QSGNode::OwnsGeometry);
//            node->setFlag(QSGNode::OwnsOpaqueMaterial);

//            QSGTexture *texture = window()->createTextureFromImage(linealImage, QQuickWindow::TextureIsOpaque);
//            auto material = new QSGOpaqueTextureMaterial;
//            material->setTexture(texture);
//            material->setFiltering(QSGTexture::Linear);
//            material->setMipmapFiltering(QSGTexture::Linear);
//            backgroundImageNode->setMaterial(material);
        }
        node->appendChildNode(linealImageNode);

        QSGImageNode * frameImageNode = window()->createImageNode();
        {
            QSGTexture *texture = window()->createTextureFromImage(frameImage, QQuickWindow::TextureHasAlphaChannel);
            qDebug() << "textureSize:" << texture->textureSize();
            if (!texture) {
                qDebug() << "something went wrong";
                return oldNode;
            }
            frameImageNode->setTexture(texture);
            frameImageNode->setRect(-margin, -margin, frameImage.width() - margin, frameImage.height() - margin);
            frameImageNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

//            QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 2);
//            geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
//            node->setGeometry(geometry);
//            node->setFlag(QSGNode::OwnsGeometry);
//            node->setFlag(QSGNode::OwnsOpaqueMaterial);

//            QSGTexture *texture = window()->createTextureFromImage(frameImage, QQuickWindow::TextureIsOpaque);
//            auto material = new QSGOpaqueTextureMaterial;
//            material->setTexture(texture);
//            material->setFiltering(QSGTexture::Linear);
//            material->setMipmapFiltering(QSGTexture::Linear);
//            frameImageNode->setMaterial(material);

        }
        node->appendChildNode(frameImageNode);

//        QSGGeometryNode * g = new QSGGeometryNode;
//        {
//            auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 1);
////            qDebug() << "textureSize:" << texture->textureSize();
//            geometry->setDrawingMode(QSGGeometry::DrawPoints);
//            auto material = new QSGFlatColorMaterial();
//            material->setColor("#222222");
//            g->setMaterial(material);
//            g->setGeometry(geometry);
//            g->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
////            frameImageNode->setTexture(texture);
//            frameImageNode->setRect(0, 0, 400, 400);
//            frameImageNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

////            QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 2);
////            geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
////            node->setGeometry(geometry);
////            node->setFlag(QSGNode::OwnsGeometry);
////            node->setFlag(QSGNode::OwnsOpaqueMaterial);

////            QSGTexture *texture = window()->createTextureFromImage(frameImage, QQuickWindow::TextureIsOpaque);
////            auto material = new QSGOpaqueTextureMaterial;
////            material->setTexture(texture);
////            material->setFiltering(QSGTexture::Linear);
////            material->setMipmapFiltering(QSGTexture::Linear);
////            frameImageNode->setMaterial(material);

//        }
//        node->appendChildNode(g);


//        QSGTexture *textureBackground = window()->createTextureFromImage(backgroundImage(), QQuickWindow::TextureIsOpaque);
//        textureNode->setOwnsTexture(true);
//        textureNode->setRect(boundingRect());
//        textureNode->markDirty(QSGNode::DirtyForceUpdate);
//        textureNode->setTexture(textureBackground);
    }
    else {
        node = dynamic_cast<QSGImageNode*>(oldNode);
        node->markDirty(QSGNode::DirtyGeometry);
        auto bMirroredImageNode = dynamic_cast<QSGImageNode*>(node->childAtIndex(0));
        bMirroredImageNode->texture()->deleteLater();
        QSGImageNode::rebuildGeometry(bMirroredImageNode->geometry(), window()->createTextureFromImage(backgroundMirrored, QQuickWindow::TextureHasAlphaChannel), boundingRect(), boundingRect(), QSGImageNode::NoTransform);
        bMirroredImageNode->markDirty(QSGNode::DirtyGeometry);
//        auto linealImageNode = node->childAtIndex(1);
//        auto frameImageNode = node->childAtIndex(2);

    }
    return node;
}

//void MGKRecorderPrivate::platinaWaveReceived(QVariantMap data)
//{
//    double angle = data["angle"].toDouble();
//    double emissionTime = data["emissionTime"].toDouble();
//    double intensity = data["intensity"].toDouble();
//    double radialVelocity = data["radialVelocity"].toDouble();
////    spatialProcessor->addPressure(angle, intensity, radialVelocity, emissionTime);
//}

void MGKRecorderPrivate::updateRecorder()
{
    for(int i = 0; i < 16; i++) {
        QVector<double> sweeps;
        for(int i = 0; i < 16; i++) {

            sweeps.push_back(QRandomGenerator::global()->bounded(100.));
        }
        sweepQueue.enqueue(sweeps);
    }
    while(!sweepQueue.isEmpty()) {
        double newElapsed = elapsedTimer.elapsed();
        double deltaTime = newElapsed - elapsedFromLastDrawing;
        elapsedFromLastDrawing = newElapsed;
        QVector<double> pressures = sweepQueue.dequeue();
        if(pressures.size() > 0) {
            double sumPressure = 0;
            for(int i = 0; i < pressures.size() - 1; ++i) {
                sumPressure += pressures[i];
            }
            double sweepDistance = pressures.last();
            double alpha = std::min(sumPressure, 1.0);

            if(alpha > 0.1) {
                qDebug() << "sweepDist:" << alpha << sweepDistance << maxDistance() << margin;
                alpha = 0.9;
            }

            if(!(backgroundImage.isNull() || backgroundMirrored.isNull())) {
                QPainter painter(&backgroundImage);
                QPainter painter2(&backgroundMirrored);
                painter.setOpacity(alpha);
                painter2.setOpacity(alpha);
                painter.setPen(QPen(QColor("#222222"), 2));
                painter2.setPen(QPen(QColor("#222222"), 2));

                int pointX = static_cast<int>(KMath::instance()->dmap(sweepDistance, 0, maxDistance(), margin, 410 - margin));
                int pointY = (backgroundImage.height() + backgroundPosition) + startPosition;
                painter.drawPoint(pointX, pointY);
                painter2.drawPoint(pointX, pointY);

                painter.setPen(QPen(QColor("#222222"), 10));
                painter2.setPen(QPen(QColor("#222222"), 10));
                painter.drawPoint(410, pointY);
                painter2.drawPoint(410, pointY);
            }

            if(epStationState() == "emitting") {
                int height =static_cast<int>(backgroundImage.height());
                if(updateTimer) {
                    double ySpacing = 10;

                    double delta = ySpacing * deltaTime / emissionCycleTime();

                    backgroundPosition -= delta;
                    backgroundGlobalPosition -= delta;
                    if(backgroundPosition <= -height) {
                        if(qFuzzyCompare(backgroundOffset, 0)) {
                            backgroundOffset = 2 * height;
                        } else {
                            backgroundOffset = 0;
                        }
                        backgroundPosition = 0;
                        backgroundImage = backgroundOrigin.copy();
                        backgroundMirrored = backgroundImage.mirrored(false, true);
                    }

                    if(backgroundGlobalPosition <= -height * 2) {
                        backgroundGlobalPosition = 0;
                    }
                }
            }
            update();
        }
    }
}

void MGKRecorderPrivate::setupProcessor()
{

//    spatialProcessor->setMode(XnPressureConvertor::SpatialProcessing);
//    spatialProcessor->setChannalCount(3);
//    spatialProcessor->setMinVelocity(KMath::instance()->fromKnots(-15));
//    spatialProcessor->setMaxVelocity(KMath::instance()->fromKnots(15));

//    platinaWaveRecieved = StreamConnector::instance()->bindToStream("platinaWave", [this](const QVariant& value){
//        QVariantMap object = value.toMap();
//        double angle = object["angle"].toDouble();
//        double emissionTime = object["emissionTime"].toDouble();
//        double intensity = object["intensity"].toDouble();
//        double radialVelocity = object["radialVelocity"].toDouble();
//        spatialProcessor->addPressure(angle, intensity, radialVelocity, emissionTime);
//    });

//    connect(spatialProcessor, &XnPressureConvertor::pressureAdded, this, [](QVariant value) {
//        StreamConnector::instance()->sendData(value, "platinaWave");
//    });

//    m_handler1 = StreamConnector::instance()->bindToStream("platinaWave", [this](const QVariant& data) {
//        auto map = data.toMap();
//        spatialProcessor->setXnCount(map["xnCount"].toInt());
//        spatialProcessor->setXnShiftAngle(map["xnShiftAngle"].toDouble());
//        spatialProcessor->setXnRotation(map["xnRotation"].toDouble());
//    });

//    spatialProcessor->setNoiseGenerator(nullptr);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
//        spatialProcessor->proceed(currentDistance());
//        sweepQueue.enqueue(spatialProcessor->sweep());
    });

    timer->start(50);

}
