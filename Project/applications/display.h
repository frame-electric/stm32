#ifndef __DISPLAY_H
#define __DISPLAY_H

struct display_task_t
{
    void (*init)(void);
};
extern const struct display_task_t display_task; 
#endif
