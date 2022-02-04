#include "sampledatacontainer.h"

QMutex SampleDataContainer::m;

SampleDataContainer::SampleDataContainer()
{

}
SampleDataContainer::~SampleDataContainer()
{
    clear();
}

void SampleDataContainer::CreateNewRecord(int piDescriptor, QString type, bool isdataperiodic, int dt)
{
    auto pos = data.find(piDescriptor);
    if(pos==data.end())
    {
        set_of_piDescriptors.push_back(piDescriptor);
        SDInfo s;
        s.dt = dt;
        s.isDataPeriodic = isdataperiodic;
        s.SDType = type;
        s.buffer = new CircularBuffer<SampleData>;
        std::vector<SDInfo> vec;
        vec.push_back(s);
        data.insert({piDescriptor , vec});
        map_of_index.insert({{piDescriptor , type} , 0});
        return ;
    }
    SDInfo s;
    s.dt = dt;
    s.isDataPeriodic = isdataperiodic;
    s.SDType = type;
    s.buffer = new CircularBuffer<SampleData>;
    data[piDescriptor].push_back(s);
    map_of_index.insert({{piDescriptor ,type }, int(data[piDescriptor].size()-1)});
    return ;
}

void SampleDataContainer::clear()
{
    for (int indexpiDesc = 0;indexpiDesc<int(set_of_piDescriptors.size());indexpiDesc++)
    {
        int piDescriptor = set_of_piDescriptors[indexpiDesc];
        for (int recordpos = 0;recordpos<int(data[piDescriptor].size());recordpos++)
        {
//            for(int i = 0; i < buffer->size(); i++) {
//                delete buffer->pop();
//            }
            delete data[piDescriptor][recordpos].buffer;
        }
    }
    data.clear();
    set_of_piDescriptors.clear();
    map_of_index.clear();
}
