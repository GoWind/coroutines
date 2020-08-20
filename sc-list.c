#include "sc-list.h"

void sc_list_insert(struct sc_list_head* head, struct sc_list_head* item)
{
	(item)->next = head->next;
	item->prev = head;
	item->next->prev = item;
	head->next = item;
}

void sc_list_insert_end(struct sc_list_head *head, struct sc_list_head *item)
{
	item->next = head;
	item->prev = head->prev;
	head->prev->next = item;
	head->prev = item;
}

void sc_list_remove(struct sc_list_head *item)
{
	item->prev->next = item->next;
	item->next->prev = item->prev;
}

void sc_list_init(struct sc_list_head *head)
{
	head->prev = head;
	head->next = head;
}


                       
