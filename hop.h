#ifndef HOP
#define HOP

char* get_path(char* home_path, char* path, char* previous);
int change_directory(char* home_path, char* path, char* previous, bool print);
void hop(char* command , char* home_path , char* previous);

#endif