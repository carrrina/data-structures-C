#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>	

typedef int V;

typedef struct list {
	V data;
	struct list *prev, *next;
}*List;

List initList(V data);
List addFirst(List list, V data);
List addLast(List list, V data);
List deleteItem(List list, V data);
List freeList(List list);

#endif