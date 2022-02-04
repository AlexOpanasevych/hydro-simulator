#include "epo160painter.h"
#include <math.h>

Epo160Painter::Epo160Painter(MGKAbstractMonitor * parent) : EPO1Painter(parent)
{
    setGroupCount(6);
    setTracksInGroup(3);
    setLeftBoardStart(245);
    setLeftBoardEnd(295);
    setRightBoardStart(65);
    setRightBoardEnd(115);

//    spatialChannels.resize(groupCount());

//    auto turnSide = [this](BoardMode value) {
//        if(value == BoardMode::RIGHT)
//            setStartPosition(endPosition);
//        else
//            setStartPosition(startEndMargin());
//    };


//    turnSide(monitor()->boardMode());
//    QObject::connect(monitor(), QOverload<BoardMode>::of(&MGKAbstractMonitor::boardModeChanged), this, turnSide);
}

//void Epo160Painter::paint(QPainter *painter)
//{
//    EPO1Painter::paint(painter);
//}
