#ifndef FSM_H
#define FSM_H

#include <pthread.h>
#include <map>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <list>

#include "dataType.h"
#include "commonType.h"
#include "message.h"
#include "base.h"
#include "log.h"

class Engine;

class Transition
{
public:
    WorkMode modeFrom;
    WorkMode modeTo;
    EVENT_CODE event;
    void (*onTransition)();
};

class FSM : public Base
{
public:
    static FSM* getInstance();                                  //get instance of FSM
    FSM();
    FSM(WorkMode initworkmode);                                 //default constructor
    ~FSM();                                                     //destructor
    void Init(void *data);                                      //plugin initialize
    static void * Run(void *arg);                               //run plugin
    void handleMsg(void *msg,void *data);
    void startNewThread(void *(threadFunc)(void *),void *data); //create a thread(add a msg of create thread, to list of <engine>, create thread at <engine>)
    void sendMessage(void *msg,void *data);

    typedef t_int (FSM::*func)(void *msg, void *data);
    static const func FSMAPI[];

private:
    void generateFSM();
    void addTransition(WorkMode modeFrom, WorkMode modeTo, EVENT_CODE event,
                      void (*onTransition)());
    t_int trigger(void *msg, void *data);
    Transition createTransition(WorkMode modeFrom, WorkMode modeTo,
                                      EVENT_CODE event, void (*onTransition)());

    static void modeChangeDailyToEmergency();
    static void modeChangeDailyToCapture();
    static void modeChangeDailyToRecord();
    static void modeChangeDailyToRemote();
    static void modeChangeRemoteToEmergency();
    static void modeChangeRemoteToCapture();
    static void modeChangeRemoteToRecord();
    static void modeChangeRemoteToDaily();
    static void modeChangeRecordToEmergency();
    static void modeChangeRecordToCapture();
    static void modeChangeRecordToDaily();
    static void modeChangeCaptureToEmergency();
    static void modeChangeCaptureToRecord();
    static void modeChangeCaptureToDaily();
private:
    static FSM* mInstance;
    Engine *ctxEngine;

    WorkMode mCurrentWorkMode;
    std::list<Transition> mTransitionList;
    pthread_mutex_t mutexTransitionList;

};

#endif // FSM
