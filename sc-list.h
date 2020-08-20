#ifndef _SC_LIST_H
#define _SC_LIST_H
struct sc_list_head {
    struct sc_list_head* prev;
    struct sc_list_head* next;
};

void sc_list_insert(struct sc_list_head*, struct sc_list_head*);
void sc_list_insert_end(struct sc_list_head *, struct sc_list_head *);
void sc_list_remove(struct sc_list_head*);
void sc_list_init(struct sc_list_head*);


//head -> head of the list
//container_type -> struct in which sc_list_head is a  field
//field_name -> what is the name of field in container_type that is sc_list_head
//var_ptr, a pointer to container_type which holds the value of the 
//current element while iterating
#define sc_list_for_each_entry(var_ptr, head, field_name, container_type) \
        for(var_ptr = sc_container_of((head)->next, container_type, field_name); \
            &(var_ptr->field_name) != head; \
            var_ptr = sc_container_of(var_ptr->field_name.next, container_type, field_name))

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)

//Uses Gcc's braced group within expression extension to the C language
//https://radek.io/2012/11/10/magical-container_of-macro/
#define sc_container_of(PTR, TYPE, MEMBER)({                   \
        const typeof(((TYPE *)0)->MEMBER)* __mptr = (PTR); \
        (TYPE*)( (char*) __mptr - offsetof(TYPE, MEMBER));})


#endif
