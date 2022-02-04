#ifndef WAVEFIELDITEM_H
#define WAVEFIELDITEM_H

#include <QElapsedTimer>
#include <QQuickItem>
#include <QSGSimpleMaterialShader>
#include <QSGTexture>
#include <wave/waveitem.h>
#include <wave/wavedata.h>
#include <QFuture>
#include <QTimer>

class WaveFieldItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool updating READ updating WRITE setUpdating NOTIFY updatingChanged)

    Q_PROPERTY(double startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(double arcAngle READ arcAngle WRITE setArcAngle NOTIFY arcAngleChanged)
    Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(bool creatingWave READ creatingWave WRITE setCreatingWave NOTIFY creatingWaveChanged)
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)
public:
    explicit WaveFieldItem(QQuickItem *parent = nullptr);
    ~WaveFieldItem();

public slots:
    //! [getters]
    int fps() const;
    bool updating() const;
    double arcAngle() const;
    double lineScale() const;
    double lineWidth() const;
    bool creatingWave() const;
    double startAngle() const;
    //! [setters]
    void setFps(int fps);
    void setUpdating(bool updating);
    void setArcAngle(double arcAngle);
    void setLineWidth(double lineWidth);
    void setStartAngle(double startAngle);
    void setCreatingWave(bool creatingWave);


signals:
    void fpsChanged(int fps);
    void updatingChanged(bool updating);
    void arcAngleChanged(double arcAngle);
    void lineWidthChanged(double lineWidth);
    void startAngleChanged(double startAngle);
    void creatingWaveChanged(bool creatingWave);

    void updatePoints(const QVector<QVector<QPointF>> points);

protected:
    virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
private:
    QList<WaveItem*> m_waves;
    QVector<QVector<QPointF>> m_pointsList;
    int m_timeAlive = 20000;

    bool m_updating = true;
    bool m_creatingWave = false;
    double m_lineWidth = 20;
    double m_arcAngle = 360;
    double m_startAngle = 0;
    int m_fps = 200;
    QFuture<void> f;
};

#endif // WAVEFIELDITEM_H
