#include "setshippathmsg.h"

void SetShipPathMsg::serialize(QDataStream *s) const
{
//    *s << (int)this->path.size();
//    for(auto coords : this->path) {
//        *s << coords[0] << coords[1] << coords[2];
//    }
//    *s << (int)this->torpedoEjectPoints.size();
//    for(auto coords : this->torpedoEjectPoints) {
//        *s << coords[0] << coords[1] << coords[2];
//    }
//    *s << (int)this->velocity.size();
//    for(auto pointVelocity : velocity){
//        *s << pointVelocity;
//    }
//    *s << (int)this->angle.size();
//    for(auto currAngle : angle){
//        *s << currAngle;
//    }
    *s << cmds;
}

void SetShipPathMsg::deserialize(QDataStream *s)
{
//    int size;

//    *s >> size;
//    for(int i = 0; i < size; i++) {
//        double x, y, z;
//        *s >> x >> y >> z;
//        this->path.push_back({x, y, z});
//    }

//    *s >> size;
//    for(int i = 0; i < size; i++) {
//        double x, y, z;
//        *s >> x >> y >> z;
//        this->torpedoEjectPoints.push_back({x, y, z});
//    }

//    *s >> size;
//    for(int i = 0; i < size; i++){
//        double vel;
//        *s >> vel;
//        velocity.push_back(vel);
//    }

//    *s >> size;
//    for(int i = 0; i < size; i++){
//        double ang;
//        *s >> ang;
//        angle.push_back(ang);
//    }
    *s >> cmds;
}

void SetShipPathMsgAns::serialize(QDataStream *s) const
{
    *s << this->success;
}

void SetShipPathMsgAns::deserialize(QDataStream *s)
{
    *s >> this->success;
}
