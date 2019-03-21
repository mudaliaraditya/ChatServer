#pragma once

enum class CMESSAGE_CODE_ACTIONS : long 
{
    MESSAGE_CODE_ACTIONS_INVALID                   = 1000,
    MESSAGE_CODE_ACTIONS_REGISTER                  = 1001,//this is used to send from client to server 
    MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE         = 1002,//this is response from server to client that it has been registered
    MESSAGE_CODE_ACTIONS_REGISTER_TARGET           = 1003,//this is from client to server to register targer
    MESSAGE_CODE_ACTIONS_REGISTER_TARGET_RESPONSE  = 1004,//this is form server to indicate that target has been registed for client
    MESSAGE_CODE_ACTIONS_CHAT                      = 1005,//this goes from client to server
    MESSAGE_CODE_ACTIONS_CHAT_RESPONSE             = 1006,//this is from server to client that message has been receved
    MESSAGE_CODE_ACTIONS_CHAT_MESSAGE              = 1007,
            
    MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY          = 1009,
    MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY_RES      = 1010,
    MESSAGE_CODE_ACTIONS_CHAT_MSG_SERV_TO_CLI      = 1011,
	MESSAGE_CODE_ACTIONS_DUMMY                     = 3000,
};


enum class CCOMMAND_TYPE : short 
{
   CCOMMAND_TYPE_INVALID       = 0,
   CCOMMAND_TYPE_COMMAND       = 1,
   CCOMMAND_TYPE_REQUEST       = 2,
   CCOMMAND_TYPE_RESPONSE      = 3,
   CCOMMAND_TYPE_DELIVERY      = 4,
   CCOMMAND_TYPE_DELIVERY_RES  = 5, 
   CCOMMAND_TYPE_DELIVERY_CONF = 6,
};


enum class CEToFromServer : long
{
   CEToFromServer_INVALID = 0,
   CEToFromServer_CLNT_TO_SERV = 10011,
   CEToFromServer_SERV_TO_CLNT = 10012,
 //  CEToFromServer_INVALID = 0;
};
