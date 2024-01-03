#ifndef LIST
#define LIST

#define LIST_SIZE_STEP 10

typedef struct {
	int count;
	int size;
	void **data;
} list;

list *list_init();

int list_add(list *l, void *data);

void *list_get(list *l, int index);

int list_remove(list *l, int index);

int list_insert(list *l, void *data, int index);

#endif
