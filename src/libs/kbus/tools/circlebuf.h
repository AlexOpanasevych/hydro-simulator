/*!
    \brief Circular buffer for template data
    \authors oleksii
*/

#ifndef CIRCLEBUF_H
#define CIRCLEBUF_H

#include <QVector>
#include <QDebug>

template<class T>
class CircleBuf
{
public:
    CircleBuf(int size) :
        m_size(size),
        m_data(size)
    {
    }

    bool push(const T& a)
    {
        if ((write_pos == read_pos) && !isEmpty) { //full with data
            qDebug() << "OVERFLOW!!!!!!!!!!!!!!!!!!!!!!!!";
            return false;
        }
        m_data[write_pos] = a;
        write_pos = (write_pos+1) % m_size;
        isEmpty = false;
        return true;
    }

    T pop(bool *ok) {
        if (isEmpty){
            *ok = false;
            return T();
        }
        int rpos = read_pos;
        read_pos =  (read_pos+1) % m_size;
        //qDebug() << "read" << rpos;
        if (read_pos == write_pos) isEmpty = true;
        *ok = true;
        return m_data[rpos];
    }

    T take(int i){
        return m_data[(read_pos+i) % m_size];
    }

    bool bufIsEmpty(){
        return isEmpty;
    }


    void clear() {
        write_pos = 0;
        read_pos = 0;
        isEmpty = true;
    }

private:
    int write_pos = 0;
    int read_pos = 0;
    bool isEmpty = true;
    int m_size;
    QVector<T> m_data;
};

#endif // CIRCLEBUF_H
