#ifndef _IO_H
#define _IO_H

#define IO_BUFFER_SIZE 1024

bool prompt_s(char *prompt_str, char *dest);
int prompt_i(char *prompt_str);
bool prompt_b(char *prompt_str);
bool prompt_v(char *prompt_str, char **dest, int max_count, int *dest_count);
bool prompt_vi(char *prompt_str, int *dest, int max_count, int *dest_count);

#endif
