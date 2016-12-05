#ifndef _JSON_C
#define _JSON_C

#include<stdio.h>
#include <string.h>
#include "listentry.h"
#include "list.h"
#include "malloc.h"
#include "conver.h"
#include <unistd.h>
#include "readir.h"

/*****************************************************************************
 Prototype    : print_detail_entry
 Description  : write details json
 Input        : FILE *fp                     
                struct check_result *result  
 Output       : None
 Return Value : void
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
void print_detail_entry(FILE *fp, struct check_result *result) {
    struct duplicate_detail *detail_entry = NULL;
    fprintf(fp, "    {\n");
    fprintf(fp, "      \"duplicate\": %d,\n", result->lines);
    fprintf(fp, "      \"fileName1\": \"%s\",\n", result->fileName1);
    fprintf(fp, "      \"fileName2\": \"%s\",\n", result->fileName2);
    fprintf(fp, "      \"details\": [\n");
    fprintf(fp, "        {\n");
    fprintf(fp, "          \"duplicate\": %d,\n", result->detail->total);
    fprintf(fp, "          \"startLine1\": %d,\n", result->detail->start_line1);
    fprintf(fp, "          \"lineNumber1\": %d,\n", result->detail->line_count1);
    fprintf(fp, "          \"startLine2\": %d,\n", result->detail->start_line2);
    fprintf(fp, "          \"lineNumber2\": %d\n", result->detail->line_count2);
    fprintf(fp, "        }");
    list_for_each_entry(detail_entry, &result->detail->detail_list, detail_list) {
          fputs(",\n", fp);
          fprintf(fp, "        {\n");
          fprintf(fp, "          \"duplicate\": %d,\n", detail_entry->total);
          fprintf(fp, "          \"startLine1\": %d,\n", detail_entry->start_line1);
          fprintf(fp, "          \"lineNumber1\": %d,\n", detail_entry->line_count1);
          fprintf(fp, "          \"startLine2\": %d,\n", detail_entry->start_line2);
          fprintf(fp, "          \"lineNumber2\": %d\n", detail_entry->line_count2);
          fprintf(fp, "        }\n");
        }
    fprintf(fp, "\n      ]\n");
    fprintf(fp, "    }\n");

    return;
}

/*****************************************************************************
 Prototype    : write_to_json
 Description  : write entry to json
 Input        : char* filename              
                struct compare_entry entry  
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/11/10
    Author       : chenxiaobo
    Modification : Created function

*****************************************************************************/
int write_to_json(char* filename, struct compare_entry entry) {
    struct check_result *result_entry = NULL;
    FILE *fp = NULL;
    if( (fp=fopen(filename,"wt")) == NULL ){
        printf("Cannot open file %s, press any key to exit!", filename);
        return -1;
    }

    fprintf(fp, "{\n");
    fprintf(fp, "  \"totalDuplicate\": %d,\n", entry.totalDuplicate);
    fprintf(fp, "  \"results\": [\n");
    print_detail_entry(fp, entry.result);
    list_for_each_entry(result_entry, &entry.result->result_list, result_list) {
        fputs(",", fp);
        print_detail_entry(fp, result_entry);
    }
    fprintf(fp, "  ]\n");
    fprintf(fp, "}\n");
    fclose(fp);

    return 0;
}

#endif
