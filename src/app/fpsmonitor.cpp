#include "fpsmonitor.h"

#include <QBrush>
#include <QDateTime>
#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <QProcess>
#include <QSGNode>
#include <QQuickWindow>

#include <math/kmath.h>

FPSMonitor::FPSMonitor(QQuickItem *parent): QQuickItem(parent), _currentFPS(0), _cacheCount(0)
{
    _times.clear();
    m_history.fill(0, 100);
    setFlag(QQuickItem::ItemHasContents);

    connect(this, &FPSMonitor::workingChanged, this, [this](){
        if (working()) update();
    });
}

FPSMonitor::~FPSMonitor()
{
}

void FPSMonitor::recalculateFPS()
{
    if (working())
    {
        qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        _times.push_back(currentTime);

        while (_times[0] < currentTime - 1000) {
            _times.pop_front();
        }

        int currentCount = _times.length();
        _currentFPS = (currentCount + _cacheCount) / 2;

        if (currentCount != _cacheCount){
            setFps(_currentFPS);
            if (_currentFPS < 20){
                setColor("red");
            }else if (_currentFPS < 45){
                setColor("orange");
            }else{
                setColor("#6ab2f2");
            }
            m_history.push_back(_currentFPS);
            while(m_history.count() > 100){
                m_history.pop_front();
            }
        }

        _cacheCount = currentCount;

        //QProcess p;
        //p.start("ps", QStringList() << "-p" << QString::number(QGuiApplication::applicationPid()) << "-o" << "%cpu,%mem");
        //p.waitForFinished();
        //QString memory = p.readAllStandardOutput();
        //qDebug() << memory << QString::number(QGuiApplication::applicationPid());
        ////setText(QString("; RAM: %1 MB").arg(memory.toLong() / 1024));
        //p.close();
    }
}

void FPSMonitor::updateClipNode(QVector<double> points)
{
    auto g = m_clipNode->geometry();
    g->allocate(points.count() + 3);
    double step = width() / points.count();
    double x = 0;
    for (int i = 0; i < points.count(); i++){
        g->vertexDataAsPoint2D()[i].set(x, KMath::instance()->dmap(70 - points[i], 0, 60, 0, height()));
        x+= step;
    }
    g->vertexDataAsPoint2D()[points.count() ].set(width(), height());
    g->vertexDataAsPoint2D()[points.count() + 1].set(0, height());
    g->vertexDataAsPoint2D()[points.count() + 2].set(0, KMath::instance()->dmap(70 - points[0], 0, 60, 0, height()));
    m_clipNode->markDirty(QSGNode::DirtyGeometry);
}

QSGNode *FPSMonitor::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (!oldNode){
        oldNode = new QSGNode();
        //m_graphFrame = new GraphicFrame();
        //m_graphLine = new GraphicLine();
        //m_clipNode = new QSGClipNode();
        //m_graphLineBackground = new GraphicLineBackground();
        //m_clipNode->setGeometry(new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4));
        //m_graphText = window()->createTextureFromImage(QImage(width(), height(), QImage::Format_RGB32));
        //oldNode->appendChildNode(m_graphFrame);
        //oldNode->appendChildNode(m_graphLine);
        //m_clipNode->appendChildNode(m_graphLineBackground);
        //oldNode->appendChildNode(m_clipNode);
        //oldNode->appendChildNode(m_graphLineBackground);
    }
    if (working()){
        recalculateFPS();

        //m_graphFrame->width = width();
        //m_graphFrame->height = height();
        //m_graphFrame->color = m_color;
        //m_graphFrame->updatePoints();
        //
        //m_graphLine->width = width();
        //m_graphLine->height = height();
        //m_graphLine->color = m_color;
        //m_graphLine->updatePoints(m_history);

        //m_graphLineBackground->width = width();
        //m_graphLineBackground->height = height();
        //m_graphLineBackground->color = m_color;
        //m_graphLineBackground->updatePoints();
        //
        //updateClipNode(m_history);

        //oldNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
        update();
    }
    return oldNode;
}


GraphicLine::GraphicLine() : m_geometry(new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4))
{
    setGeometry(m_geometry);
    m_material = new QSGFlatColorMaterial;
    m_material->setColor(color);
    setMaterial(m_material);
    m_geometry->setDrawingMode(GL_LINE_STRIP);
    m_geometry->setLineWidth(2);
    setFlags(QSGNode::OwnsMaterial | QSGNode::OwnsGeometry);
}

void GraphicLine::updatePoints(QVector<double> points)
{
    m_geometry->allocate(points.count());
    m_material->setColor(color);
    double step = width / points.count();
    double x = 0;
    for (int i = 0; i < points.count(); i++){
        m_geometry->vertexDataAsPoint2D()[i].set(x, KMath::instance()->dmap(70 - points[i], 0, 60, height * 0.1, height - height * 0.12 ));
        x+= step;
    }

    markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
}

GraphicLineBackground::GraphicLineBackground() : m_geometry(new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4))
{
    setGeometry(m_geometry);
    m_material = GraphMaterial::createMaterial();
    qreal r, g, b, a;
    color.getRgbF(&r,&g, &b, &a);
    m_material->state()->r = r;
    m_material->state()->g = g;
    m_material->state()->b = b;
    m_material->state()->a = a;
    m_material->setFlag(QSGMaterial::Blending);
    m_geometry->setDrawingMode(GL_POLYGON);
    setMaterial(m_material);
    setFlags(QSGNode::OwnsMaterial);
}

void GraphicLineBackground::updatePoints()
{
    qreal r, g, b, a;
    color.getRgbF(&r,&g, &b, &a);
    m_material->state()->r = r;
    m_material->state()->g = g;
    m_material->state()->b = b;
    m_material->state()->a = a;

    m_geometry->vertexDataAsPoint2D()[0].set(0, 0);
    m_geometry->vertexDataAsPoint2D()[1].set(0, height);
    m_geometry->vertexDataAsPoint2D()[2].set(width, height);
    m_geometry->vertexDataAsPoint2D()[3].set(width, 0);

    markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
}

GraphicFrame::GraphicFrame() : m_geometry(new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4))
{
    setGeometry(m_geometry);
    m_material = new QSGFlatColorMaterial();
    m_material->setColor(color);
    m_geometry->setLineWidth(4);
    m_geometry->setDrawingMode(GL_LINE_STRIP);
    setMaterial(m_material);
    setFlags(QSGNode::OwnsMaterial);
}

void GraphicFrame::updatePoints()
{
    m_material->setColor(color);
    m_geometry->vertexDataAsPoint2D()[0].set(0, 0);
    m_geometry->vertexDataAsPoint2D()[1].set(0, height);
    m_geometry->vertexDataAsPoint2D()[2].set(width, height);
    m_geometry->vertexDataAsPoint2D()[3].set(width, 0);

    markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
}
