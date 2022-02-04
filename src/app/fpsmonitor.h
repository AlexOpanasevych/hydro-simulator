#ifndef FPSMONITOR_H
#define FPSMONITOR_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QSGGeometryNode>
#include <QSGTexture>
#include <QSGMaterial>
#include <QSGFlatColorMaterial>
#include <libs/klibcorelite/kmacro.h>
#include <QPointF>
#include <QSGSimpleMaterial>

struct ColorStruct
{
    float r, g, b, a;
};

class GraphMaterial : public QSGSimpleMaterialShader<ColorStruct>
{
    QSG_DECLARE_SIMPLE_SHADER(GraphMaterial, ColorStruct)
public:

    const char *vertexShader() const {
        return
        "attribute highp vec4 aVertex;              \n"
        "uniform highp mat4 qt_Matrix;              \n"
        "varying highp vec2 texCoord;               \n"
        "void main() {                              \n"
        "    gl_Position = qt_Matrix * aVertex;     \n"
        "    texCoord = vec2(1.0, 0.7);             \n"
        "}";
    }

    const char *fragmentShader() const {
        return
        "uniform lowp float qt_Opacity;             \n"
        "uniform lowp vec4 color;                   \n"
        "varying highp vec2 texCoord;               \n"
        "void main ()                                               \n"
        "{                                                          \n"
        "     vec2 u_c = vec2(0.5,0.5);                             \n"
        "     float distanceFromLight = length(texCoord.xy - u_c);  \n"
        "     gl_FragColor = mix(vec4(0.,0.,0.,0.), color, distanceFromLight*0.5) * qt_Opacity;  \n"
        "}";
    }

    QList<QByteArray> attributes() const {
        return QList<QByteArray>() << "aVertex" ;
    }

    void updateState(const ColorStruct *color, const ColorStruct *) {
        program()->setUniformValue("color", color->r, color->g, color->b, color->a);
    }

};

class GraphicLine : public QSGGeometryNode
{
public:
    GraphicLine();
    QSGGeometry *m_geometry = nullptr;
    QSGFlatColorMaterial *m_material = nullptr;
    void updatePoints(QVector<double> points);
    QColor color = "green";
    double width = 200;
    double height = 200;
};

class GraphicLineBackground : public QSGGeometryNode
{
public:
    GraphicLineBackground();
    QSGGeometry *m_geometry = nullptr;
    QSGSimpleMaterial<ColorStruct> *m_material = nullptr;
    void updatePoints();
    double width = 200;
    double height = 200;
    QColor color = "green";
};

class GraphicFrame : public QSGGeometryNode
{
public:
    GraphicFrame();
    QSGGeometry *m_geometry = nullptr;
    QSGFlatColorMaterial *m_material = nullptr;
    void updatePoints();
    QColor color = "green";
    double width = 200;
    double height = 200;
};

class FPSMonitor: public QQuickItem
{
    Q_OBJECT
    K_QML_TYPE(FPSMonitor)
    K_AUTO_PROPERTY(double, fps, fps, setFps, fpsChanged, 0)
    K_AUTO_PROPERTY(bool, working, working, setWorking, workingChanged, true)
    K_READONLY_PROPERTY(QColor, color, color, setColor, colorChanged, "green")
    public:
        FPSMonitor(QQuickItem *parent = 0);
    ~FPSMonitor();

private:
    void recalculateFPS();
    double _currentFPS;
    int _cacheCount;
    QVector<qint64> _times;
    QVector<double> m_history;
    GraphicLine *m_graphLine = nullptr;
    GraphicLineBackground *m_graphLineBackground = nullptr;
    GraphicFrame *m_graphFrame = nullptr;
    QSGClipNode * m_clipNode = nullptr;
    QSGTexture *m_graphText = nullptr;

    void updateClipNode(QVector<double> points);

    // QQuickItem interface
protected:
    virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};

#endif // FPSMONITOR_H
