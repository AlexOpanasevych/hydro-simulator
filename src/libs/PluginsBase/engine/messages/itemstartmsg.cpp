#include "itemstartmsg.h"

void ItemStartMsg::serialize( QDataStream * s ) const
{
    *s << id << itemName;
}

void ItemStartMsg::deserialize( QDataStream * s )
{
    *s >> id >> itemName;
}

void ItemStopMsg::serialize( QDataStream * s ) const
{
    *s << id << itemName;
}

void ItemStopMsg::deserialize( QDataStream * s )
{
    *s >> id >> itemName;
}
