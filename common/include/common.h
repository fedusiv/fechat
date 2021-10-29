#ifndef __COMMON_H__
#define __COMMON_H__

typedef enum APP_STATE_E
{
    STATE_LOGIN,
    STATE_CHAT
}APP_STATE;


APP_STATE get_application_state();
void error_handling(const char * error_msg);

#endif // __COMMON_H__
