#include "pluginSocket.h"
#include "engine.h"
PluginSocket *PluginSocket::mInstance = NULL;
PluginSocket::PluginSocket():Base("pluginSocket")
{
    PluginSocket::mInstance = this;
    mutexFdList  = PTHREAD_MUTEX_INITIALIZER;
}
PluginSocket::~PluginSocket()
{
    delete mInstance;
}

PluginSocket * PluginSocket::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new PluginSocket();
    }
    return mInstance;
}

void PluginSocket::Init(void *data)
{
    ctxEngine = static_cast<Engine*>(data);
    PluginSocket * ctxPluginSocket = PluginSocket::getInstance();
    InitSocket();//create a socket
    startNewThread(threadRecvConnect,ctxPluginSocket);
    startNewThread(threadRecvMsg,ctxPluginSocket);
}

void *PluginSocket::Run(void *arg)
{
    void *psMessage = NULL;
    LOGGER_DBG("PluginSocket::Run");
    PluginSocket *ctxPluginSocket = static_cast<PluginSocket *>(arg);

    while(1)
    {
        //waiting..............
        //lock
        pthread_mutex_lock(ctxPluginSocket->getMutexPtr());
        while(ctxPluginSocket->msgListEmpty())
        {
            LOGGER_DBG(">>>thread sleepping, PluginSocket::Run");
            //wait  wake up
            pthread_cond_wait(ctxPluginSocket->getCondPtr(), ctxPluginSocket->getMutexPtr());
            LOGGER_DBG(">>>thread is waked up, PluginSocket::Run");
        }
        psMessage = ctxPluginSocket->getMsgFromListFront();
        //unlock
        pthread_mutex_unlock(ctxPluginSocket->getMutexPtr());
        ctxPluginSocket->handleMsg(psMessage,arg);
    }
    return NULL;
}

int PluginSocket::InitSocket()
{
    int ret;
    struct sockaddr_un srvAddr;
    pthread_t id;

    LOGGER_DBG("PluginSocket::InitSocket start");
    listenFd=socket(PF_UNIX,SOCK_STREAM,0);
    if(listenFd<0)
    {
        LOGGER_ERR("fail, cannot create communication socket");
        return -1;
    }

    //set PluginSocket addr_param
    srvAddr.sun_family=AF_UNIX;
    strncpy(srvAddr.sun_path,SOCKET_PATH,sizeof(srvAddr.sun_path)-1);
    unlink(SOCKET_PATH);
    //bind sockfd & addr
    ret=bind(listenFd,(struct sockaddr*)&srvAddr,sizeof(srvAddr));
    if(ret==-1)
    {
        LOGGER_ERR("cannot bind server socket");
        close(listenFd);
        unlink(SOCKET_PATH);
        return -1;
    }
    //listen sockfd
    ret=listen(listenFd,CONNECT_CLIENT_MAX);
    if(ret==-1)
    {
        LOGGER_ERR("cannot listen the client connect request");
        close(listenFd);
        unlink(SOCKET_PATH);
        return -1;
    }
    LOGGER_DBG("init a socket, fd= %d",listenFd);
    LOGGER_DBG("Server::InitSocket end");
    return 0;
}

void * PluginSocket::threadRecvConnect(void *arg)
{
    struct sockaddr_un cltAddr;
    int comFd;//accept a client
    socklen_t cltAddrLen=sizeof(cltAddr);
    LOGGER_DBG("PluginSocket::threadRecvConnect start");
    PluginSocket *ctxPluginSocket = static_cast<PluginSocket *>(arg);
    while(1)
    {
        LOGGER_DBG("wait for client......");
        comFd = accept(ctxPluginSocket->listenFd,(struct sockaddr*)&cltAddr,&cltAddrLen);
        if(comFd>0){
            LOGGER_DBG("have a client connect now,comFd= %d",comFd);
            //lock
            pthread_mutex_lock(&(ctxPluginSocket->mutexFdList));
            ctxPluginSocket->mFdList.push_back (comFd);
            //unlock
            pthread_mutex_unlock(&(ctxPluginSocket->mutexFdList));
            //create thread for this client(comfd),using recv msg
        }
    }
    LOGGER_DBG("PluginSocket::threadRecvConnect end");
    return NULL;
}
void * PluginSocket::threadRecvMsg(void *arg)
{
    cJSON *root;
    std::list<int>::iterator it;
    int num;
    char JsonMsg[SOCKET_MSG_LENGTH];
    if(arg==NULL)
    {
        return NULL;
    }
    PluginSocket *ctxPluginSocket = static_cast<PluginSocket *>(arg);
    LOGGER_DBG("PluginSocket::threadRecvMsg start");
    while(1)
    {
        memset(JsonMsg,0,SOCKET_MSG_LENGTH);
        it =ctxPluginSocket->mFdList.begin();
        for (; it != ctxPluginSocket->mFdList.end(); it++)
        {
            num=read(*it,JsonMsg,sizeof(JsonMsg));
            if(num>0){
                LOGGER_DBG("<<<RECV_msg=",JsonMsg);
                std::string temp=JsonMsg;
                if(temp.find("\"origin\":")==std::string::npos)
                {
                    LOGGER_ERR("invalid msg,continue next");
                    continue;
                }
                root=cJSON_Parse(JsonMsg);
                if(root==NULL)
                {
                    continue;
                }
                if(temp.find("\"bind\":")!=std::string::npos)
                {
                    int bind = cJSON_GetObjectItem(root,MSG_KEY_BIND)->valueint;
                    std::string origin = cJSON_GetObjectItem(root,MSG_KEY_ORIGIN)->valuestring;
                    if(bind==1)
                    {
                        //client reveal oneself, server bind <fd> and <clientName>
                        ctxPluginSocket->mFdMap[origin] = *it;
                        LOGGER_DBG("bind <origin=%s,fd=%d>",origin.c_str(),ctxPluginSocket->mFdMap[origin]);
                    }
                    continue;
                }
                else
                {
                    //normal messages
                    std::map<std::string, Base*>::iterator it_pif;
                    std::string destination = cJSON_GetObjectItem(root,MSG_KEY_DESTINATION)->valuestring;
                    it_pif = ctxPluginSocket->pluginIFList.find(destination);
                    if(it_pif == ctxPluginSocket->pluginIFList.end())
                    {
                        LOGGER_ERR("have no this module,according to <destination>=%s",destination.c_str());
                        continue ;
                    }
                    Message *ctxMsg = new Message();
                    ctxMsg->origin = cJSON_GetObjectItem(root,MSG_KEY_ORIGIN)->valuestring;
                    ctxMsg->destination = destination;
                    ctxMsg->opertation_id = cJSON_GetObjectItem(root,MSG_KEY_OPID)->valueint;
                    ctxMsg->dataListVoid.push_back(it_pif->second);
                    ctxMsg->dataListString.push_back(JsonMsg);
                    LOGGER_DBG("add msg to list of <engine>");
                    ctxPluginSocket->sendMessage(ctxMsg,ctxPluginSocket);
                }
            }
        }
    }
    close(ctxPluginSocket->listenFd);
    LOGGER_DBG("PluginSocket::threadRecvMsg end");
    return NULL;
}

void PluginSocket::handleMsg(void *msg,void *data)
{
    LOGGER_DBG("PluginSocket::handleMsg");
    if(NULL == msg ||data == NULL)
    {
        LOGGER_ERR("Invalid message,msg or data is NULL");
        return;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    if(E_SOCKET_FUNCTION_MAX_NUMBER <= ctxMsg->opertation_id)
    {
        LOGGER_ERR("Invalid message,have no opertation");
        delete ctxMsg;
        return;
    }
    PluginSocket *ctxPluginSocket = static_cast<PluginSocket *>(data);
    (ctxPluginSocket->*socketServiceAPI[ctxMsg->opertation_id])(msg, data);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void PluginSocket::startNewThread(void *(threadFunc)(void *),void *data)
{
    //create a thread, recv connect from client (in fact:add msg to list of the <engine>, engine create this thread)
    if(threadFunc==NULL||data==NULL)
    {
        return;
    }
    PluginSocket *ctxPluginSocket = static_cast<PluginSocket *>(data);
    Message *ctxMsg = new Message();
    ctxMsg->origin = getClassName();
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = E_ENGINE_FUNCTION_START_NEW_THREAD;
    ctxMsg->dataListVoid.push_back(ctxPluginSocket);
    ctxMsg->funcList.push_back(threadFunc);
    LOGGER_DBG("add msg to list of engine,make <engine> to create a thread, be used for accepting the connection");
    sendMessage(ctxMsg,ctxPluginSocket);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void PluginSocket::sendMessage(void *msg,void *data)
{
    if(msg==NULL||data==NULL)
    {
        return;
    }
    PluginSocket *ctxPluginSocket = static_cast<PluginSocket *>(data);
    //lock
    pthread_mutex_lock(ctxPluginSocket->ctxEngine->getMutexPtr());
    ctxPluginSocket->ctxEngine->addMsgToListBack(msg);
    //wake up Engine::Run
    pthread_cond_signal(ctxPluginSocket->ctxEngine->getCondPtr());
    //unlock
    pthread_mutex_unlock(ctxPluginSocket->ctxEngine->getMutexPtr());
}
