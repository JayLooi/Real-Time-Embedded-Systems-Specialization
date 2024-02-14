#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

#define RTES_SYSLOG(priority, COURSE_NUM, ASSGNMT_NUM, log_fmt, ...)  \
    syslog(priority, "[COURSE:%d][ASSIGNMENT:%d] "log_fmt, COURSE_NUM, ASSGNMT_NUM, ##__VA_ARGS__)

#define COURSE_NUM      (1)
#define ASSGNMT_NUM     (1)


void *helloWorldThread(void *threadp)
{
    RTES_SYSLOG(LOG_INFO, COURSE_NUM, ASSGNMT_NUM, "Hello World from Thread! ");
    return threadp;
}


int main (int argc, char *argv[])
{
    // struct utsname kernel_info = { 0 };
    // int rc = 0;
    pthread_t thread_id;
    openlog(NULL, 0, LOG_USER);
    
    /* rc = uname(&kernel_info);
    if (rc != 0)
    {
        RTES_SYSLOG(LOG_ERR, COURSE_NUM, ASSGNMT_NUM, "Failed to read kernel info: %s", strerror(errno));
    }
    else
    {
        RTES_SYSLOG(LOG_INFO, COURSE_NUM, ASSGNMT_NUM, "")
    } */

    FILE *pipe = popen("uname -a", "r");

    if (pipe == NULL)
    {
        RTES_SYSLOG(LOG_ERR, COURSE_NUM, ASSGNMT_NUM, "Failed to open a pipe: %s", strerror(errno));
    }
    else
    {
        char kernel_info[256] = { 0 };
        if (fgets(kernel_info, sizeof(kernel_info), pipe) != NULL)
        {
            RTES_SYSLOG(LOG_INFO, COURSE_NUM, ASSGNMT_NUM, "%s", kernel_info);
        }
        else
        {
            RTES_SYSLOG(LOG_ERR, COURSE_NUM, ASSGNMT_NUM, "Failed in reading kernel info from pipe: %s", strerror(errno));
        }
    }

    RTES_SYSLOG(LOG_INFO, COURSE_NUM, ASSGNMT_NUM, "Hello World from Main! ");
    
    pthread_create(&thread_id, NULL, helloWorldThread, NULL);
    
    pthread_join(thread_id, NULL);

    closelog();

    return 0;
}

