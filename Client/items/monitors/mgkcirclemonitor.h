#ifndef MGKCIRCLEMONITOR_H
#define MGKCIRCLEMONITOR_H

#include <items/mgkabstractmonitor.h>
#include <QSGTexture>
#include <kmacro.h>
#include <QtQuick/QSGSimpleMaterialShader>

//#define TEXTURE_SIZE 64

//struct LineMaterial
//{
//    ~LineMaterial() {
//        delete texture;
//    }

//    QColor          color;
//    QSGTexture*     texture;
//};

//class DashedLineShader : public QSGSimpleMaterialShader<LineMaterial>
//{
//    QSG_DECLARE_SIMPLE_SHADER(DashedLineShader, LineMaterial)

//    public:
//        DashedLineShader()
//    {
//        setShaderSourceFile(QOpenGLShader::Vertex, "linevshader.vsh");
//        setShaderSourceFile(QOpenGLShader::Fragment, "linefshader.fsh");
//    }

//};

class MGKCircleMonitorPrivate : public MGKAbstractMonitor
{
    Q_OBJECT
    bool firstTime = true;
    K_QML_TYPE(MGKCircleMonitorPrivate)
    K_AUTO_PROPERTY(double, angle, angle, setAngle, angleChanged, 0)
    int linesCount = 15;
    int yDeviation = 60;
    QRegion m_clipRegion;
public:
    explicit MGKCircleMonitorPrivate(QQuickItem * parent = nullptr);

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);
public slots:
    void onSizeChanged();
};

#endif // MGKCIRCLEMONITOR_H
