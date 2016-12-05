/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.
******************************************************************************
  File Name     : readir.c
  Version       : Initial Draft
  Author        : chenxiaobo
  Created       : 2016/11/10
  Last Modified :
  Description   : 11
  Function List :
              get_filename
              read_filecount
  History       :
  1.Date        : 2016/11/10
    Author      : chenxiaobo
    Modification: Created file

 *----------------------------------------------*/
#ifndef _READIR_C
#define _READIR_C

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "string.h"
#include "listentry.h"

/*****************************************************************************
 Prototype    : read_filecount
 Description  : read file path file count
 Input        : char *path  
                int *count  
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/8
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int read_filecount(char *path, int *count) {
    DIR *dir;
    struct dirent *ptr;
    struct stat statbuf;
    char path_name[100];
    int len = 0;

    dir = opendir(path);
    if (dir == NULL)  {
        printf("open %s failed \n", path);
        return -1;
    }

    while((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_name == NULL) {
            continue;
        }
        if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        len = strlen(path);
        if (path[len-1] == '/'){
            snprintf(path_name, 100, "%s%s", path, ptr->d_name);
        } else {
            snprintf(path_name, 100, "%s/%s", path, ptr->d_name);
        }
        stat(path_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            read_filecount(path_name, count);
        }else {
            (*count)++;
        }
    }
    closedir(dir);

    return 0;
}

/*****************************************************************************
 Prototype    : get_filename
 Description  : read file name from file_path
 Input        : struct file_entry *pst_files  
                char * path                   
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/8
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int get_filename(struct file_entry *pst_files, char * path, int depath){
    DIR *dir;
    struct dirent *ptr;
    struct stat statbuf;
    struct file_entry * filelist = pst_files;
    int len = 0;
    int count = depath;
    int sub_count = 9;
    char path_name[100];
    dir = opendir(path);
    if (dir == NULL) {
        printf("open dir %s failed \n", path);
        return -1;
    }

    while((ptr = readdir(dir)) != NULL)
    {
        filelist = pst_files + count;
        //printf("count = %d", count);
        if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        len = strlen(path);
        if (path[len-1] == '/'){
            snprintf(path_name, 100, "%s%s", path, ptr->d_name);
        }
        else{
            snprintf(path_name, 100, "%s/%s", path, ptr->d_name);
        }

        stat(path_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            sub_count = get_filename(pst_files, path_name, count);
            count = sub_count;
        }else {
            count ++;
            memcpy(filelist->file_info, path_name, 100);
        }
    }

    closedir(dir);
    return count;
}

#endif
