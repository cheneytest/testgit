#ifndef _CONVER_H
#define _CONVER_H

void replace_space(char *origin);
char * file_oneline_comment(char *origin);
char* find_multilines_comment(char *origin, int * comment_flag);
char * conver_line(char *p_line, int file_comment, int *p_flag);

#endif

