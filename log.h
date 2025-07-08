#ifndef LOG
#define LOG

FILE* get_file_pointer(char* home_path, char* previous, char* mode);
void saveCommand(char* command , char* home_path, char* previous);
int get_commands(char** commands, char* home_path, char* previous);
void handle_log(char* command, char* home_path, char* previous);

#endif