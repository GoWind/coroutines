

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)

//braced group within expression
#define container_of(PTR, TYPE, MEMBER)({                   \
        const typeof(((TYPE *)0)->MEMBER)* __mptr = (PTR); \
        (TYPE*)( (char*) __mptr -offsetof(TYPE, MEMBER));})


void sc_list_insert(struct sc_list_head *head, struct sc_list_head *item)
{
	item->next = head->next;
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

//head -> head of the list
//container_type -> struct in which sc_list_head is a  field
//field_name -> what is the name of field in container_type that is sc_list_head
//var_ptr, a pointer to container_type which holds the value of the 
//current element while iterating
#define sc_list_for_each_entry(var_ptr, head, field_name, container_type) \
        for(var_ptr = container_of((head)->next, container_type, field_name); \
            &(var_ptr->field_name) != head; \
            var_ptr = sc_container_of(var_ptr->field_name.next, container_type, field_name))

                        
