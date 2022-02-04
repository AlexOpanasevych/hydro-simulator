#ifndef GRADIENTMATERIAL_H
#define GRADIENTMATERIAL_H

#include <QColor>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <QSGSimpleMaterialShader>
#include <math.h>

struct State
{
    QColor color;

    int compare(const State *other) const {
        uint rgb = color.rgba();
        uint otherRgb = other->color.rgba();

        if (rgb == otherRgb) {
            return 0;
        } else if (rgb < otherRgb) {
            return -1;
        } else {
            return 1;
        }
    }
};

class Shader : public QSGSimpleMaterialShader<State>
{
    QSG_DECLARE_SIMPLE_SHADER(Shader, State)
    //! [2] //! [3]
public:

    const char *vertexShader() const override {
        return
                "attribute highp vec4 aVertex;                              \n"
                "attribute highp vec2 aTexCoord;                            \n"
                "uniform highp mat4 qt_Matrix;                              \n"
                "varying highp vec2 texCoord;                               \n"
                "void main() {                                              \n"
                "    gl_Position = qt_Matrix * aVertex;                     \n"
                "    texCoord = aTexCoord;                                  \n"
                "}";
    }

    const char *fragmentShader() const override {
        return
                "uniform lowp float qt_Opacity;                             \n"
                "uniform lowp vec4 color;                                   \n"
                "varying highp vec2 texCoord;                               \n"
                "void main ()                                               \n"
                "{                                                          \n"
                "     vec2 u_c = vec2(0.5,0.5);                             \n"
                "     float distanceFromLight = length(texCoord.xy - u_c);  \n"
                "     gl_FragColor = mix(vec4(0.,0.,0.,0.), color, distanceFromLight*0.5) * qt_Opacity;  \n"
                "}";
    }
    //! [3] //! [4]
    QList<QByteArray> attributes() const override
    {
        return QList<QByteArray>() << "aVertex" << "aTexCoord";
    }
    //! [4] //! [5]
    void updateState(const State *state, const State *) override
    {
        program()->setUniformValue(id_color, state->color);
    }
    //! [5] //! [6]
    void resolveUniforms() override
    {
        id_color = program()->uniformLocation("color");
    }

private:
    int id_color;
    //! [6]
};


//! [7]
class ColorNode : public QSGGeometryNode
{
public:
    QPointF pos = {0,0};
    int radius = 1000;
    int startAngle = 0;
    int arcRadius = 360;
    int lineWidth = 200;

    ColorNode()
        : m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    {
        setGeometry(&m_geometry);
        m_geometry.setDrawingMode(GL_TRIANGLES);
        //QSGSimpleMaterial<State> *material = Shader::createMaterial();
        //material->state()->color = QColor("green");
        QSGFlatColorMaterial  *material = new QSGFlatColorMaterial();
        material->setColor("green");
        material->setFlag(QSGMaterial::Blending);
        m_trianglesCount = (arcRadius * 2 - 2);
        m_geometry.allocate((m_trianglesCount + 2) * 3, 0);
        m_geometry.vertexDataAsPoint2D()[0].set(0, 0);
        setMaterial(material);

        drawCircle(radius);

        setFlags(QSGNode::OwnsMaterial | QSGNode::OwnsGeometry);
    }

    void drawCircle(double radius){
        for (int i = 0; i <m_trianglesCount + 2; i++)
        {
            QPointF p1 = {0,0}, p2 = {0,0}, p3 = {0,0};
            //! чётные
            if ((i)%2==0){
                double radian1 = (startAngle + ((i+1)/2) - 90) * M_PI / 180;
                double radian2 = (startAngle + ((i+1)/2 + 1) - 90) * M_PI / 180;

                if (lineWidth > radius){
                    p1.setX(pos.x());
                    p1.setY(pos.y());
                }else{
                    p1.setX(std::cos(radian1) * (radius - lineWidth) + pos.x());
                    p1.setY(std::sin(radian1) * (radius - lineWidth) + pos.y());
                }

                p2.setX(std::cos(radian1) * radius + pos.x());
                p2.setY(std::sin(radian1) * radius + pos.y());

                p3.setX(std::cos(radian2) * radius + pos.x());
                p3.setY(std::sin(radian2) * radius + pos.y());
            }else{
                double radian1 = (startAngle + ((i+1)/2)  - 90) * M_PI / 180;
                double radian2 = (startAngle + ((i+1)/2) - 1 - 90) * M_PI / 180;

                if (lineWidth > radius){
                    p1.setX(pos.x());
                    p1.setY(pos.y());
                    p2.setX(pos.x());
                    p2.setY(pos.y());

                }else{
                    p1.setX(std::cos(radian1) * (radius - lineWidth) + pos.x());
                    p1.setY(std::sin(radian1) * (radius - lineWidth) + pos.y());
                    p2.setX(std::cos(radian2) * (radius - lineWidth) + pos.x());
                    p2.setY(std::sin(radian2) * (radius - lineWidth) + pos.y());
                }

                p3.setX(std::cos(radian1) * radius + pos.x());
                p3.setY(std::sin(radian1) * radius + pos.y());
            }
            m_geometry.vertexDataAsPoint2D()[i * 3].set(p1.x(), p1.y());
            m_geometry.vertexDataAsPoint2D()[i * 3 + 1].set(p2.x(), p2.y());
            m_geometry.vertexDataAsPoint2D()[i * 3 + 2].set(p3.x(), p3.y());
        }
    }

    QSGGeometry m_geometry;
    int m_trianglesCount;
};

#endif // GRADIENTMATERIAL_H
