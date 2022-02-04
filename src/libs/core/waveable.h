#ifndef WAVEABLE_H
#define WAVEABLE_H

//!
//! \brief The Waveable interface is used for providing wave emitting and consuming possibility
//!
class Waveable
{
public:
    Waveable();


    virtual void emit() = 0;
    virtual void consume() = 0;

};

#endif // WAVEABLE_H
