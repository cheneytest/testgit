#ifndef _CLONE_CHECK_H
#define _CLONE_CHECK_H

int file_reader(char *filename, struct entry_info **pp_headlist);
void clear_entry(struct entry_info *phead);
#endif
