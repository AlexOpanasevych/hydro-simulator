#ifndef CONTROLLERSAMPLEDATA_H
#define CONTROLLERSAMPLEDATA_H

#include "kcirclebuffer.h"
#include "sampledata.h"

struct ControllerSampleData : SampleData
{
    ControllerSampleData();
    std::string to_string() override;

};

#endif // CONTROLLERSAMPLEDATA_H
