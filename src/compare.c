#ifndef _COMPARE_C
#define _COMPARE_C

#include<stdio.h>
#include <string.h>
#include "listentry.h"
#include "list.h"
#include "malloc.h"
#include "conver.h"
#include <unistd.h>
#include "readir.h"

/*****************************************************************************
 Prototype    : add_detail_list
 Description  : add duplicate to list
 Input        : struct check_result **pp_result      
                struct duplicate_detail *dup_detail  
 Output       : None
 Return Value : void
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/8
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
void add_detail_list(struct check_result **pp_result, struct duplicate_detail *dup_detail){

    struct duplicate_detail *detail = NULL;
    struct check_result *result = *pp_result;
    if (result == NULL) {
        result = (struct check_result *)malloc(sizeof(struct check_result));
        if (result == NULL) {
            printf("no enough memory");
            return;
        }
        memset(result, 0, sizeof(struct check_result));
        result->detail = NULL;
    }

    result->lines += dup_detail->total;
    detail = (struct duplicate_detail *)malloc(sizeof(struct duplicate_detail));
    if (detail == NULL) {
        printf("no enough memory");
        return;
    }

    memcpy(detail, dup_detail, sizeof(struct duplicate_detail));

    if (result->detail == NULL) {
        result->detail = detail;
        INIT_LIST_HEAD(&result->detail->detail_list);
    } else {
        list_add_tail (&detail->detail_list, &result->detail->detail_list);
    }

    *pp_result = result;
    return;
}

/*****************************************************************************
 Prototype    : check_samelines
 Description  : compare next lines if same
 Input        : struct entry_info **ppst_file1       
                struct entry_info **ppst_file2       
                struct duplicate_detail *dup_detail  
                int record                           
                int samefile                         
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/8
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int check_samelines(struct entry_info **ppst_file1, 
                    struct entry_info **ppst_file2, 
                    struct duplicate_detail *dup_detail,
                    int record,
                    int samefile) {
    struct entry_info *file1 = *ppst_file1;
    struct entry_info *file2 = *ppst_file2;
    struct entry_info *next1 = NULL;
    struct entry_info *next2 = NULL;
    int count = 1;
    
    dup_detail->start_line1 = file1->line_number;
    dup_detail->start_line2 = file2->line_number;

    while (1) {
        next1 = list_entry((file1)->st_list.next, typeof(*file1), st_list);
        next2 = list_entry((file2)->st_list.next, typeof(*file2), st_list);
        
        if ((next1->line_number <= 0) || (next2->line_number <= 0)) {
            //printf("break1 %d %d %d %d \n", next1->line_number, file1->line_number, next2->line_number, file2->line_number);
            break;
        }

        if (samefile == 0) {
            if (strcmp(next1->pc_detail, next2->pc_detail) != 0) {
                //printf("%saaa%sbbb\n",next1->pc_detail, next2->pc_detail);
                //printf("break2 1 \n");
                break;
            }
        } else {
            if (strcmp(next1->pc_detail, next2->pc_detail) != 0 || 
                (next1->line_number == next2->line_number)){
                //printf("break2 2\n");
                break;
            }
        }

        count ++;
        file1 = next1;
        file2 = next2;
    }

    dup_detail->line_count1 = file1->line_number - dup_detail->start_line1 + 1;
    dup_detail->line_count2 = file2->line_number - dup_detail->start_line2 + 1;
    dup_detail->total = dup_detail->line_count1 + dup_detail->line_count2;
    if (count >= record) {
    /*    printf("start line1 %d start line2 %d; count1 %d count2 %d total %d \n", 
          dup_detail->start_line1, dup_detail->start_line2, dup_detail->line_count1,
          dup_detail->line_count2, dup_detail->total);*/
        *ppst_file1 = next1;
    }

    *ppst_file2 = next2;

    return count;
}

/*****************************************************************************
 Prototype    : do_compare
 Description  : compare two file duplicate and record the duplicate info
                to compare_entry 
 Input        : struct file_entry * pst_file1    
                struct file_entry * pst_file2    
                struct compare_entry *p_compare  
                int compare_count                
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/8
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int do_compare(struct file_entry * pst_file1, 
               struct file_entry * pst_file2, 
               struct compare_entry *p_compare, 
               int compare_count)
{
    int line_count = 0;
    char *line_info1 = NULL;
    char *line_info2 = NULL;
    struct entry_info *file_head1 = NULL;
    struct entry_info *file_head2 = NULL;
    struct entry_info *file_entry1 = NULL;
    struct entry_info *file_entry2 = NULL;
    file_head1 = pst_file1->p_entry;
    file_head2 = pst_file2->p_entry;
    struct duplicate_detail dup_detail;
    struct check_result *result = NULL;

    list_for_each_entry(file_entry1, &file_head1->st_list, st_list) {
        line_info1 = file_entry1->pc_detail;
        list_for_each_entry(file_entry2, &file_head2->st_list, st_list) {
            line_info2 = file_entry2->pc_detail;
            if (strcmp(line_info1,line_info2) == 0){
                memset(&dup_detail, 0, sizeof(dup_detail));
                line_count = check_samelines(&file_entry1, &file_entry2, &dup_detail, compare_count, 0);

                if (line_count >= compare_count) {
                    add_detail_list(&result, &dup_detail);
                }
                if (file_entry2 == file_head2)
                    break;
            } 
        }

        if (file_entry1 == file_head1)
            break;
    }

    if (result != NULL) {
        memcpy(result->fileName1, pst_file1->file_info, 100); 
        memcpy(result->fileName2, pst_file2->file_info, 100); 
        if (p_compare->result == NULL) {
            p_compare->result = result;
            INIT_LIST_HEAD(&p_compare->result->result_list);
        } else {
            list_add_tail(&result->result_list, &p_compare->result->result_list);
        }

        p_compare->totalDuplicate +=result->lines;
    }
    
    return 0;
}

/*****************************************************************************
 Prototype    : do_self_compare
 Description  : compare one file code duplicate info
 Input        : struct file_entry * pst_file     
                struct compare_entry *p_compare  
                int compare_count                
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/8
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int do_self_compare(struct file_entry * pst_file, struct compare_entry *p_compare, int compare_count){
    int line_count = 0;
    struct entry_info *file_head1 = NULL;
    struct entry_info *file_head2 = NULL;
    struct entry_info *file_entry1 = NULL;
    struct entry_info *file_entry2 = NULL;
    file_head1 = pst_file->p_entry;
    file_head2 = pst_file->p_entry;
    char *line_info1 = NULL;
    char *line_info2 = NULL;
    struct duplicate_detail dup_detail;
    struct check_result *result = NULL;

    //printf("do_self_compare \n");

    list_for_each_entry(file_entry1, &file_head1->st_list, st_list) {
        line_info1 = file_entry1->pc_detail;
        list_for_each_entry(file_entry2, &file_head2->st_list, st_list) {
            if (file_entry1->line_number >= file_entry2->line_number)
                continue;
            line_info2 = file_entry2->pc_detail;
            if ((strcmp(line_info1,line_info2) == 0) &&
                file_entry1->line_number != file_entry2->line_number){
                //printf("line1 %d, line2 %d \n", file_entry1->line_number, file_entry2->line_number);
                memset(&dup_detail, 0, sizeof(dup_detail));
                line_count = check_samelines(&file_entry1, &file_entry2, &dup_detail, compare_count, 1);

                if (line_count >= compare_count) {
                    add_detail_list(&result, &dup_detail);
                }
                if (file_entry2 == file_head2)
                    break;
            } 
        }

        if (file_entry1 == file_head1)
            break;
    }

    if (result != NULL) {
        memcpy(result->fileName1, pst_file->file_info, 100); 
        memcpy(result->fileName2, pst_file->file_info, 100); 
        if (p_compare->result == NULL) {
            p_compare->result = result;
            INIT_LIST_HEAD(&p_compare->result->result_list);
        } else {
            list_add_tail(&result->result_list, &p_compare->result->result_list);
        }

        p_compare->totalDuplicate +=result->lines;
    }
    
    return 0;
}

void release_compare_entry(struct compare_entry *p_compare)
{
    struct check_result * result = NULL;
    struct duplicate_detail *detail = NULL;
    if (p_compare->result == NULL) { 
        return;
    }

    list_for_each_entry(result, &p_compare->result->result_list, result_list) {
        list_for_each_entry(detail, &result->detail->detail_list, detail_list) {
            free(detail);
        }
        free(result->detail);
        free(result);
    }
    result = p_compare->result;
    list_for_each_entry(detail, &result->detail->detail_list, detail_list) {
        free(detail);
    }
    free(p_compare->result);
}

#endif
