#include<stdio.h>
#include<string.h>
#include "malloc.h"

int find_comment_slash(char *origin, int po);
/*****************************************************************************
 Prototype    : substr
 Description  : get substr info from string
 Input        : char *origin        
                unsigned int start  
                unsigned int end    
 Output       : None
 Return Value : char
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/8
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
char * substr(char *origin, int start, int end) {
    int len = strlen(origin);
    if (start > len) 
        return NULL;
    if (end > len)
        end = len;
    memcpy(origin, origin + start, end - start + 1);
    origin[end-start + 1] = '\0';
    return origin;
}

/*****************************************************************************
 Prototype    : replace_space
 Description  : delete string space
 Input        : char *origin  
 Output       : None
 Return Value : void
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
void replace_space(char *origin) {
  int len,i,j;
  len = strlen(origin);
  j=0;
  
	for(i=0;i<len;i++)
	{  
		if((origin[i]!=' ') && (origin[i] != 9))
		{
			origin[j]=origin[i];
			j++;
		}
	}
  if (j>=1) {
    if (origin[j-1] != 10)
        origin[j] = 10; 
    else 
        origin[j] = '\0';
  }
}

/*****************************************************************************
 Prototype    : find_subString
 Description  : cut string
 Input        : char *origin  
                char *sub     
                int position  
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int find_subString(char *origin, char *sub, int position) {
    char * p1 = NULL;
    char * p2 = NULL;
    char * p3 = NULL;
    unsigned int i = 0, j;
    int flag = 0;
    
    p1 = origin + position;
    p2 = sub;
    for(i = 0; i<strlen(origin); i++)
    {
        if(*p1 == *p2)
          {
              p3 = p1;
              for(j = 0;j<strlen(sub);j++)
              {
                if(*p3 == *p2)
                {
                  p3++;p2++;
                } 
                else
                  break;
              }
              p2 = sub;
              if(j == strlen(sub))
              {
                flag = 1;
                
                break;
              }
          }
        p1++; 
    }
    
    if (flag == 1) {
        return i + position;
    }
    else {
        return -1;
    }
}

/*****************************************************************************
 Prototype    : file_oneline_comment
 Description  : find one line comments and replace them
 Input        : char *origin  
 Output       : None
 Return Value : char
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
char * file_oneline_comment(char *origin){
    int po = find_comment_slash(origin, 0);
    if (po == 0) {
        origin[0] = '\0';
        return origin;
    }
    return substr(origin, 0, po);
}

/*****************************************************************************
 Prototype    : find_comment_slash
 Description  : find one line comment start place
 Input        : char *origin  
                int po        
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int find_comment_slash(char *origin, int po) {
    char comment[] = "//";
    char string[] = "\"";
    int slash_position = 0;
    int squo_position = 0;
    int equo_position = 0;
    int end = strlen(origin);

    slash_position = find_subString(origin, comment, po);
    if (slash_position > 0) {
        squo_position = find_subString(origin, string, po);
        if (slash_position < squo_position) {
            end = slash_position;
        }
        else {
            equo_position = find_subString(origin, string, squo_position + 1);
            if (equo_position <= slash_position) {
                end = slash_position;
            } else {
                end = find_comment_slash(origin, equo_position +1 );
            }
        }
    } else if (slash_position == 0) {
        end = 0;
    }
     
    return end;
}

/*****************************************************************************
 Prototype    : find_multilines_comment
 Description  : find multil lines comment and replace 
 Input        : char *origin        
                int * comment_flag  
 Output       : None
 Return Value : char
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
char* find_multilines_comment(char *origin, int * comment_flag) {
    char *find = NULL;
    char start[] = "/*";
    char end[] = "*/";
    char *pc = NULL;
    int pos = -1;
    int epos = -1;
    int flag = 0;
    int m_len = 0;
    if (*comment_flag == 1)
        find = end;
    else if (*comment_flag == 0)
        find = start;
    pos = find_subString(origin, find, 0);
    if (pos >=0)
        flag = 1;
        
    if (*comment_flag == 1) {
        if (flag == 1) {
            *comment_flag = 0;
            return substr(origin, pos+2, strlen(origin));
        } else 
            return NULL;
    }
    else if (*comment_flag == 0) {
        if (flag == 1) {
            epos = find_subString(origin, end, 0);
            if (epos >=0) {
                *comment_flag = 0;
                m_len = strlen(origin) -  epos + pos - 2;
                pc = (char *)malloc(m_len + 1);
                memcpy(pc, origin, pos);
                memcpy(pc + pos, substr(origin, epos + 2, strlen(origin)), strlen(origin) - epos - 2);
                memcpy(origin, pc, m_len);
                origin[m_len] = '\0';
                free(pc);
            } else {
                *comment_flag = 1;
                if (pos == 0) {
                    origin[0] = '\0';
                    return origin;
                }
                  
                return substr(origin, 0, pos);
            }
        }
    }    
        
    return origin;
}

/*****************************************************************************
 Prototype    : conver_line
 Description  : conver string formate
 Input        : char *p_line      
                int file_comment  
                int *p_flag       
 Output       : None
 Return Value : char
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
char * conver_line(char *p_line, int file_comment, int *p_flag){
    char *p1 = NULL;
    p1 = p_line;
    replace_space(p_line);
    if (file_comment == 1) {
        p1 = file_oneline_comment(p_line);
        p1 = find_multilines_comment(p1, p_flag);
    }
    if (strlen(p1) == 1 && p1[0] == 10) {
        return NULL;
    }
    return p1;
}
