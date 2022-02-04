#ifndef SAMPLADATACONTAINER_H
#define SAMPLADATACONTAINER_
#include "circularbuffer.h"
#include <vector>
#include <map>
#include "core_global.h"


//class CircularBuffer;
class CORE_LIBRARY_EXPORT SDInfo
{
public:
    int dt;
    bool isDataPeriodic;
    QString SDType;
    int start = -1;
    CircularBuffer<SampleData> *buffer;
private:
};
class CORE_LIBRARY_EXPORT SampleDataContainer
{
    friend class Logger;
public:
    SampleDataContainer();
    ~SampleDataContainer();
    template<class T>
    std::vector<T> getSapmleRange(int piDescriptor ,  int lsecond , int rsecond);
    template<class T>
    T getSample(int piDescriptor , int uSecond);
    template<class T>

    T * getLastSample(int piDescriptor );
    template<class T>
    void pushSample(int piDescriptor , T* data);
    void CreateNewRecord(int piDescriptor  , QString type , bool isdataperiodic , int dt = -1);
    void clear();
private:
    static QMutex m;
    std::map<int , std::vector<SDInfo> > data;//piDescriptor - vector of datas
    std::vector<int> set_of_piDescriptors;
    std::map<std::pair<int , QString> , int > map_of_index;//Type->index
};
template <class T>
void SampleDataContainer::pushSample(int piDescriptor, T* dat)
{
    int RecordPosition = map_of_index[{piDescriptor , dat->getType()}];
    data[piDescriptor][RecordPosition].buffer->clearToSaveZone();
    SampleData* sd;
    sd= static_cast<SampleData*>(dat);
    data[piDescriptor][RecordPosition].buffer->push(sd);
    return;
}
template<class T>
T * SampleDataContainer::getLastSample(int piDescriptor)
{
    if(!map_of_index.empty() and !data.empty()) {
        int RecordPosition = map_of_index[std::make_pair(piDescriptor , T().getType())];
    //    if(buffer)
        SampleData *a = (data[piDescriptor][RecordPosition].buffer->peek());
        return static_cast<T*>(a);

    }
    return nullptr;
}
template <class T>
T SampleDataContainer::getSample(int piDescriptor  , int uSecond)
{
    int RecordPosition = map_of_index[std::make_pair(piDescriptor , T().getType())];
    T result = *static_cast<T*>(data[piDescriptor][RecordPosition].buffer->watch(uSecond));
    return result;
}
template<class T>
std::vector<T> SampleDataContainer::getSapmleRange(int piDescriptor  , int lsecond , int rsecond)
{
    int RecordPosition = map_of_index[std::make_pair(piDescriptor , T().getType())];
    std::vector<SampleData*> res;
    res=  data[piDescriptor][RecordPosition].buffer->watchInRange(lsecond , rsecond );
    std::vector<T> result;
    for (int i=0;i<int(res.size());i++)
    {
        result.push_back(*static_cast<T*>(res[i]));
    }
    return result;
}
#endif // SAMPLADATACONTAINER_H
