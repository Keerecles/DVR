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
#include "commonType.h"
#include "cJSON.h"
#include "threadManager.h"
#include "message.h"
#include "pluginIf.h"

#define MSG_KEY_BIND "bind"
#define MSG_KEY_DESTINATION "destination"
#define MSG_KEY_ORIGIN "origin"
#define MSG_KEY_RECEIVER "receiver"
#define MSG_KEY_OPID "functionid"
#define SOCKET_PATH "/tmp/socket_path"
#define CONNECT_CLIENT_MAX 5
#define SOCKET_MSG_LENGTH 1024
typedef int t_int;
typedef unsigned int t_uint;
enum OPERATION_CODE_SOCKET {
    E_SOCKET_FUNCTION_REGIST_IF,
    E_SOCKET_FUNCTION_UNREGIST_IF,
    E_SOCKET_FUNCTION_SEND_MSG_TO_CLIENT,
    E_SOCKET_FUNCTION_MAX_NUMBER,
};

class Engine;
class PluginSocket : public Base
{
public:
    static PluginSocket* getInstance();
    PluginSocket();
    ~PluginSocket();
    void Init(void *data);
    static void * Run(void *arg);
    void handleMsg(void * msg,void *data); // send message to application client

    typedef t_int (PluginSocket::*func)(void *msg, void *data);
    static const func socketServiceAPI[];
private:
    void startNewThread(void *(threadFunc)(void *),void *data);//create a thread(add a msg of create thread, to list of <engine>, create thread at <engine>)
    void sendMessage(void *msg,void *data);

    int InitSocket();
    static void * threadRecvConnect(void *arg);//recv connect from client
    static void * threadRecvMsg(void *arg);//recv msg from client
    t_int registIF(void *msg, void *data);
    t_int unRegistIF(void *msg, void *data);
    t_int sendMessageToClient(void *msg, void *data);
private:
    Engine * ctxEngine;

    static PluginSocket * mInstance;   //< this class instance
    std::map<std::string , int > mFdMap;//send msg fd, <className,fd_num>
    std::list<int> mFdList;//save fd
    int listenFd;
    std::map<std::string, PluginIF*> pluginIFList;//all module <className,PluginIF>
    pthread_mutex_t mutexFdList ;
};
#endif // PUGINSOCKET

