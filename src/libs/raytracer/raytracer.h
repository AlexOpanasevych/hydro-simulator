#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QWidget>
#include <src/lib/ktools/kpropertyshell.h>
#include <src/lib/utility/propertyhelper.h>
//for CustomButton class
#include <QPushButton>
#include <QStylePainter>
#include <QStyleOptionGraphicsItem>



class CustomButton : public QPushButton
{
    Q_OBJECT
    public:
        enum Orientation {
            Horizontal,
            VerticalTopToBottom,
            VerticalBottomToTop
        };

        CustomButton(QWidget * parent = nullptr): QPushButton(parent) {}
        CustomButton(const QString & text, QWidget *parent = nullptr) : QPushButton(text, parent) {}

        QSize sizeHint() const
        {
            QSize sh = QPushButton::sizeHint();

                if (m_Orientation != CustomButton::Horizontal)
                {
                    sh.transpose();
                }
                return sh;
        }

        CustomButton::Orientation orientation() const
        {
            return m_Orientation;
        }
        void setOrientation(const CustomButton::Orientation &orientation)
        {
            m_Orientation = orientation;
        }

    protected:
        void paintEvent(QPaintEvent *event)
        {
            Q_UNUSED(event);
            QStylePainter painter(this);
            QStyleOptionButton option;
            initStyleOption(&option);

            if (m_Orientation == CustomButton::VerticalTopToBottom)
            {
                painter.rotate(90);
                painter.translate(0, -1 * width());
                option.rect = option.rect.transposed();
            }

            else if (m_Orientation == CustomButton::VerticalBottomToTop)
            {
                painter.rotate(-90);
                painter.translate(-1 * height(), 0);
                option.rect = option.rect.transposed();
            }

            painter.drawControl(QStyle::CE_PushButton, option);
        }

    private:
        Orientation m_Orientation = Horizontal;
};

class Simulation;
class SimulationGraphic;
class QCustomPlot;
class QQuickWidget;
class RayTracer : public QWidget
{
    Q_OBJECT
    KPropertyShell selfVesselDestinationDepth = "selfVesselDestinationDepth";
    AUTO_PROPERTY(bool, platinaSoundMode, platinaSoundMode, setPlatinaSoundMode, platinaSoundModeChanged, false)
protected:
    RayTracer(QWidget *parent = nullptr);
    ~RayTracer();
    static RayTracer *instance;
public:
    enum Mode {
        FullMode,
        WaveReflectionOnly,
        WaveAcceptOnly,
        OffMode
    };

    static RayTracer *getInstance();
    bool isReflected(double distance, double depth);
    bool isRecieved(double distance, double depth);
    void calculateWave(double depth);

    //getters && setters
    bool getWidgetState() const;
    void setWidgetState(bool value);

private:
    Simulation *simulation;
    SimulationGraphic *simulationGraphic;

    QCustomPlot *soundPlot;
    QCustomPlot *wavePlot;
    QQuickWidget *qml;

    bool widgetState = false;
    bool prevSoundMode = false;
    double selfVesselDepth = 0;
signals:
    void buttonClicked();
    void createMissedMessage(double x, double y);
public slots:
    void submarineChoosed(double distance, double depth);
    void setSubmarinePosition(double distance, double depth);
};

#endif // RAYTRACER_H
