#include "stack.h"

Stack initStack(E data)
{
	Stack s = (Stack) malloc(sizeof(struct stack));
	s->data = data;
	s->next = NULL;
	return s;
}

Stack push(Stack s, E data)
{
	Stack top;

	if (isEmptyStack(s))
		return initStack(data);
	top = initStack(data);
	top->next = s;
	return top;
}

Stack pop(Stack s)
{
	Stack tmp;

	if (isEmptyStack(s))
		return s;
	tmp = s;
	s = s->next;
	free(tmp);
	return s;
}

E top(Stack s)
{
	if (isEmptyStack(s))
		return -1;
	return s->data;
}

int isEmptyStack(Stack s)
{
	if (s == NULL)
		return 1;
	return 0;
}

Stack freeStack(Stack s)
{
	while (!isEmptyStack(s)) {
		s = pop(s);
	}
	return NULL;
}
