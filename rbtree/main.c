#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "rbtree.h"

struct WFNSPolicyEntry
{
    struct rb_node rb;
    int value;
    char name[1];
};

struct rb_root root;

int add_policy(const char *name, int value)
{
	struct rb_node **p = &root.rb_node;
	struct rb_node *parent = NULL;
	struct WFNSPolicyEntry *entry;
	int n, ret = -1;

	while (*p)
	{
		parent = *p;
		entry = rb_entry(*p, struct WFNSPolicyEntry, rb);
		n = strcasecmp(name, entry->name);
		if (n < 0)
			p = &(*p)->rb_left;
		else if (n > 0)
			p = &(*p)->rb_right;
		else
			break;
	}

	if (!*p)
	{
		size_t len = strlen(name);
		size_t size = offsetof(struct WFNSPolicyEntry, name) + len + 1;

		entry = (struct WFNSPolicyEntry *)malloc(size);
		if (entry)
		{
			memcpy(entry->name, name, len + 1);
			entry->value = value;
			rb_link_node(&entry->rb, parent, p);
			rb_insert_color(&entry->rb, &root);
			ret = 0;
		}
	}
	else {
        printf(" exist\n");
    }
	return ret;
}

inline struct  WFNSPolicyEntry* get_policy_entry(const char *name)
{
	struct rb_node *p = root.rb_node;
	struct WFNSPolicyEntry *entry;
	int n;

	while (p) {
		entry = rb_entry(p, struct WFNSPolicyEntry, rb);
		n = strcasecmp(name, entry->name);
		if (n < 0)
			p = p->rb_left;
		else if (n > 0)
			p = p->rb_right;
		else
			break;
	}
    if (entry) {
        printf("key: %s, value: %d\n", entry->name, entry->value);
        return entry;
    } else {
        printf("key:%s not found\n", name);
    }


	return NULL;
}

void for_each(struct rb_node *p) {
    if (!p) {
        return;
    }
    for_each(p->rb_left);
	struct WFNSPolicyEntry *entry;
	entry = rb_entry(p, struct WFNSPolicyEntry, rb);
    printf("key: %s, value: %d\n", entry->name, entry->value);
    for_each(p->rb_right);
}

int main() {
    root.rb_node = NULL;
    add_policy("a", 1);
    add_policy("b", 2);
    add_policy("c", 3);
    add_policy("ca1", 4);
    add_policy("ca2", 4);
    add_policy("ca3", 4);
    for_each(root.rb_node);
    return 0;
}