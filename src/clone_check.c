#ifndef _CLONE_CHECK_C
#define _CLONE_CHECK_C

#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "listentry.h"
#include "list.h"
#include "malloc.h"
#include "conver.h"
#include <unistd.h>
#include "readir.h"
#include "compare.h"
#include "json.h"
#define N 1000

#define true  1
#define false 0 

int filt_comment = 0;      /* user config comment should compare*/
int filt_bracket = 0;      /* user config bracket should compare in different lines*/
int jason_create = 0;      /* flag show json file should be create*/
char *json_name = NULL;    /* json file name*/
char *compare_path = NULL; /* user config compare file path */
int line_compare = 5;      /* use config how many lines should consider as duplacate */

/*****************************************************************************
 Prototype    : cc_malloc
 Description  : malloc and memset memory
 Input        : int size  
 Output       : None
 Return Value : void
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/9
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
void* cc_malloc(int size){
    void  *pv = NULL;
    pv = malloc(size);
    if (pv != NULL) {
        memset(pv, 0, size);
    }
    return pv;
}

/*****************************************************************************
 Prototype    : file_reader
 Description  : read compare file 
 Input        : char *filename                   
                struct entry_info **pp_headlist  
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/9
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int file_reader(char *filename, struct entry_info **pp_headlist) {
    FILE *fp;
    char str[N+1];
    struct entry_info *pst_entry = NULL;
    struct entry_info *pst_add = NULL;
    struct entry_info *p_head = NULL;
    char *ptr_line;
    int line_count = 0;
    int line_commnet = 0;
    char *pst = NULL;

    if((fp=fopen(filename,"rt")) == NULL){
        printf("Cannot open file %s.\n", filename);
        return -1;
    }

    p_head = (struct entry_info *)cc_malloc(sizeof(struct entry_info));
    if (!p_head) {
        printf("no enough memory");
        return -1;
    }
    p_head->line_number = 0;
    INIT_LIST_HEAD(&p_head->st_list);
    *pp_headlist = p_head;

    while(fgets(str, N, fp) != NULL){
        line_count++;
        ptr_line = (char *)cc_malloc(strlen(str) + 1);
        if (!ptr_line) {
            printf("no enough memory");
            return -1;
        }

        memcpy(ptr_line, str, strlen(str));
        pst = conver_line(ptr_line, filt_comment, &line_commnet);
        if (pst == NULL || strlen(pst) == 0) {
            free(pst);
            continue;
        }

        pst_entry = (struct entry_info *)cc_malloc(sizeof(struct entry_info));
        if (!pst_entry) {
            printf("malloc memory %d failed", sizeof(struct entry_info));
            return -1;
        }
        INIT_LIST_HEAD(&pst_entry->st_list);
        list_add_tail (&pst_entry->st_list, &p_head->st_list);
        pst_entry->line_number = line_count;
        pst_entry->pc_detail = pst;

        if (filt_bracket == 1) {
            if (strlen(pst) > 2) {
                if( pst[strlen(pst) - 2] == '{' || 
                    pst[strlen(pst) - 2] == '}'){
                        pst_add = (struct entry_info *)cc_malloc(sizeof(struct entry_info));
                        if (pst_add == NULL) {
                            printf("malloc entry_info %d failed", sizeof(struct entry_info));
                            return -1;
                        }
                        ptr_line = (char *)cc_malloc(2);
                        if (ptr_line == NULL) {
                            printf("malloc size %d failed", 2);
                            return -1;
                        }
                        memcpy(ptr_line, (pst + (strlen(pst) - 2)), 2);
                        memcpy(pst + (strlen(pst)-2), pst + (strlen(pst)-1), 2);
                        INIT_LIST_HEAD(&pst_add->st_list);
                        list_add_tail (&pst_add->st_list, &p_head->st_list);
                        pst_add->line_number = line_count;
                        pst_add->pc_detail = ptr_line;
                    }
              }
        }
        
    }

    fclose(fp);
    return 0;
}

void clear_entry(struct entry_info *phead)
{
    struct entry_info * p_entry = NULL;
    list_for_each_entry(p_entry, &phead->st_list, st_list) {
        free(p_entry->pc_detail);
        free(p_entry);
    }
    free(phead);
}

/*****************************************************************************
 Prototype    : arg_parse
 Description  : parse arg 
 Input        : int argc     
                char **argv  
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/9
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int arg_parse(int argc, char **argv)
{
   int ch;
   opterr = 0;

   if (argc < 2) {
       printf("please input correct parameter");
       return 0;
   }
   
   compare_path = argv[1];
   
   while((ch = getopt(argc-1, argv+1, "n:j:bch")) != -1)
   switch(ch)
   {
      case 'n':
         line_compare = atoi(optarg);
         break;
      case 'j':
         json_name = optarg;
         jason_create = 1;
         break;
      case 'b':
          filt_bracket = 1;
         break;
      case 'c':
         filt_comment = 1;
         break;
      default:
         printf("unknow option :%c\n", ch);
         return -1;
   }

   return 1;
}

/*****************************************************************************
 Prototype    : clone_check
 Description  : clone code chck 
 Input        : int argc       
                char * argv[]  
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int clone_check(int argc, char * argv[]){
    struct file_entry *pst_files = NULL;
    struct compare_entry compare;
    int ret = -1;
    int file_count = 0;
    int i = 0, j = 0;
    int size = 0;

    memset(&compare, 0, sizeof(compare));

    /* argument parse */
    if ((ret = arg_parse (argc, argv)) != 1) {
        printf("please input correct paramet");
        return 0 ;
    }

    /* read file count in path */
    ret = read_filecount(compare_path, &file_count);
    if (ret == -1) {
        return 0;
    }

    if (file_count < 2) {
        printf("no enough file info %d", file_count);
        return 0;
    }

    size = sizeof(struct file_entry)*file_count;
    pst_files = (struct file_entry *)cc_malloc(size);
    if (pst_files == NULL) {
        printf("malloc memory failed %d", size);
        goto release;
    }

    /* get all file name in specifical path */
    ret = get_filename(pst_files, compare_path, 0);
    if (ret == -1) {
        goto release;
    }

    /* read file and formate line info as config */
    for (i = 0; i < file_count; i++) {
        ret = file_reader((pst_files+i)->file_info, &(pst_files+i)->p_entry);
        if (ret == -1) {
            printf("read file %s failed", (pst_files+i)->file_info);
            goto release;
        }
    }
    
    /* do compare between each other */
    for (i = 0; i < file_count; i++) {
        for(j = i+1; j < file_count; j++){
            do_compare((pst_files + i), (pst_files + j), &compare, line_compare);
        }
    }

    /* do compare in self */
    for (i = 0; i < file_count; i++) {
        do_self_compare((pst_files + i), &compare, line_compare);
    }

    if (jason_create == 1){
        /* output the result to json fle */
        write_to_json(json_name, compare);
    } else {
        printf("totalDuplicate: %d", compare.totalDuplicate);
    }
    
release:
    for (i = 0; i < file_count; i++) {
        if((pst_files + i) != NULL) {
            //printf("free %x \n", pst_files + i);
            clear_entry((pst_files+i)->p_entry);
        }
    }

    release_compare_entry(&compare);

    return ret;
}

#endif

