attribute highp vec4 qt_Vertex;
attribute highp vec2 qt_MultiTexCoord0;

uniform highp mat4 qt_Matrix;
uniform highp float bend;
uniform highp float minimize;
uniform highp float side;
uniform highp float width;
uniform highp float height;

varying highp vec2 qt_TexCoord0;

void main() {
    qt_TexCoord0 = qt_MultiTexCoord0;
    highp vec4 pos = qt_Vertex;
    pos.x = mix(qt_Vertex.x, width, minimize);
    highp float t = pos.x / width;
    t = (3. - 2. * t) * t * t;
    pos.y = mix(qt_Vertex.y, side * height, t * bend);
    gl_Position = qt_Matrix * pos;
}
