#include "trajectory.h"
#include <math.h>
#include <QRegularExpression>
#include <libs/PluginsBase/tools/trajectoryparser.h>
#include <libs/PluginsBase/tools/cmd.h>

Trajectory::Trajectory(QObject *parent) : QObject(parent)
{
    auto p = qobject_cast<SceneItem*>(parent);
    setItem(p);

    connect(this, &Trajectory::propertiesChanged, this, &Trajectory::updateProperties);
    //connect(this, &Trajectory::xChanged, this, &Trajectory::updateProperties);
    //connect(this, &Trajectory::yChanged, this, &Trajectory::updateProperties);
}

void Trajectory::updateProperties()
{
    double R = item()->property("rotation").toDouble();
    double xs = x(), ys = -y();

    m_trajectories.clear();
    m_vertexes.clear();
    cmdPos.clear();


    TrajectoryParser parser;
    bool isError = false;
    auto cmds = parser.parse(properties(), &isError);

    if (!isError)
    {
        int pos = 0;

        for (auto cmd : *cmds) {
            auto tokens = cmd->params();
            if (cmd->name() == "FORWARD") {
                auto d = getParamValue(cmds, cmd, "D");
                auto z = getParamValue(cmds, cmd, "Z");
                auto alpha = M_PI * R / 180;
                auto b = new LineSegment(this);
                b->setDepth(z);
                double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
                x1 = xs;
                y1 = ys;
                xs += d * sin(alpha);
                ys += d * cos(alpha);
                x2 = xs;
                y2 = ys;
                b->setPoints({{x1, y1}, {x2, y2}});
                m_trajectories.push_back(b);
                m_vertexes.push_back(b);
                cmdPos.push_back(pos);
            } else if (cmd->name() == "ROTATE") {

                auto a = getParamValue(cmds, cmd, "A");
                auto r = getParamValue(cmds, cmd, "R");

                auto b = new CircleSegment(this);
                b->setup({xs, ys},r,R,a);
                b->build({},{});
                m_trajectories.push_back(b);
                m_vertexes.push_back(b);
                auto ep = b->getEndPoint();
                xs = ep.x();
                ys = ep.y();
                R += a;
                cmdPos.push_back(pos);

            } else if (cmd->name() == "FIRE") {
                auto lb = getParamValue(cmds, cmd, "LB");
                auto rb = getParamValue(cmds, cmd, "RB");
                auto p = getParamValue(cmds, cmd, "P");
                auto a = 0;
                if (p != 0) a = p;
                else if (lb != 0) a = item()->property("rotation").toDouble() + lb;
                else if (rb != 0) a = item()->property("rotation").toDouble() + rb;

                auto d = 100.;
                auto alpha = M_PI * a / 180;
                auto b = new FireSegment(this);
                b->setColor(Qt::blue);

                double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
                x1 = xs;
                y1 = ys;
                x2 = xs + d * sin(alpha);
                y2 = ys + d * cos(alpha);
                b->setPoints({{x1, y1}, {x2, y2}});
                m_trajectories.push_back(b);
                m_vertexes.push_back(b);
                cmdPos.push_back(pos);
            }
            pos += cmd->getLength() + 1;
        }
        emit vertexesChanged(vertexes());
    }

}

void Trajectory::selectTrajectory(int place)
{

    for (auto traj : vertexes())
    {
        traj->setSelected(false);
    }
    if (place == -1)
    {
        emit vertexesChanged(m_vertexes);
        return;
    }

    for (int  i = 1; i< cmdPos.count(); ++i) {
        if (place <= cmdPos[i]) {
            m_vertexes[i - 1]->setSelected(true);
            break;
        }
    }
    if (cmdPos.length() > 0)
        if (cmdPos.last() < place)
            m_vertexes.last()->setSelected(true);

    emit vertexesChanged(m_vertexes);
}

void Trajectory::move(QPointF point)
{
    setX(point.x());
    setY(point.y());
    updateProperties();
}

double Trajectory::getParamValue(QList<Cmd *> *list, Cmd *cmd, QString param)
{
    auto p = cmd->getParameter(param);
    if (p == nullptr) return -1;

    if (p->isValid) {
        if (p->modifier == "-") return -p->value;
        else return p->value;
    }
    else
    {
        int idx = list->indexOf(const_cast<Cmd*>(cmd));
        if (idx == -1 || idx - 1 < 0)
        {
            if (param == QString("V")) return item()->property("velocity").toDouble();
            else if (param == QString("D")) return 0; //Distance
            else if (param == QString("Z")) return item()->property("z").toDouble();
            else if (param == QString("ACC")) return 0; //
            else if (param == QString("DZ")) return 0;
            else if (param == QString("A")) return 0;
            else if (param == QString("R")) return 0;
            else if (param == QString("P")) return 0;
            else if (param == QString("LB")) return 0;
            else if (param == QString("RB")) return 0;
        }
        else
        {
            return getParamValue(list, list->at(idx - 1), param);
        }
    }
    return 0.0;
}

void CircleSegment::setup(QPointF p, double r, double K, double angle)
{
    using namespace std::complex_literals;
    std::complex<double> tmp = 1i * M_PI/180.0 * (90 - K);
    std::complex<double> z = r * (angle > 0 ? -1i : 1i);
    z *= std::exp(tmp);
    setC({p.x() + z.real(), p.y() + z.imag()});
    R = r;
    fromAngle = 180 + 180/M_PI*std::arg(z);
    toAngle = fromAngle - angle;
}

void CircleSegment::build(QPointF p1, QPointF p2)
{
    Q_UNUSED(p2)
    Q_UNUSED(p1)
    QVector<QPointF> result;
    for (int  i = 0; i <= pointsCount; ++i) {
        double angle = (M_PI/180) * (fromAngle + (i) * (toAngle-fromAngle) / pointsCount);
        result.push_back({c().x()+R*cos(angle), Y_SWAP*(c().y()+R*sin(angle))});
    }
    setPoints(result);
}

QPointF CircleSegment::getEndPoint()
{
    double angle = (M_PI/180) * (toAngle);
    return { c().x()+R*cos(angle), c().y()+R*sin(angle) };
}

double CircleSegment::getEndCourse()
{
    return 90 + toAngle;
}

