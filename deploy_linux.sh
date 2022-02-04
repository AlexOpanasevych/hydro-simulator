#!/bin/bash

cqtdeployer force-clear -bin ~/Projects/2020/torpedosimulator/build_release/src/app/app -qmake ~/Qt/5.15.2/gcc_64/bin/qmake -extraPlugin ~/Projects/2020/torpedosimulator/bin/plugins/ -libDir ~/Projects/2020/torpedosimulator/bin/libs/ -targetDir ~/Projects/2020/torpedosimulator/deploy -qmlDir ~/Projects/2020/torpedosimulator/src/app/

