#include "pluginSocket.h"

const PluginSocket::func PluginSocket::socketServiceAPI[] = {
    &PluginSocket::registIF,
    &PluginSocket::unRegistIF,
    &PluginSocket::sendMessageToClient,
};

//--------------------------------------------------------------------------
/**
 *  export functions
 */
//--------------------------------------------------------------------------
t_int PluginSocket::registIF(void *msg, void *data)
{

    if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }

    Message *ctxMsg = static_cast<Message *>(msg);

    PluginSocket *ctxPluginSocket = static_cast<PluginSocket *>(data);

    Base *ctxPluginIF = static_cast<Base *>(ctxMsg->dataListVoid[0]);

    ctxPluginSocket->pluginIFList[ctxPluginIF->getClassName()] = ctxPluginIF;

    delete ctxMsg;

    return E_OPERATION_ERROR_NONE;
}

t_int PluginSocket::unRegistIF(void *msg, void *data)
{

    if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }

    Message *ctxMsg = static_cast<Message *>(msg);

    PluginSocket *ctxPluginSocket = static_cast<PluginSocket *>(data);

    Base *ctxPluginIF = static_cast<Base *>(ctxMsg->dataListVoid[0]);

    ctxPluginSocket->pluginIFList.erase(ctxPluginIF->getClassName());

    delete ctxMsg;

    return E_OPERATION_ERROR_NONE;
}

t_int PluginSocket::sendMessageToClient(void *msg, void *data)
{
    if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);

    std::map<std::string , int >::iterator itClientFd;
    itClientFd = mFdMap.find(ctxMsg->receiver);
    LOGGER_DBG("PluginSocket::sendMessageToClient,fd=%d,receler=%s",itClientFd->second,ctxMsg->receiver.c_str());
    if (itClientFd!=mInstance->mFdMap.end())
    {
        LOGGER_DBG("<<<SEND_msg=%s",ctxMsg->dataListString[0].c_str());
        write(itClientFd->second,ctxMsg->dataListString[0].c_str() ,SOCKET_MSG_LENGTH);
        delete ctxMsg;
    }
    else
    {
        LOGGER_DBG("invalid message,have no receiver<fd>");
        delete ctxMsg;
    }
    return E_OPERATION_ERROR_NONE;
}
