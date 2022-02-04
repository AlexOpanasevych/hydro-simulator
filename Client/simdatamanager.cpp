#include "simdatamanager.h"

//float SimDataManager::getDistance() const
//{
//    return m_distance;
//}

//void SimDataManager::setDistance(float distance)
//{
//    m_distance = distance;
//}

//float SimDataManager::getSpeed() const
//{
//    return m_speed;
//}

//void SimDataManager::setSpeed(float speed)
//{
//    m_speed = speed;
//}

//float SimDataManager::getPelengAngle() const
//{
//    return m_pelengAngle;
//}

//void SimDataManager::setPelengAngle(float pelengAngle)
//{
//    m_pelengAngle = pelengAngle;
//}

//int SimDataManager::getBoard() const
//{
//    return m_board;
//}

//void SimDataManager::setBoard(int board)
//{
//    m_board = board;
//}

//float SimDataManager::getCourseAngle() const
//{
//    return m_courseAngle;
//}

//void SimDataManager::setCourseAngle(float courseAngle)
//{
//    m_courseAngle = courseAngle;
//}

SimDataManager::SimDataManager(QObject *parent) : QObject(parent)
{

}

void SimDataManager::fromMap(const QVariantMap &data)
{
    setBoard(data["board"].toInt());
    setCourseAngle(data["course"].toDouble());
    setDistance(data["distance"].toDouble());
    setPelengAngle(data["peleng"].toDouble());
    setSpeed(data["speed"].toDouble());
}
