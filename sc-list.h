struct sc_list_head {
    struct sc_list_head* prev;
    struct sc_list_head* next;
};

void sc_list_insert(struct sc_list_head*, struct sc_list_head*);
void sc_list_insert_end(struct sc_list_head *, struct sc_list_head *);
void sc_list_remove(struct sc_list_head*);
void sc_list_init(struct sc_list_head*);
