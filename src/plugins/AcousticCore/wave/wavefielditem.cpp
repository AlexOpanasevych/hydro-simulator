#include "wavefielditem.h"

#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGNode>
#include <math.h>
#include <QQmlEngine>
#include <QtGlobal>
#include <cdebug.h>
#include <QtConcurrent>

#include <QGuiApplication>
#include <acousticsimulationcore.h>

WaveFieldItem::WaveFieldItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    qRegisterMetaType<QVector<QVector<QPointF>>>("QVector<QVector<QPointF>>");

    connect(this, &WaveFieldItem::updatePoints, this, [this](const QVector<QVector<QPointF>> points){
        m_pointsList = points;
        //qDebug(v) << "update before";
        update();
        //qDebug(v) << "update after";
    }, Qt::QueuedConnection);
    connect(this, &WaveFieldItem::destroyed, this, [this](){
        m_updating = false;
    });

    auto inst = AcousticSimulationCore::instance();
    f = QtConcurrent::run([this, inst](){
        QElapsedTimer elapsedTimer;
        elapsedTimer.start();

        const int defaultuSleepTime = 5000;
        int currentFramesCount = 1, sleepTime = defaultuSleepTime;
        int visualMult = 1;
        double p180 =  M_PI / 180;
        while(m_updating){
            QList<WaveData> data = inst->getWavesData();
            QVector<QVector<QPointF>> points;

            for (auto d : data){
                int trianglesCount = abs(d.arcRadius * 2 - 2);

                QVector<QPointF> vertexPoints((trianglesCount + 2) * 3);
                for (int i = 0; i < trianglesCount + 2; i++)
                {
                    QPointF p1 = {0,0}, p2 = {0,0}, p3 = {0,0};
                    //! чётные
                    if ((i)%2==0){
                        double radian1 = (d.startAngle + ((i+1)/2) - 90) * p180;
                        double radian2 = (d.startAngle + ((i+1)/2 + 1) - 90) * p180;
                        double cos1 = std::cos(radian1);
                        double sin1 = std::sin(radian1);
                        if (d.lineWidth > d.radius){
                            p1.setX(d.pos.x());
                            p1.setY(d.pos.y());
                        }else{
                            p1.setX(cos1 * (d.radius - d.lineWidth) + d.pos.x());
                            p1.setY(sin1 * (d.radius - d.lineWidth) + d.pos.y());
                        }

                        p2.setX(cos1 * d.radius + d.pos.x());
                        p2.setY(sin1 * d.radius + d.pos.y());

                        p3.setX(std::cos(radian2) * d.radius + d.pos.x());
                        p3.setY(std::sin(radian2) * d.radius + d.pos.y());
                    }else{
                        double radian1 = (d.startAngle + ((i+1)/2) - 90) * p180;
                        double radian2 = (d.startAngle + ((i+1)/2) - 1 - 90) * p180;
                        double cos1 = std::cos(radian1);
                        double sin1 = std::sin(radian1);

                        if (d.lineWidth > d.radius){
                            p1.setX(d.pos.x());
                            p1.setY(d.pos.y());
                            p2.setX(d.pos.x());
                            p2.setY(d.pos.y());
                        }else{
                            p1.setX(cos1 * (d.radius - d.lineWidth) + d.pos.x());
                            p1.setY(sin1 * (d.radius - d.lineWidth) + d.pos.y());

                            p2.setX(std::cos(radian2) * (d.radius - d.lineWidth) + d.pos.x());
                            p2.setY(std::sin(radian2) * (d.radius - d.lineWidth) + d.pos.y());
                        }
                        p3.setX(cos1 * d.radius + d.pos.x());
                        p3.setY(sin1 * d.radius + d.pos.y());
                    }
                    vertexPoints[i * 3] = p1;
                    vertexPoints[i * 3 + 1] = p2;
                    vertexPoints[i * 3 + 2] = p3;
                }
                points.push_back(vertexPoints);
            }

            if (m_updating){
                static int k = 0;
                if (k++ >= visualMult){
                    k = 0;
                }
                emit updatePoints(points);
            }

            if (elapsedTimer.elapsed() > 1000){
                //qDebug(v) << currentFramesCount << sleepTime << data.count();
                if (currentFramesCount < fps() * 0.95){
                    sleepTime = defaultuSleepTime * currentFramesCount / fps();
                }else if (currentFramesCount > fps() * 1.05){
                    sleepTime = defaultuSleepTime * fps() / currentFramesCount;
                }
                currentFramesCount = 1;
                elapsedTimer.restart();

            }
            currentFramesCount++;
            QThread::usleep(sleepTime);
        }
    });
}

WaveFieldItem::~WaveFieldItem()
{
    m_updating = false;
    f.waitForFinished();
}

int WaveFieldItem::fps() const
{
    return m_fps;
}

bool WaveFieldItem::creatingWave() const
{
    return m_creatingWave;
}

double WaveFieldItem::lineWidth() const
{
    return m_lineWidth;
}

double WaveFieldItem::arcAngle() const
{
    return m_arcAngle;
}

double WaveFieldItem::lineScale() const
{
    return 0;
}

double WaveFieldItem::startAngle() const
{
    return m_startAngle;
}

bool WaveFieldItem::updating() const
{
    return m_updating;
}

void WaveFieldItem::setUpdating(bool updating)
{
    if (m_updating == updating)
        return;

    m_updating = updating;
    update();
    emit updatingChanged(m_updating);
}

void WaveFieldItem::setCreatingWave(bool creatingWave)
{
    if (m_creatingWave == creatingWave)
        return;

    m_creatingWave = creatingWave;
    emit creatingWaveChanged(m_creatingWave);
}

void WaveFieldItem::setLineWidth(double lineWidth)
{
    if (qFuzzyCompare(m_lineWidth, lineWidth))
        return;

    m_lineWidth = lineWidth;
    emit lineWidthChanged(m_lineWidth);
}

void WaveFieldItem::setArcAngle(double arcAngle)
{
    if (qFuzzyCompare(m_arcAngle, arcAngle))
        return;

    m_arcAngle = arcAngle;
    emit arcAngleChanged(m_arcAngle);
}

void WaveFieldItem::setStartAngle(double startAngle)
{
    if (qFuzzyCompare(m_startAngle, startAngle))
        return;

    m_startAngle = startAngle;
    emit startAngleChanged(m_startAngle);
}

void WaveFieldItem::setFps(int fps)
{
    if (m_fps == fps)
        return;

    m_fps = fps;
    emit fpsChanged(m_fps);
}

QSGNode *WaveFieldItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    //qDebug(v) << "start updating nodes";
    if (!oldNode){
        oldNode = new QSGNode();
    }else{
        QVector<QVector<QPointF>> dataList_copy(m_pointsList);

        int wavesCount = m_waves.count();
        int dataCount = dataList_copy.count();
        if (wavesCount > dataCount){
            for (int i = wavesCount - 1; i >= dataCount; i--){
                auto w = m_waves.at(i);
                m_waves.removeAll(w);
                oldNode->removeChildNode(w);
                oldNode->markDirty(QSGNode::DirtyGeometry);
                delete w;
            }
        }else if (wavesCount < dataCount){
            for (int i = 0; i < dataCount - wavesCount; i++){
                auto w = new WaveItem();
                auto d = dataList_copy[i];
                w->setPoints(d);
                oldNode->appendChildNode(w);
                m_waves.append(w);
            }
        }

        for (int i = 0; i < dataCount; i++){
            auto w = m_waves[i];
            auto d = dataList_copy[i];
            w->setPoints(d);
        }
    }
    oldNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    //qDebug(v) << "end updating nodes";
    return oldNode;
}

