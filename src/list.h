
#ifndef _LLIST_H
#define _LLIST_H

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

#define INIT_LIST_HEAD(head) do {			\
		(head)->next = (head)->prev = head;	\
	} while (0)


static inline void
list_add_tail (struct list_head *new1, struct list_head *head)
{
	new1->next = head;
	new1->prev = head->prev;

	new1->prev->next = new1;
	new1->next->prev = new1;
}

#define list_entry(ptr, type, member)					\
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

#endif /* _LLIST_H */

