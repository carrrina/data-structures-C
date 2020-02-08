#include "graph.h"

//TODO - Implementati functiile pentru un graf neorientat

//Functie care va deseneaza graful
void drawGraph(Graph g, char *name)
{
	int i, j;
	FILE *stream;
	char *buffer;
	List tmp;

	if (g == NULL || name == NULL)
		return;
	stream = fopen(name, "w");
	fprintf(stream, "graph G {\n");
	fprintf(stream, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=yellow];\n");
	for (i = 0; i < g->V; i++) {
		tmp = g->adjLists[i];
		while (tmp != NULL) {
			if (tmp->data > i)
				fprintf(stream, "    %d -- %d;\n", i, tmp->data);
			tmp = tmp->next;
		}
	}
	fprintf(stream, "}\n");
	fclose(stream);
	buffer = (char*) malloc(SIZE*sizeof(char));
	sprintf(buffer, "dot %s | neato -n -Tpng -o graph.png", name);
	system(buffer);
	free(buffer);
}

Graph initGraph(int V) {
	int i;
	Graph g = (Graph)malloc(sizeof(struct graph));
	g->V = V;
	g->adjLists = (List*)malloc(V * sizeof(List));
	g->dist = (int *)malloc(V * sizeof(int)); 
	for (i = 0; i < V; i++) {
		g->adjLists[i] = NULL;
		g->dist[i] = -1;
	}
	return g;
}

Graph insertEdge(Graph g, int u, int v) {
	g->adjLists[u] = addLast(g->adjLists[u], v);
	g->adjLists[v] = addLast(g->adjLists[v], u);
	return g;
}

Graph deleteVertex(Graph g, int v) {
	int i, u;
	Graph aux = g; List l = NULL;
	aux->V = g->V - 1;
	for (i = 0; i < g->V; i++) {
		if (i == v) 
			continue;
		l = aux->adjLists[i];
		while (l != NULL) {
			u = l->data;
			if (u == v) 
				l = deleteItem(l, u);
			if (u > v)
				(l->data)--;
			l = l->next;
		}
	}
	aux->adjLists[v] = freeList(aux->adjLists[i]);
	for (i = v; i < g->V - 1; i++) {
		aux->adjLists[i] = aux->adjLists[i + 1];
		aux->dist[i] = aux->dist[i + 1];
	}
	aux->adjLists[g->V - 1] = freeList(aux->adjLists[g->V - 1]);
	return aux;
}

int getInDegree(Graph g, int v) {
	int nr = 0;
	List l = g->adjLists[v];
	while (l != NULL) {
		nr++;
		l = l->next;
	}
	return nr;
}

void printGraph(Graph g) {
	int i;
	List adjList;
	for (i = 0; i < g->V; i++) {
		printf("%d: ", i);
		adjList = g->adjLists[i];
		while (adjList != NULL) {
			printf("%d ", adjList->data);
			adjList = adjList->next;
		}
		printf("\n");
	}
}

void dfs(Graph g, int start) {
	Stack S = initStack(start);
	int u, x;
	List l = NULL;
	while (!isEmptyStack(S)) {
		u = top(S);
		S = pop(S);
		if (!g->dist[u]) {
			g->dist[u] = 1;
			printf("%d ", u);
			l = g->adjLists[u];
			while (l != NULL) {
				x = l->data;
				if (!g->dist[x])
					S = push(S, x);
				l = l->next;
			}
		}
	}
	S = freeStack(S);
}
 
void recursiveDFS(Graph g, int start) {
	
	int i, end = 1;
	for (i = 0; i < g->V; i++) 
		if (!g->dist[i]) //daca mai sunt noduri nevizitate
			end = 0;
	if (end)
		return;

	if (!g->dist[start]) {
		g->dist[start] = 1;
		printf("%d ", start);
	}
	List l = g->adjLists[start];
	int x;
	do {
		x = l->data;
		l = l->next;
	} while (g->dist[x] && l != NULL);
	if (l == NULL && g->dist[x])
		for (i = 0; i < g->V; i++)
			if (!g->dist[i]) {
				recursiveDFS(g, i);
				return;
			}
	recursiveDFS(g, x);
}

void bfs(Graph g, int start) {
	int *color, u, v, i;
	color = calloc(g->V, sizeof(int));
	List l = g->adjLists[start];
	for (i = 0; i < g->V; i++)
		color[i] = 'w';
	color[start] = 'g';
	printf("%d ", start);
	g->dist[start] = 0;
	Queue Q = NULL;
	Q = enqueue(Q, start);
	while (!isEmptyQueue(Q)) {
		u = first(Q);
		Q = dequeue(Q);
		l = g->adjLists[u];
		while (l != NULL) {
			v = l->data;
			if (color[v] == 'w') {
				color[v] = 'g';
				printf("%d ", v);
				g->dist[v] = g->dist[u] + 1;
				Q = enqueue(Q, v);
			}
			l = l->next;
		}
		color[u] = 'b';
	}
	Q = freeQueue(Q);
}

int connNumber(Graph g) {
	int nr = 0, i, u, *viz;
	viz = calloc(g->V, sizeof(int));
	Queue Q = NULL;
	List l = NULL;
	for (i = 0; i < g->V; i++)
		if (!viz[i]) {
			nr++;
			viz[i]= 1;
			Q = enqueue(Q, i);
			while (!isEmptyQueue(Q)) {
				u = first(Q);
				Q = dequeue(Q);
				l = g->adjLists[u];
				while (l != NULL) {
					u = l->data;
					if (!viz[u]) {
						viz[u] = 1;
						Q = enqueue(Q, u);
					}
					l = l->next;
				}
			}
		}
	Q = freeQueue(Q);
	return nr;
}

int isConnected(Graph g) { //verifica daca un graf e conex
	if (connNumber(g) == 1)
		return 1;
	else
		return 0;
}

int isArtPt(Graph g, int v) { //daca un nod este punct de articulatie
	Graph aux = deleteVertex(g, v);
	if (!isConnected(aux))
		return 1;
	else
		return 0;
}

int hasArtPts(Graph g) { //daca un graf are puncte de articulatie
	int nr = 0, i;
	for (i = 0; i < g->V; i++)
		if (isArtPt(g, i))
			return 1;
	return 0;
}

void biconnComp(Graph g) {
	int i, j, u, *mark, k;
	mark = malloc(g->V * sizeof(int));
	for (i = 0; i < g->V; i++)
		mark[i] = -1;
	Queue Q = NULL;
	List l = NULL;
	Graph h = g;
	printf("%d\n", g->V);
	for (i = 0; i < g->V; i++) {
		//printf("mark %d = %d\n", i, mark[i]);
		//if (mark[i] == -1) {
			mark[i]= i;
			l = g->adjLists[i];
			while (l != NULL) {
				u = l->data;
				//if (mark[u] == -1) 
					mark[u] = i; //marcheaza vecinii lui i
				l = l->next;
			}
			for (j = 0; j < g->V; j++) 
				if (mark[j] != i)
					Q = enqueue(Q, j);
			while (!isEmptyQueue(Q)) {
				h = deleteVertex(h, first(Q));
				Q = dequeue(Q);
			}
			if (!hasArtPts(h)) {
				printf("O componenta biconexa este:\n");
				printf("%d", i);
				l = g->adjLists[i];
				while (l != NULL) {
					printf(" %d", l->data);
					l = l->next;
				}
				printf("\n");
			}
			h = g;
		//}
	}
	Q = freeQueue(Q);
}

int main() 
{
	Graph g = initGraph(7);
	g = insertEdge(g, 0, 4);
	g = insertEdge(g, 0, 5);
	g = insertEdge(g, 0, 6);
	g = insertEdge(g, 1, 2);
	g = insertEdge(g, 1, 5);
	g = insertEdge(g, 2, 3);
	g = insertEdge(g, 2, 5);
	g = insertEdge(g, 3, 4);
	g = insertEdge(g, 4, 5);
	g = insertEdge(g, 4, 6);
	printGraph(g);
	g->dist = (int *)calloc(g->V, sizeof(int));
	dfs(g, 0);
	printf("\n");
	int i;
	for (i = 0; i < g->V; i++)
		g->dist[i] = 0;
	recursiveDFS(g, 0);
	for (i = 0; i < g->V; i++)
		g->dist[i] = -1;
	printf("\n");
	bfs(g, 1);
	printf("\n");
	printf("Nr de comp conexe este: %d\n", connNumber(g));
	if (isConnected(g))
		printf("Graful este conex.\n");
	else
		printf("Graful nu este conex.\n");
	
	Graph g1 = initGraph(10);
	g1 = insertEdge(g1, 0, 1);
	g1 = insertEdge(g1, 0, 2);
	g1 = insertEdge(g1, 0, 3);
	g1 = insertEdge(g1, 1, 2);
	g1 = insertEdge(g1, 1, 3);
	g1 = insertEdge(g1, 2, 3);
	g1 = insertEdge(g1, 4, 5);
	g1 = insertEdge(g1, 4, 6);
	g1 = insertEdge(g1, 5, 6);
	g1 = insertEdge(g1, 7, 8);
	printGraph(g1);
	printf("Nr de comp conexe este: %d\n", connNumber(g1));
	if (isConnected(g1))
		printf("Graful este conex.\n");
	else
		printf("Graful nu este conex.\n");
	//biconnComp(g1);
	return 0;
}