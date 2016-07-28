#!/bin/sh

CXXFLAGS="-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gstreamer-1.0  
          -I./engine/include -I./fsm/include -I./gstpipeline/include -I./socket/include 
          -I./engine/src -I./fsm/src -I./gstpipeline/src -I./socket/src "
SOURCECODE="engine/src/base.cpp engine/src/engine.cpp engine/src/log.cpp 
            engine/src/threadManager.cpp engine/src/main.cpp
            fsm/src/fsm.cpp fsm/src/fsmApi.cpp
            gstpipeline/src/videomonitor.cpp gstpipeline/src/videomonitorApi.cpp
            socket/src/pluginSocket.cpp socket/src/pluginSocketApi.cpp
            engine/src/cJSON.c"
OUTPUT_BIN="DVR"

CXXLIBS="-L/usr/lib/x86_64-linux-gnu -Wl,--no-as-needed -lpthread -Wl,--no-as-needed -lglib-2.0 -Wl,--no-as-needed -lgstreamer-1.0 -Wl,-lm,-lgobject-2.0"

#g++ -v -Wall -Werror -g -std=c++11 $CXXFLAGS $CXXLIBS $SOURCECODE -o $OUTPUT_BIN 2> build.log
g++ -Wall -Werror -Wall -Wno-sign-compare -Wall -Wno-conversion-null -Wall -Wno-return-type -g -std=c++11 $CXXFLAGS $CXXLIBS $SOURCECODE -o $OUTPUT_BIN 2> build.log

#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS base.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS engine.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS log.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS plugin.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS thread_manager.cpp
#g++ -c -Wall -Werror -g -std=c++11 $CXXFLAGS main.cpp

#g++ -Wall -Werror -g -std=c++11 $CXXLIBS base.o engine.o log.o plugin.o thread_manager.o main.o -o $OUTPUT_BIN
