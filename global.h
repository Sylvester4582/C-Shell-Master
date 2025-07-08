#ifndef GLOBAL
#define GLOBAL

char *trim(char *s);
void set_global_flags(int dur, char* firstword);
int get_global_dur();
char* get_global_firstword();
void remove_consecutive_spaces(char *str);

#endif