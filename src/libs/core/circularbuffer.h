#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include "sampledata.h"
#include <QDebug>
#include <QMutex>
#include "core_global.h"

template <typename T>
class CORE_LIBRARY_EXPORT CircularBuffer{
public:
    CircularBuffer(int size=100000);
    ~CircularBuffer();

    T* peek();
    int size() {return now_in_buffer;}

    T * peekNext() {
        if(isEmpty()) return nullptr;
        int rpos = read_pos;
        read_pos =  (read_pos+1) % capacity;
        if (read_pos == write_pos) m_isEmpty = true;
        return m_data[rpos];
    }

    int getCapacity();
    int getReadPos();
    int get_write_pos() {  return write_pos; }
    void clearToSaveZone();
    bool push(T* a);

    T* pop(bool *ok = nullptr);

    bool isEmpty() const {  return m_isEmpty;  }

    T *watch(int time);

    int watchPosition(int time);
    std::vector<T *> watchInRange(int start, int finish);
    bool isFPosEqualToWritePos()
    {
        return (f_pos==write_pos);
    }

    std::vector<T*> moveToWritePos();

    void clear() {

    }
private:
    const double upper_bound=0.8;
    const double lower_bound=0.7;
    std::vector< T* > m_data;
    int write_pos = 0;
    int capacity = 0;
    int read_pos = 0;
    bool m_isEmpty = true;
    int now_in_buffer = 0;
    int f_pos = 0;
    int deleted = 0;
    static QMutex m;
};

template<typename T>
CircularBuffer<T>::CircularBuffer(int size){
    m_data.resize(size);
    capacity = size;
    read_pos = 0;
    write_pos = 0;
}

template<typename T>
CircularBuffer<T>::~CircularBuffer()
{
    while(read_pos!=write_pos)
    {
//        if(m_data[read_pos] != nullptr)
            delete m_data[read_pos];
        read_pos = (read_pos+1)%capacity;
    }
//    now_in_buffer = 0;
//    m_isEmpty = true;
}

template<typename T>
T *CircularBuffer<T>::peek()
{
    if (m_isEmpty)
    {
        return nullptr;
    }
    if (write_pos==0)
    {
        return m_data[capacity-1];
    }
    else
    {
        return m_data[write_pos-1];
    }
}

template<typename T>
int CircularBuffer<T>::getCapacity()
{
    return capacity;
}

template<typename T>
int CircularBuffer<T>::getReadPos()
{
    return read_pos;
}

template<typename T>
void CircularBuffer<T>::clearToSaveZone()
{
    if((double(now_in_buffer)/double(capacity))>=upper_bound)
    {
        int num = now_in_buffer-lower_bound*capacity;
        for (int i=1;i<=num;++i)
        {
            qDebug() << read_pos << write_pos;
            deleted++;
            delete m_data[read_pos];
            read_pos = (read_pos+1)%capacity;
            now_in_buffer--;
        }
    }
    return;
}

template<typename T>
bool CircularBuffer<T>::push(T *a) {
    now_in_buffer++;
    m_data[write_pos] = a;
    write_pos = (write_pos+1) % capacity;
    m_isEmpty = false;
    return true;
}

template<typename T>
T *CircularBuffer<T>::pop(bool *ok) {
    if (m_isEmpty){
        if (ok!=nullptr) *ok = false;
    }
    now_in_buffer--;
    int rpos = read_pos;
    read_pos =  (read_pos+1) % capacity;
    if (read_pos == write_pos) m_isEmpty = true;
    if (ok!=nullptr) *ok = true;
    return m_data[rpos];
}

template<typename T>
T *CircularBuffer<T>::watch(int time)
{
    if(write_pos>read_pos)
    {
        int l , r;
        l = read_pos;
        r = write_pos-1;
        while(l<r)
        {
            int mid = (r-l)/2+l;
            if (m_data[mid]->uSecond<time)
            {
                l = mid+1;
            }
            else
            {
                r = mid;
            }
        }
        return m_data[l];
    }
    else
    {
        if (m_data[capacity-1]->uSecond<time)
        {
            int l , r;
            l = 0;
            r = write_pos-1;
            while(l<r)
            {
                int mid = (r-l)/2+l;
                if (m_data[mid]->uSecond<time)
                {
                    l = mid+1;
                }
                else
                {
                    r = mid;
                }
            }
            return m_data[l];
        }
        else
        {
            int l , r;
            l = read_pos;
            r = capacity-1;
            while(l<r)
            {
                int mid = (r-l)/2+l;
                if (m_data[mid]->uSecond<time)
                {
                    l = mid+1;
                }
                else
                {
                    r = mid;
                }
            }
            return m_data[l];
        }
    }

}

template<typename T>
int CircularBuffer<T>::watchPosition(int time)
{
    if(write_pos>read_pos)
    {
        int l , r;
        l = read_pos;
        r = write_pos-1;
        while(l<r)
        {
            int mid = (r-l)/2+l;
            if (m_data[mid]->uSecond<time)
            {
                l = mid+1;
            }
            else
            {
                r = mid;
            }
        }
        return l;
    }
    else
    {
        if (m_data[capacity-1]->uSecond<time)
        {
            int l , r;
            l = 0;
            r = write_pos-1;
            while(l<r)
            {
                int mid = (r-l)/2+l;
                if (m_data[mid]->uSecond<time)
                {
                    l = mid+1;
                }
                else
                {
                    r = mid;
                }
            }
            return l;
        }
        else
        {
            int l , r;
            l = read_pos;
            r = capacity-1;
            while(l<r)
            {
                int mid = (r-l)/2+l;
                if (m_data[mid]->uSecond<time)
                {
                    l = mid+1;
                }
                else
                {
                    r = mid;
                }
            }
            return l;
        }
    }
}

template<typename T>
std::vector<T *> CircularBuffer<T>::watchInRange(int start, int finish)
{
    int i = watchPosition(start);
    int j = watchPosition(finish);
    std::vector<SampleData*> result;
    for (  ; i-1!=j ;i =(i+1)%capacity)
    {
        result.push_back(m_data[i]);
    }
    return result;
}

template<typename T>
std::vector<T *> CircularBuffer<T>::moveToWritePos()
{
    std::vector<SampleData*> result;
    while(f_pos!=write_pos)
    {
        result.push_back(m_data[f_pos]);
        f_pos = (f_pos+1)%capacity;
    }
    return result;
}
#endif // CIRCULARBUFFER_H
