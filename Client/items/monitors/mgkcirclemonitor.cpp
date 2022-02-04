#include "mgkcirclemonitor.h"

#include <QSGGeometryNode>
#include <QSGSimpleRectNode>
#include <streamconnector.h>
#include <cmath>
#include <QSGMaterialShader>
#include <QOpenGLExtraFunctions>
#include <QBitmap>

#include <QPainter>

#include <items/painters/tucpainter.h>

//class DashShader : public QSGMaterialShader
//{
////    QColor m_color;
//    int m_id_matrix  ;
////    int m_id_color   ;
//    int m_id_resolution ;
//    int m_id_factor;
//    int m_id_pattern;

//public:
//    DashShader() {

//    }

//    const char *vertexShader() const override {
//        return
//        "#version 330                                     \n"
//        "layout (location = 0) in vec3 inPos;             \n"
//        "flat out vec3 startPos;                          \n"
//        "out vec3 vertPos;                                \n"
//        "uniform mat4 u_mvp;                              \n"
//        "void main()                                      \n"
//        "{                                                \n"
//        "    vec4 pos    = u_mvp * vec4(inPos, 1.0);      \n"
//        "    gl_Position = pos;                           \n"
//        "    vertPos     = pos.xyz / pos.w;               \n"
//        " startPos    = vertPos;                       \n"
//        "}";
//    }

//    const char *fragmentShader() const override {
//        return
//                "#version 330                                                  \n"
//                "flat in vec3 startPos;                                        \n"
//                "in vec3 vertPos;                                              \n"
//                "out vec4 fragColor;                                           \n"
//                "uniform vec2  u_resolution;                                   \n"
//                "uniform uint  u_pattern;                                      \n"
//                "uniform float u_factor;                                       \n"
//                "void main()                                                   \n"
//                "{                                                             \n"
//                "    vec2  dir  = (vertPos.xy-startPos.xy) * u_resolution/2.0; \n"
//                "    float dist = length(dir);                                 \n"
//                "                                                              \n"
//                "    uint bit = uint(round(dist / u_factor)) & 15U;            \n"
//                "    if ((u_pattern & (1U<<bit)) == 0U)                        \n"
//                "        discard;                                              \n"
//                "    fragColor = vec4(1.0);                                    \n"
//                "}";
//    }

//    const char *const * attributeNames() const override {
//        static char * const names[] = {(char* const)"vertex", 0};
//        return names;
//    }

//    void initialize() override
//    {
//        QSGMaterialShader::initialize();
//        m_id_matrix   = program()->uniformLocation("u_mvp");
//        m_id_resolution  = program()->uniformLocation("u_resolution");
//        m_id_factor  = program()->uniformLocation("u_factor");
//        m_id_pattern  = program()->uniformLocation("u_pattern");
//        program()->setUniformValue(m_id_pattern, 2.0f);
//        program()->setUniformValue(m_id_factor, 0x18ff);
//        QVector3D p0(-1.0f, -1.0f, 0.0f);
//        QVector3D p1( 1.0f, -1.0f, 0.0f);
//        QVector3D p2( 1.0f,  1.0f, 0.0f);
//        QVector3D p3(-1.0f,  1.0f, 0.0f);
//        QVector<QVector3D> varray1{ p0, p1, p1, p2, p2, p3, p3, p0 };

////        auto CreateVAO = [](QVector<QVector3D> &varray) -> GLint
////        {
////            GLuint bo[2], vao;
////            QOpenGLExtraFunctions glFuncs(QOpenGLContext::currentContext());

////            glFuncs.glGenBuffers(2, bo);
////            glFuncs.glGenVertexArrays(1, &vao);
////            glFuncs.glBindVertexArray(vao);
////            glFuncs.glEnableVertexAttribArray(0);
////            glFuncs.glBindBuffer(GL_ARRAY_BUFFER, bo[0] );
////            glFuncs.glBufferData(GL_ARRAY_BUFFER, varray.size()*sizeof(*varray.data()), varray.data(), GL_STATIC_DRAW);
////            glFuncs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

////            return vao;
////        };

////        GLuint vao1 = CreateVAO(varray1);

////        QVector<QVector3D> varray2;
////            for (size_t u=0; u <= 360; u += 8)
////            {
////                double a = u * M_PI/180.0;
////                double c = cos(a), s = sin(a);
////                varray2.push_back(QVector3D((float)c, (float)s, 0.0f));
////            }
////            GLuint vao2 = CreateVAO(varray2);

////            QMatrix4x4 m(project);
////            int vpSize[2]{0, 0};

//    }

//    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override
//    {
//        Q_ASSERT(program()->isLinked());
//        if (state.isMatrixDirty())
//            program()->setUniformValue(m_id_matrix, state.combinedMatrix());
//        program()->setUniformValue(m_id_resolution, QVector2D(state.deviceRect().width(), state.deviceRect().height()));

//    }
////    QColor color() const;
////    void setColor(const QColor &color);


//};

//class Material : public QSGMaterial
//{
//public:
//    QSGMaterialType *type() const { static QSGMaterialType type; return &type; }
//    QSGMaterialShader *createShader() const { return new DashShader; }
//};

//class DashLineNode : public QSGGeometryNode
//{
//public:
//    DashLineNode(/*const QColor & color*/)
//    {
////        setGeometry(&m_geometry);
////        m_geometry.setDrawingMode(GL_LINES);

//        QSGMaterial *material = new Material;

//        material->setFlag(QSGMaterial::Blending);
//        setMaterial(material);
//        setFlag(OwnsMaterial);
//    }

//    QSGGeometry  *m_geometry;
//};




MGKCircleMonitorPrivate::MGKCircleMonitorPrivate(QQuickItem *parent) : MGKAbstractMonitor(parent)
{
    connect(this, &MGKAbstractMonitor::sizeChanged, this, &MGKCircleMonitorPrivate::onSizeChanged);
    setActivePainter(new TucPainter(this)); // default

}

void MGKCircleMonitorPrivate::paint(QPainter *painter)
{
    painter->setBrush(backgroundColor());
    painter->setPen(QPen(backgroundColor(), 0));
    painter->drawEllipse(boundingRect());

    MGKAbstractMonitor::paint(painter);

    painter->setPen(QPen(QColor("#444444"), 1));

    painter->translate(width() / 2, height() / 2);
    painter->rotate(m_angle);
    painter->translate(-width() / 2, -height() / 2);

    painter->setClipRegion(getMaskAsRegion());

    linesCount--;
    const int spacing = static_cast<int>(height() / linesCount);
    const int margin = static_cast<int>((height() - (spacing * linesCount)) / 2);
    const int x1 = static_cast<int>(margin + spacing);
    const int x2 = static_cast<int>(width() - margin - spacing);
    linesCount++;

    painter->drawLine(x1, static_cast<int>(height() * 0.5), x2, static_cast<int>(height() * 0.5));


    for(int i = margin; i < width(); i += spacing) {
        painter->drawLine(i, 0, i, static_cast<int>(height()));
    }

    painter->setPen(QPen(QColor("#444444"), 2, Qt::PenStyle::DashLine));
    painter->drawLine(x1, static_cast<int>(height() * 0.5 + yDeviation), x2, static_cast<int>(height() * 0.5 - yDeviation));

}

void MGKCircleMonitorPrivate::onSizeChanged()
{
//    QImage mask(static_cast<int>(width()), static_cast<int>(height()), QImage::Format_ARGB32);
    if(mask.isNull()) mask = QImage(static_cast<int>(width()), static_cast<int>(height()), QImage::Format_ARGB32);
    else mask = mask.scaled(static_cast<int>(width()), static_cast<int>(height()));
    mask.fill(0);
    if(!mask.isNull()) {
        QPainter maskPainter(&mask);
        maskPainter.setBrush(Qt::darkGray);
        maskPainter.setPen(QPen(Qt::darkGray, 0));
        maskPainter.drawEllipse(boundingRect());
    }
//    this->mask = mask;
}

//QSGNode *MGKCircleMonitorPrivate::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
//{
//    QSGClipNode * node = nullptr;
//    if(!oldNode) {
//        auto clipNode = new QSGClipNode();
//        clipNode->setIsRectangular(false);

//        auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 360 + 2);
//        geometry->setDrawingMode(GL_TRIANGLE_FAN);

//        int r_px = width() / 2.0;
//        int r_py = height() / 2.0;

//        geometry->vertexDataAsPoint2D()[0].set(r_px, r_py);

//        for (int i = 1; i <= 361; ++i)
//        {
//            double rad = (i - 90) * M_PI / 180.;
//            geometry->vertexDataAsPoint2D()[i].set(std::cos(rad) * r_px + r_px, std::sin(rad) * r_py + r_py);
//        }

//        clipNode->setGeometry(geometry);
//        clipNode->setFlag(QSGNode::OwnsGeometry);
////
//        QSGSimpleRectNode * rectNode = new QSGSimpleRectNode(boundingRect(), backgroundColor());
//        clipNode->appendChildNode(rectNode);

//        const int spacing = static_cast<int>(height() / (linesCount - 1));
//        const int margin = static_cast<int>((height() - (spacing * (linesCount - 1))) / 2);
//        const int x1 = static_cast<int>(margin + spacing);
//        const int x2 = static_cast<int>(width() - margin - spacing);
//        {
//            auto lineNode = new QSGGeometryNode();
//            auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), linesCount * 2);
//            geometry->setDrawingMode(QSGGeometry::DrawLines);
//            auto material = new QSGFlatColorMaterial();
//            material->setColor("#444444");
//            lineNode->setMaterial(material);
//            lineNode->setGeometry(geometry);
//            lineNode->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);

//            geometry->vertexDataAsPoint2D()[0].set(x1, height() / 2);
//            geometry->vertexDataAsPoint2D()[1].set(x2, height() / 2);
//            int startMargin = margin;
//            for(int i = 2; i < linesCount * 2; i+=2) {
//                geometry->vertexDataAsPoint2D()[i].set(startMargin, 0);
//                geometry->vertexDataAsPoint2D()[i + 1].set(startMargin, height());
//                startMargin+=spacing;
//            }

//            clipNode->appendChildNode(lineNode);
//        }

//        {
//            auto line = QVector2D(x2, r_py + yDeviation) - QVector2D(x1, r_py - yDeviation);
//            auto lineLength = line.length();
//            const int gap = 4;
//            auto lineNode = new QSGGeometryNode();
//            auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), (int)(2 * ((lineLength - gap) / (2 * gap))) + 2);
//            geometry->setDrawingMode(GL_LINES);
//            geometry->setLineWidth(2);
//            auto material = new QSGFlatColorMaterial();
//            material->setColor("#444444");
//            lineNode->setMaterial(material);
//            lineNode->setGeometry(geometry);
//            lineNode->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);

//            for (int i = 0, size = geometry->vertexCount(); i < size; i++) {
//                geometry->vertexDataAsPoint2D()[ i ].set(x2 - i * gap * line.normalized().x(), r_py - yDeviation + i * gap * line.normalized().y());
//            }

//            clipNode->appendChildNode(lineNode);
//        }

//        QSGNode * afterShiningNode = new QSGNode;
//        clipNode->appendChildNode(afterShiningNode);
////        {
////            auto lineNode = new DashLineNode();
////            auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
////            geometry->setDrawingMode(GL_LINES);
////            geometry->setLineWidth(2);
//////            QSGGeometry::updateRectGeometry(lineNode->geometry(), boundingRect());
//////                static_cast<QSGSimpleMaterial<State>*>(n->material())->state()->color = color;

////            lineNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

//////            auto geometry = lineNode->geometry();

////            geometry->vertexDataAsPoint2D()[0].set(x1, r_py - yDeviation);
////            geometry->vertexDataAsPoint2D()[1].set(x2, r_py + yDeviation);
////            lineNode->setGeometry(geometry);
////            clipNode->appendChildNode(lineNode);
////        }

//        node = clipNode;
//    }
//    else {
//        node = dynamic_cast<QSGClipNode*>(oldNode);
//        setPaintNode(node->lastChild());
//        updateSurface();
//    }

//    return node;
//}
