#ifndef __COMM_PROTOCOL_H__
#define __COMM_PROTOCOL_H__

enum COMM_OPCODES_E
{
    COMM_OPCODE_LOGIN_REQ = 1,
    COMM_OPCODE_LOGIN_RES,
    COMM_OPCODE_KA_REQ,
    COMM_OPCODE_KA_RES,
}COMM_OPCODES;

typedef struct _COMM_DEFAULT_MSG_S
{
    unsigned int opcode;
    void * data;
    short crc;
}COMM_DEFAULT_MSG_S;

typedef struct _COMM_OPCODE_LOGIN_REQ_S
{
    unsigned char login[20];    // login string request
    unsigned char password[20];    // login string request
}COMM_OPCODE_LOGIN_REQ_S;

typedef struct _COMM_OPCODE_LOGIN_RES_S
{
    unsigned char result;
    unsigned char login[20];
}COMM_OPCODE_LOGIN_RES_S;

typedef struct _COMM_OPCODE_KA_REQ_S
{
    unsigned char empty;
}COMM_OPCODE_KA_REQ_S;

typedef struct _COMM_OPCODE_KA_RES_S
{
    unsigned char empty;
}COMM_OPCODE_KA_RES_S;


#endif // __COMM_PROTOCOL_H__
