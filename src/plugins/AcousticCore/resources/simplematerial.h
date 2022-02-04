#ifndef SIMPLEMATERIAL_H
#define SIMPLEMATERIAL_H

#include <QSGSimpleMaterial>



struct Color
{
    float r, g, b, a;
    int compare(const Color *other) const { return 1; }
};


class MinimalShader : public QSGSimpleMaterialShader<Color>
{
    QSG_DECLARE_SIMPLE_SHADER(MinimalShader, Color)
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
        "     gl_FragColor = mix(vec4(0.,0.,0.,0.), color, distanceFromLight*1.0) * qt_Opacity;  \n"
        "}";
    }

    QList<QByteArray> attributes() const {
        return QList<QByteArray>() << "aVertex" ;
    }

    void updateState(const Color *color, const Color *) {
        program()->setUniformValue("color", color->r, color->g, color->b, color->a);
    }

};
#endif // SIMPLEMATERIAL_H
