#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKET_ALLOC 16

typedef struct Bucket Bucket;
typedef struct Hashmap Hashmap;
typedef struct LinkedList LinkedList;

Bucket *new_map_bucket();
Hashmap *new_map();
LinkedList *new_map_linked(char *key, char *value);
size_t hash_func(char *item, size_t map_size);
void add_map_linked(LinkedList *self, char *key, char *value);
void insert_map(Hashmap *self, char *key, char *value);

struct LinkedList {
	struct LinkedList *n;
	char *key;
	char *value;
};

struct Bucket {
	LinkedList *linkedlist;
};

struct Hashmap {
	Bucket *buckets;
	size_t len;
	size_t size;
};

size_t hash_func(char *item, size_t map_size)
{
	size_t len = strlen(item);

	size_t acc = 0;
	for (size_t i = 0; i <= len - 1; ++i) {
		acc += item[i];
	}
	acc += 1;

	return ((len - acc) / (acc + len)) % (map_size);
}

LinkedList *new_map_linked(char *key, char *value)
{
	LinkedList *self = malloc(sizeof(LinkedList));
	self->key = key;
	self->value = value;
	self->n = NULL;

	return self;
}

void add_map_linked(LinkedList *self, char *key, char *value)
{
	LinkedList *p = self;

	/* Walk list */
	while (p->n != NULL) {
		p = p->n;
	}

	p->n = malloc(sizeof(LinkedList));
	p = p->n;

	/* Point item to value */
	p->key = key;
	p->value = value;
	p->n = NULL;
}

/* Allocate a new bucket. */
Bucket *new_map_bucket()
{
	Bucket *self = malloc(sizeof(Bucket) * BUCKET_ALLOC);
	self->linkedlist = NULL;

	return self;
}

/*
 * Allocate and setup new hashmap with a
 * default bucket size.
 */
Hashmap *new_map()
{
	Hashmap *this = malloc(sizeof(Hashmap));
	this->buckets = new_map_bucket();
	this->len = 0;
	this->size = BUCKET_ALLOC;

	return this;
}

void insert_map(Hashmap *self, char *key, char *value)
{
	size_t idx = hash_func(key, self->size);

	/* No collisions */
	if (self->buckets[idx].linkedlist == NULL) {
		self->buckets[idx].linkedlist = new_map_linked(key, value);
	} else {
		/* Collisions detected */
		LinkedList *p = self->buckets[idx].linkedlist;

		add_map_linked(p, key, value);
	}
}

/*
 * Searches hashmap for key-value.
 *
 * Returns NULL if value does not exist.
 */
char *search_map(Hashmap *self, char *key)
{
	size_t idx = hash_func(key, self->size);

	LinkedList *p = self->buckets[idx].linkedlist;
	do {
		if (!strcmp(p->key, key)) {
			return p->value;
		} else {
			p = p->n;
		}
	} while (p != NULL);

	return NULL;
}

int main(int argc, char *argv[])
{
	Hashmap *map = new_map();

	/*
	printf("%lu\n", hash_func("key", BUCKET_ALLOC));
	printf("%lu\n", hash_func("ANOTHER", BUCKET_ALLOC));
	printf("%lu\n", hash_func("BRUH", BUCKET_ALLOC));
	printf("%lu\n", hash_func("bruh", BUCKET_ALLOC));
	*/

	insert_map(map, "key", "value");
	insert_map(map, "ANOTHER", "VALUE");
	insert_map(map, "BRUH", "first");
	insert_map(map, "bruh", "second");
	insert_map(map, "nuts", "balls");
	insert_map(map, "hello", "nuts");
	insert_map(map, "four", "my");

	printf("%s\n", search_map(map, "key"));
	printf("%s\n", search_map(map, "ANOTHER"));
	printf("%s\n", search_map(map, "BRUH"));
	printf("%s\n", search_map(map, "bruh"));
	printf("%s\n", search_map(map, "nuts"));
	printf("%s\n", search_map(map, "hello"));

	return 0;
}
