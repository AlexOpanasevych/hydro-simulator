# How to use
```c
#include "vdebug.h"
```
#### Display message
```c
VDebug::Info("Test");
VDebug::Success("Test");
VDebug::Warning("Test");
VDebug::Error("Test");
```
![alt text](img/messages.PNG)

#### Display array
```c
VDebug::Hex(arr);
VDebug::Hex("MyArr: ", arr);
```
![alt text](img/array.PNG)