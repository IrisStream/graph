#include "graph.h"

Graph CreateGraphFromFile(string file_name)
{
	fstream input(file_name, ios::in);
	Graph graph;
	input >> graph.num_vertices;
	for (int i = 0; i < graph.num_vertices; i++)
	{
		graph.free[i] = true;
	}
	for (int i = 0; i < graph.num_vertices; i++)
	{
		for (int j = 0; j < graph.num_vertices; j++)
		{
			input >> graph.adjacency_matrix[i][j];
		}
	}
	input.close();
	return graph;
}

bool IsValidGraph(Graph g)
{
	if (g.num_vertices < 0)
		return false;
	for (int i = 0; i < g.num_vertices; i++)
	{
		if (g.adjacency_matrix[i][i] != 0)
			return false;
		for (int j = 0; j < g.num_vertices; j++)
		{
			if (i != j && g.adjacency_matrix[i][j] < 0)
				return false;
		}
	}
	return true;
}

bool IsUndirectedGraph(Graph g)
{
	for (int i = 0; i < g.num_vertices; i++)
	{
		for (int j = i + 1; j < g.num_vertices; j++)
		{
			if (g.adjacency_matrix[i][j] != g.adjacency_matrix[j][i])
			{
				return false;
			}
		}
	}
	return true;
}

void DisplayGraph(Graph g)
{
	cout << "So luong dinh cua do thi la: " << g.num_vertices << endl;
	bool flag = IsUndirectedGraph(g);
	for (int i = 0; i < g.num_vertices; i++)
	{
		for (int j = 0; j < g.num_vertices; j++)
		{
			if (flag)
			{
				if (i <= j && g.adjacency_matrix[i][j])
					cout << i << "<->" << j << endl;
			}
			else
			{
				if (g.adjacency_matrix[i][j])
					cout << i << "->" << j << endl;
			}
		}
	}
}

int CountEdge(Graph g)
{
	int numEdges = 0;
	for (int i = 0; i < g.num_vertices; i++)
	{
		for (int j = 0; j < g.num_vertices; j++)
		{
			if(g.adjacency_matrix[i][j] == 1)
				numEdges++;
		}
	}
	if (IsUndirectedGraph(g))
		return numEdges / 2;
	else
		return numEdges;
}

int CountVertices(Graph g, int flag)
{
	int evenDegVertices = 0;
	for(int i = 0;i<g.num_vertices;i++)
	{
		int deg = 0;
		for (int j = 0; j < g.num_vertices; j++)
		{
			if (g.adjacency_matrix[i][j] == 1)
				deg++;
		}
		if (deg % 2 == 0)
			evenDegVertices++;
	}
	if (flag == 1)
		return g.num_vertices - evenDegVertices;
	else
		return evenDegVertices;
}

void resetState(Graph g)
{
	for (int i = 0; i < g.num_vertices; i++)
	{
		g.free[i] = true;
	}
}

void BFS(Graph g, int start_vertex)
{
	g.free[start_vertex] = false;
	queue<int> q;
	q.push(start_vertex);
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		for (int v = 0; v < g.num_vertices; v++)
		{
			if (g.adjacency_matrix[u][v] == 1 && g.free[v])
			{
				g.free[v] = false;
				q.push(v);
			}
		}
	}
}

void DFS(Graph g, int start_vertex)
{
	g.free[start_vertex] = false;
	stack<int> q;
	q.push(start_vertex);
	while (!q.empty())
	{
		int u = q.top();
		q.pop();
		for (int v = 0; v < g.num_vertices; v++)
		{
			if (g.adjacency_matrix[u][v] == 1 && g.free[v])
			{
				g.free[v] = false;
				q.push(v);
			}
		}
	}
}

int CountConnectedComponents(Graph g)
{
	resetState(g);
	int components = 0;
	for (int i = 0; i < g.num_vertices; i++)
	{
		if (g.free[i] == true)
		{
			components++;
			BFS(g, i);
		}
	}
	return components;
}

bool IsConnectedGraph(Graph g)
{
	return (CountConnectedComponents(g) == 1);
}

void FindShortestPathDijkstra(Graph g, int start_vertex)
{
	priority_queue<ii,vector<ii>,greater<ii> > q;
	int* dist = new int[g.num_vertices];
	for (int i = 0; i < g.num_vertices; i++)
	{
		dist[i] = oo;
	}
	dist[start_vertex] = 0;
	q.push(mp(0,start_vertex));
	while (!q.empty())
	{
		int u = q.top().second;
		int du = q.top().first;
		q.pop();
		for (int v = 0; v < g.num_vertices; v++)
		{
			int uv = g.adjacency_matrix[u][v];
			if (uv > 0 && dist[v] > dist[u] + uv)
			{
				dist[v] = dist[u] + uv;
				q.push(mp(dist[v],v));
			}
		}
	}
	for (int i = 0; i < g.num_vertices; i++)
	{
		if (i != start_vertex)
		{
			if (dist[i] == oo)
				cout << "Khong co duong di tu " << start_vertex << " den " << i << endl;
			else
				cout << "Duong di ngan nhat tu " << start_vertex << " den " << i << " la " << dist[i] << endl;
		}
	}
	delete[]dist;
}

void FindShortestPathFloyd(Graph g, int start_vertex)
{
	int** shortestDist = new int* [g.num_vertices];
	for (int i = 0; i < g.num_vertices; i++)
	{
		shortestDist[i] = new int[g.num_vertices];
	}
	for (int i = 0; i < g.num_vertices; i++)
	{
		for (int j = 0; j < g.num_vertices; j++)
		{
			if (i!= j && g.adjacency_matrix[i][j] == 0)
				shortestDist[i][j] = oo;
			else
				shortestDist[i][j] = g.adjacency_matrix[i][j];
		}
	}
	for (int k = 0; k < g.num_vertices; k++)
	{
		for (int i = 0; i < g.num_vertices; i++)
		{
			for (int j = 0; j < g.num_vertices; j++)
			{
				shortestDist[i][j] = min(shortestDist[i][j], shortestDist[i][k] + shortestDist[k][j]);
			}
		}
	}
	for (int i = 0; i < g.num_vertices; i++)
	{
		if (i != start_vertex)
		{
			if (shortestDist[start_vertex][i] == oo)
				cout << "Khong co duong di tu " << start_vertex << " den " << i << endl;
			else
				cout << "Duong di ngan nhat tu " << start_vertex << " den " << i << " la " << shortestDist[i] << endl;
		}
	}
	for (int i = 0; i < g.num_vertices; i++)
	{
		delete[] shortestDist[i];
	}
	delete[] shortestDist;
}

void FindShortestPathBellman(Graph g, int start_vertex)
{
	resetState(g);
	queue<int> q;
	int* dist = new int[g.num_vertices];
	for (int i = 0; i < g.num_vertices; i++)
	{
		dist[i] = oo;
	}
	dist[start_vertex] = 0;
	q.push(start_vertex);
	g.free[start_vertex] = false;
	while (!q.empty())
	{
		int u = q.front();
		g.free[u] = true;
		q.pop();
		for (int v = 0; v < g.num_vertices; v++)
		{
			int uv = g.adjacency_matrix[u][v];
			if (dist[v] > dist[u] + uv)
			{
				dist[v] = dist[u] + uv;
				if (g.free[v] == true)
				{
					q.push(v);
					g.free[v] = false;
				}
			}
		}
	}
	for (int i = 0; i < g.num_vertices; i++)
	{
		if (i != start_vertex)
		{
			if (dist[i] == oo)
				cout << "Khong co duong di tu " << start_vertex << " den " << i << endl;
			else
				cout << "Duong di ngan nhat tu " << start_vertex << " den " << i << " la " << dist[i] << endl;
		}
	}
	delete[]dist;
}