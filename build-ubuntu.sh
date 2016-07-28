#!/bin/sh

CXXFLAGS="-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gstreamer-1.0  -I/home/keerelces/Documents/Kee/Sofia/QT4Android/DVR/code/DVR/"
SOURCECODE="core/src/base.cpp core/src/engine.cpp core/src/log.cpp 
            core/src/threadManager.cpp core/src/main.cpp
            fsm/src/fsm.cpp fsm/src/fsmApi.cpp
            gstpipeline/src/videomonitor.cpp gstpipeline/src/videomonitorgst.cpp"
OUTPUT_BIN="DVR"

CXXLIBS="-L/usr/lib/x86_64-linux-gnu -Wl,--no-as-needed -lpthread -Wl,--no-as-needed -lglib-2.0 -Wl,--no-as-needed -ldl -lgstreamer-1.0"

#g++ -v -Wall -Werror -g -std=c++11 $CXXFLAGS $CXXLIBS $SOURCECODE -o $OUTPUT_BIN 2> build.log
g++ -Wall -Werror -Wall -Wno-sign-compare -Wall -Wno-conversion-null -Wall -Wno-return-type -g -std=c++11 $CXXFLAGS $CXXLIBS $SOURCECODE -o $OUTPUT_BIN 2> build.log

#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS base.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS engine.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS log.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS plugin.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS thread_manager.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS main.cpp

#g++ -Wall -Werror -g -std=c++11 $CXXLIBS base.o engine.o log.o plugin.o thread_manager.o main.o -o $OUTPUT_BIN
