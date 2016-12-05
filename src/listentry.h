#ifndef _LLISTENTRY_H
#define _LLISTENTRY_H

#include "list.h"
struct entry_info {
    int line_number;
    int if_comment;
    char *pc_detail;
    struct list_head st_list;
};

struct file_entry {
    char file_info[100];
    struct entry_info *p_entry;
};

struct duplicate_detail {
    int start_line1;
    int start_line2;
    int line_count1;
    int line_count2;
    int total;
    struct list_head detail_list;
};

struct check_result{
    int lines;
    char fileName1[100];
    char fileName2[100];
    struct duplicate_detail *detail;
    struct list_head result_list;
};

struct compare_entry {
    int totalDuplicate;
    struct check_result *result;
};

#endif
