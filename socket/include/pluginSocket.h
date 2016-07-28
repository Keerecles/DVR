#ifndef PUGINSOCKET
#define PUGINSOCKET
#include <pthread.h>
#include <map>
#include <iostream>
//#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <list>
//#include <sstrseam>
#include "commonType.h"

#include "cJSON.h"
#include "threadManager.h"
#include "message.h"
class Engine;
enum OPERATION_CODE_SOCKET {
    E_SOCKET_FUNCTION_REGIST_IF,
    E_SOCKET_FUNCTION_UNREGIST_IF,
    E_SOCKET_FUNCTION_SEND_MSG_TO_CLIENT,
    E_SOCKET_FUNCTION_MAX_NUMBER,
};

class PluginSocket : public Base
{
public:
    static PluginSocket* getInstance();
    PluginSocket();
    ~PluginSocket();
    void Init(void *data);
    static void * Run(void *arg);
    int InitSocket();//niu
    void handleMsg(void * msg,void *data); // send message to application client

    typedef t_int (PluginSocket::*func)(void *msg, void *data);
    static const func socketServiceAPI[];


    static void * threadRecvConnect(void *arg);//recv connect from client
    static void * threadRecvMsg(void *arg);//recv msg from client
private:
    void startNewThread(void *(threadFunc)(void *),void *data);//create a thread(add a msg of create thread, to list of <engine>, create thread at <engine>)
    void sendMessage(void *msg,void *data);

    t_int registIF(void *msg, void *data);
    t_int unRegistIF(void *msg, void *data);
    t_int sendMessageToClient(void *msg, void *data);


private:
    static PluginSocket * mInstance;   //< this class instance
    Engine * ctxEngine;
    std::map<std::string , int > mFdMap;//send msg fd, <className,fd_num>
    std::list<int> mFdList;//save fd
    int listenFd;
    std::map<std::string, Base*> pluginIFList;//all module <className,PluginIF>
    pthread_mutex_t mutexFdList ;
};
#endif // PUGINSOCKET

