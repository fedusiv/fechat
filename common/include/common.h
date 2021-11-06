#ifndef __COMMON_H__
#define __COMMON_H__

#define APP_VERSION "061121"


typedef enum APP_STATE_E
{
    STATE_LOGIN,
    STATE_CHAT
}APP_STATE;


APP_STATE get_application_state();
void error_handling(const char * error_msg);
void clear_screen();    // clear screen

#endif // __COMMON_H__
