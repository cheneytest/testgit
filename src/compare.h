
int do_compare(struct file_entry * pst_file1, struct file_entry * pst_file2, struct compare_entry *p_sesults, int compare_count);
int do_self_compare(struct file_entry * pst_file, struct compare_entry *p_sesults, int compare_count);
void release_compare_entry(struct compare_entry *p_compare);