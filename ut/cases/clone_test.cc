#include <limits.h>
#include "gtest/gtest.h"
extern "C"
{
#include "../../src/list.h"
#include "../../src/listentry.h"
#include "../../src/Readir.h"
#include "../../src/compare.h"
#include "../../src/json.h"
#include "../../src/conver.h"
#include "../../src/check_clone.h"
}

TEST(readdir, read_filecount) {
  int count = 0;
  char path_ok[] = "../read_dir/";
  char path_nook[] = "noexit/";
  EXPECT_EQ(-1, read_filecount(path_nook, &count));
  EXPECT_EQ(0, read_filecount(path_ok, &count));
  EXPECT_EQ(2, count);
}

TEST(readdir, get_filename) {
  int size = 0;
  int file_count = 2;
  struct file_entry * pst_files = NULL;
  char path_ok[] = "../read_dir/";
  size = sizeof(struct file_entry)*file_count;
  pst_files = (struct file_entry *)malloc(size);
  EXPECT_EQ(file_count, get_filename(pst_files, path_ok, 0));
  EXPECT_EQ(0, strcmp(pst_files->file_info, "../read_dir/a.txt"));
  EXPECT_EQ(0, strcmp((pst_files+1)->file_info, "../read_dir/b.txt"));
  free(pst_files);
}


TEST(conver, replace_space) {
  char str[] = "    free(pst_files);\n";
  replace_space(str);
  EXPECT_EQ(0, strcmp(str, "free(pst_files);\n"));
}

TEST(conver, file_oneline_comment) {
  char str[] = "//    free(pst_files);";
  EXPECT_EQ(0, strcmp(file_oneline_comment(str), ""));
}

TEST(conver, find_multilines_comment) {
  int flage = 0;
  char str[] = "/*    free(pst_files);";
  char str1[] = "test */ aaa";
  EXPECT_EQ(0, strcmp(find_multilines_comment(str, &flage), ""));
  EXPECT_EQ(0, strcmp(find_multilines_comment(str1, &flage), " aaa"));
}

TEST(clone_check , read_compare) {
    char file1[] = "../read_dir/a.txt";
	char file2[] = "../read_dir/b.txt";
    struct entry_info *p_entry1 = NULL;
	struct entry_info *p_entry2 = NULL;
	EXPECT_EQ(0, file_reader(file1, &p_entry1));
	EXPECT_EQ(0, file_reader(file2, &p_entry2));
	clear_entry(p_entry1);
	clear_entry(p_entry2);
}
