#include<stdio.h>
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)

//braced group within expression
#define container_of(PTR, TYPE, MEMBER)({                   \
        const typeof(((TYPE *)0)->MEMBER)* __mptr = (PTR); \
        (TYPE*)( (char*) __mptr -offsetof(TYPE, MEMBER));})

struct radax {
   int a;
   int b;
   char c;
   unsigned int d;
};

void main() {
 struct radax r;
 r.c = 'c';
 r.a = 4;r.b=5;

 printf("%ld\n", offsetof(struct radax, c)); 
 printf("%ld\n", offsetof(struct radax, a)); 
}
