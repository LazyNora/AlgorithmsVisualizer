#ifndef SSSPALGO_H_
#define SSSPALGO_H_

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#include "Player.h"

const char* printNode(float Distance, bool Visited, int Previous, bool Marked);

int Dijkstra(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src);
int FloydWarshall(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n);
int BellmanFord(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src);
int Kruskal(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n);
int Prim(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src);
int BreadthFirstSearch(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src);
int DepthFirstSearch(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src);

#endif // !SSSPALGO_H_

#ifdef SSSPALGO_IMPLEMENTATION

const char* printNode(float Distance, bool Visited, int Previous, bool Marked) {
	if (FloatEquals(Distance, 0) && !Visited && Previous == -1)
		return TextFormat("0%c", Marked ? '*' : '\0');
	else if (Visited)
		return TextFormat("-");
	else if (FloatEquals(Distance, FLT_MAX))
		return TextFormat("(INF;-)");
	else
		return TextFormat("(%g;%s)%c", Distance, Previous == -1 ? "null" : TextFormat("%d", Previous), Marked ? '*' : '\0');
}

char* Path(int* Previous, int src, int dest) {
	std::stack<int> path;

	int i = dest;
	while (i != src && i != -1) {
		path.push(i);
		i = Previous[i];
	}
	char* result = new char[100];
	strcpy(result, TextFormat("%d", src));
	while (!path.empty()) {
		strcat(result, TextFormat(" -> %d", path.top()));
		path.pop();
	}
	return result;
}

// start = src
int Dijkstra(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src) {
	//Init Visualizer {
	player.init();
	VIS::Table table;
	table.init(n, "Dijkstra Algorithm");
	TABLE.tables.push_back(table);
	VIS::TableRow row;
	PSEUDO.codes[0] = "function Dijkstra(Graph, source):";
	PSEUDO.codes[1] = "";
	PSEUDO.codes[2] = "    for each vertex v in Graph.Vertices:";
	PSEUDO.codes[3] = "        dist[v] <- INF";
	PSEUDO.codes[4] = "        prev[v] <- null";
	PSEUDO.codes[5] = "        add v to Q";
	PSEUDO.codes[6] = "    dist[source] <- 0";
	PSEUDO.codes[7] = "";
	PSEUDO.codes[8] = "    while Q is not empty:";
	PSEUDO.codes[9] = "        u <- vertex in Q with min dist[u]";
	PSEUDO.codes[10] = "        remove u from Q";
	PSEUDO.codes[11] = "";
	PSEUDO.codes[12] = "        for each neighbor v of u still in Q:";
	PSEUDO.codes[13] = "           alt <- dist[u] + Graph.Edges(u, v)";
	PSEUDO.codes[14] = "           if alt < dist[v]:";
	PSEUDO.codes[15] = "               dist[v] <- alt";
	PSEUDO.codes[16] = "               prev[v] <- u";
	PSEUDO.codes[17] = "";
	PSEUDO.codes[18] = "     return dist[], prev[]";
	//}

	std::vector<int> Q; //The set of nodes that are not visited
	float* Distance = new float[n]; //Distance from src to each nodes
	bool* Visited = new bool[n]; //Indicate if the node is visited
	int* Previous = new int[n];
	float minDistance = FLT_MAX;
	int minIndex = -1;
	for (int i = 0; i < n; i++) { //Init
		Distance[i] = FLT_MAX;
		Previous[i] = -1;
		Q.push_back(i);
		Visited[i] = 0;

		//Visualize {
		table.colHeaders[i] = TextFormat("%d", i);
		row[i].text = printNode(Distance[i], Visited[i], -1, 0);
		//}
	}

	//Visualize {
	table.footer.push_back("Init");
	table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table.rowHeaders[0] = "Init";
	table.data[0] = row;
	TABLE.tables[0] = table;
	PSEUDO.highlight(2);
	PSEUDO.highlight(3);
	PSEUDO.highlight(4);
	PSEUDO.highlight(5);
	player.push();
	PSEUDO.reset();
	//}

	Distance[src] = 0; //Distance from src to itself is 0

	//Visualize {
	table.footer[0] = "Distance from source to itself is 0";
	table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
	row[src].text = printNode(Distance[src], Visited[src], Previous[src], 0);
	table.data[0] = row;
	table.select(0, src);
	TABLE.tables[0] = table;
	GRAPH.visit(src, -1, Distance[src]);
	PSEUDO.highlight(6, VIS_Pseudo_Color_LightGreen);
	player.push();
	PSEUDO.reset();
	GRAPH.leave(src, -1, Distance[src]);
	//}

	while (!Q.empty()) {
		int u = Q[0];
		for (int i = 1; i < Q.size(); i++) {
			if (Distance[Q[i]] < Distance[u])
				u = Q[i];
		}
		Q.erase(std::find(Q.begin(), Q.end(), u));

		//Visualize {
		for (int j = 0; j < n; j++)
			row[j].text = printNode(Distance[j], Visited[j], Previous[j], 0);
		row[u].text = printNode(Distance[u], Visited[u], Previous[u], 1);
		table.deselect((int)table.data.size() - 1, u);
		table.footer[0] = TextFormat("D[%d] = %s, mark %d as visited", u, FloatEquals(Distance[u], FLT_MAX) ? "INF" : TextFormat("%g", Distance[u]), u);
		table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		table.data[(int)table.data.size() - 1] = row;
		table.patch((int)table.data.size() - 1, u);
		GRAPH.visit(u, -1, Distance[u]);
		TABLE.tables[0] = table;
		PSEUDO.highlight(8, VIS_Pseudo_Color_LightTeal);
		PSEUDO.highlight(9, VIS_Pseudo_Color_LightOrange);
		PSEUDO.highlight(10, VIS_Pseudo_Color_LightRed);
		player.push();
		PSEUDO.reset();
		//}

		Visited[u] = true;

		//Visualize {
		table.footer[0] = TextFormat("Find neighbors of %d", u);
		table.FooterColor = { 253, 205, 172, 255 };
		TABLE.tables[0] = table;
		PSEUDO.highlight(8, VIS_Pseudo_Color_LightTeal);
		PSEUDO.highlight(12, VIS_Pseudo_Color_LightPink);
		player.push();
		PSEUDO.reset();
		table.rowHeaders[(int)table.data.size()] = TextFormat("%d", table.data.size());
		table.data[(int)table.data.size()] = row;
		//}

		int c = 0;

		for (int i = 0; i < n; i++) {
			if (!FloatEquals(graph[u][i], 0) && !Visited[i]) {
				float alt = Distance[u] + graph[u][i];

				c++;

				//Visualize {
				for (int j = 0; j < n; j++)
					row[j].text = printNode(Distance[j], Visited[j], Previous[j], 0);
				table.data[(int)table.data.size() - 1] = row;
				table.select((int)table.data.size() - 1, i);
				table.footer[0] = TextFormat("D[%d] = %s, D[%d] + W(%d,%d) = %g", i, FloatEquals(Distance[i], FLT_MAX) ? "INF" : TextFormat("%g", Distance[i]), u, u, i, alt);
				table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
				TABLE.tables[0] = table;
				GRAPH.select(i, u);
				PSEUDO.highlight(8, VIS_Pseudo_Color_LightTeal);
				PSEUDO.highlight(12, VIS_Pseudo_Color_LightPink);
				PSEUDO.highlight(13, VIS_Pseudo_Color_LightBlue);
				PSEUDO.highlight(14, VIS_Pseudo_Color_LightYellow);
				player.push();
				PSEUDO.reset();
				//}

				if (alt < Distance[i]) {
					Distance[i] = alt;
					Previous[i] = u;

					//Visualize {
					row[i].text = printNode(Distance[i], Visited[i], Previous[i], 0);
					table.data[(int)table.data.size() - 1] = row;
					table.footer[0] = TextFormat("D[%d] + W(%d,%d) < D[%d]. Update D[%d] = %g, prev[%d] = %d", u, u, i, i, i, Distance[i], i, u);
					table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
					table.mark((int)table.data.size() - 1, i);
					TABLE.tables[0] = table;
					GRAPH.visit(i, u, Distance[i]);
					PSEUDO.highlight(8, VIS_Pseudo_Color_LightTeal);
					PSEUDO.highlight(12, VIS_Pseudo_Color_LightPink);
					PSEUDO.highlight(15, VIS_Pseudo_Color_LightBlue);
					PSEUDO.highlight(16, VIS_Pseudo_Color_LightBlue);
					player.push();
					PSEUDO.reset();
					table.unmark((int)table.data.size() - 1, i);
					GRAPH.leave(i, u, Distance[i]);
					//}
				}
				//Visualize {
				GRAPH.deselect(i, u);
				//}
			}
		}
		//Visualize {
		TABLE.tables[0].footer[0] = (c == 0) ? TextFormat("%d has no neighbors in Q", u) : TextFormat("Updated all neighbors of %d in Q", u);
		TABLE.tables[0].FooterColor = { 253, 205, 172, 255 };
		PSEUDO.highlight(8, VIS_Pseudo_Color_LightTeal);
		PSEUDO.highlight(12, VIS_Pseudo_Color_LightPink);
		GRAPH.leave(u, -1, Distance[u]);
		player.push();
		PSEUDO.reset();
		//}
	}

	//Visualize {
	table.data.erase(std::prev(table.data.end()));
	std::vector<std::string> output;
	output.push_back("Result: ");
	int c = 0;
	for (c = 0; c < n; c++) {
		if (FloatEquals(Distance[c], FLT_MAX)) {
			table.footer[0] = TextFormat("D[%d] = INF, Graph is not connected!", c);
			table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.773f, 0.067f, 0.384f, 1.00f }));
			row[c].text = TextFormat("(INF;-)");
			table.data[(int)table.data.size() - 1] = row;
			table.mark((int)table.data.size() - 1, c);
			TABLE.tables[0] = table;
			GRAPH.visit(c, -1, FLT_MAX);
			PSEUDO.highlight(18, VIS_Pseudo_Color_LightRed);
			player.push();
			break;
		}
		output.push_back(TextFormat("   %d: %s, D[%d] = %g", c, Path(Previous, src, c), c, Distance[c]));
	}
	if (c == n) {
		table.footer = output;
		table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		TABLE.tables[0] = table;
		PSEUDO.highlight(18, VIS_Pseudo_Color_LightLime);
		player.push();
	}
	//}

	delete[] Distance;
	delete[] Visited;
	delete[] Previous;
	Q.clear();
	row.clear();
	table.clear();
	output.clear();
	return 0;
}

int FloydWarshall(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n) {
	//Init Visualizer {
	player.init();
	VIS::Table table1;
	VIS::Table table2;
	VIS::Table context;
	context.showColHeaders = false;
	context.showRowHeaders = false;
	PSEUDO.codes[0] = "function FloydWarshall(Graph):";
	PSEUDO.codes[1] = "    let dist be a |V|*|V| array of minimum distances\n    initialized to INF";
	PSEUDO.codes[2] = "    let prev be a |V|*|V| array of vertex indices\n    initialized to null";
	PSEUDO.codes[3] = "";
	PSEUDO.codes[4] = "    for each vertex v in Graph.Vertices:";
	PSEUDO.codes[5] = "        dist[v][v] <- 0";
	PSEUDO.codes[6] = "    for each edge e in Graph.Edges:";
	PSEUDO.codes[7] = "        dist[e.from][e.to] <- e.weight";
	PSEUDO.codes[8] = "        prev[e.from][e.to] <- e.from";
	PSEUDO.codes[9] = "";
	PSEUDO.codes[10] = "    for k from 1 to |V|:";
	PSEUDO.codes[11] = "        for i from 1 to |V|:";
	PSEUDO.codes[12] = "            for j from 1 to |V|:";
	PSEUDO.codes[13] = "                if dist[i][j] > dist[i][k] + dist[k][j]:";
	PSEUDO.codes[14] = "                    dist[i][j] <- dist[i][k] + dist[k][j]";
	PSEUDO.codes[15] = "                    prev[i][j] <- prev[k][j]";
	PSEUDO.codes[16] = "";
	PSEUDO.codes[17] = "    return dist, prev";

	//}

	float** Distance = new float* [n]; //Distance from one node to another
	int** Path = new int* [n]; //The previous node of the shortest path
	for (int i = 0; i < n; i++) {
		Distance[i] = new float[n];
		Path[i] = new int[n];

		//Visualize {
		table1.colHeaders[i] = TextFormat("%d", i);
		table1.rowHeaders[i] = TextFormat("%d", i);
		table2.colHeaders[i] = TextFormat("%d", i);
		table2.rowHeaders[i] = TextFormat("%d", i);
		//}

		for (int j = 0; j < n; j++) {
			//Distance from one node to itself is 0
			if (i == j) {
				Distance[i][j] = 0;
				Path[i][j] = -1;
			}
			//If there is no path, the distance is infinity
			else if (FloatEquals(graph[i][j], 0)) {
				Distance[i][j] = FLT_MAX;
				Path[i][j] = -1;
			}
			//Distance between two connected nodes is the weight of the edge
			else {
				Distance[i][j] = graph[i][j];
				Path[i][j] = j;
			}
		}
	}

	//Visualize {
	context.init(0, "Floyd - Warshall Algorithm");
	context.footer.push_back("Init");
	context.footer.push_back("");
	context.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table1.title = "Floyd - Warshall Algorithm";
	table1.footer.push_back("Distance Matrix");
	table1.set(Distance, n, n);
	table2.title = "Floyd - Warshall Algorithm";
	table2.footer.push_back("Path Matrix");
	table2.set(Path, n, n);
	TABLE.tables.push_back(context);
	TABLE.tables.push_back(table1);
	TABLE.tables.push_back(table2);
	PSEUDO.highlight(1);
	PSEUDO.highlight(2);
	PSEUDO.highlight(4);
	PSEUDO.highlight(5);
	PSEUDO.highlight(6);
	PSEUDO.highlight(7);
	PSEUDO.highlight(8);
	player.push();
	PSEUDO.reset();
	//}

	for (int k = 0; k < n; k++) {
		//Visualize {
		context.footer[0] = TextFormat("Choose %d as intermediate vertex", k);
		context.FooterColor = { 146, 207, 227, 255 };
		table1.disableCol(k);
		table1.disableRow(k);
		table2.disableCol(k);
		table2.disableRow(k);
		TABLE.tables[0] = context;
		TABLE.tables[1] = table1;
		TABLE.tables[2] = table2;
		GRAPH.visit(k);
		PSEUDO.highlight(10, VIS_Pseudo_Color_LightPink);
		player.push();
		//}
		for (int i = 0; i < n; i++) {
			if (k == i)
				continue;

			for (int j = 0; j < n; j++) {
				if (k == j || i == j) continue;
				//Visualize {
				context.footer[0] = TextFormat("Check if D[%d][%d] > D[%d][%d] + D[%d][%d]", i, j, i, k, k, j);
				context.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
				table1.mark(i, k);
				table2.mark(i, k);
				table1.mark(k, j);
				table2.mark(k, j);
				table1.select(i, j);
				table2.select(i, j);
				TABLE.tables[0] = context;
				TABLE.tables[1] = table1;
				TABLE.tables[2] = table2;
				GRAPH.visit(i);
				GRAPH.visit(k, i);
				GRAPH.visit(j, k);
				GRAPH.visit(j, i);
				PSEUDO.highlight(10, VIS_Pseudo_Color_LightPink);
				PSEUDO.highlight(11, VIS_Pseudo_Color_LightOrange);
				PSEUDO.highlight(12, VIS_Pseudo_Color_LightTeal);
				PSEUDO.highlight(13, VIS_Pseudo_Color_LightYellow);
				player.push();
				table1.deselect(i, j);
				table2.deselect(i, j);
				//}

				if (Distance[i][k] + Distance[k][j] < Distance[i][j]) {
					Distance[i][j] = Distance[i][k] + Distance[k][j];
					Path[i][j] = Path[i][k];

					//Visualize {
					context.footer[0] = TextFormat("D[%d][%d] > D[%d][%d] + D[%d][%d]", i, j, i, k, k, j);
					context.footer[1] = TextFormat("Update D[%d][%d] = %g, prev[%d][%d] = %d", i, j, Distance[i][j], i, j, k);
					context.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
					table1.patch(i, j);
					table2.patch(i, j);
					table1.data[i][j].text = TextFormat("%g", Distance[i][j]);
					table2.data[i][j].text = TextFormat("%d", Path[i][j]);
					TABLE.tables[0] = context;
					TABLE.tables[1] = table1;
					TABLE.tables[2] = table2;
					GRAPH.visit(j, i, Distance[i][j]);
					PSEUDO.reset(13);
					PSEUDO.highlight(14, VIS_Pseudo_Color_LightBlue);
					PSEUDO.highlight(15, VIS_Pseudo_Color_LightBlue);
					player.push();
					context.footer[1] = "";
					table1.depatch(i, j);
					table2.depatch(i, j);
					GRAPH.leave(j, i, Distance[i][j]);
					PSEUDO.reset();
					//}
				}
				//Visualize {
				table1.unmark(i, k);
				table2.unmark(i, k);
				table1.unmark(k, j);
				table2.unmark(k, j);
				TABLE.tables[1] = table1;
				TABLE.tables[2] = table2;
				GRAPH.leave(i);
				GRAPH.leave(k, i);
				GRAPH.leave(j, k);
				GRAPH.leave(j, i);
				//}
			}
		}
		//Visualize {
		table1.disableCol(k, false);
		table1.disableRow(k, false);
		table2.disableCol(k, false);
		table2.disableRow(k, false);
		GRAPH.leave(k);
		//}
	}
	//Visualize {
	context.footer[0] = "Result:";
	context.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (FloatEquals(Distance[i][j], FLT_MAX)) {
				context.footer.push_back(TextFormat("No path from %d to %d", i, j));
				continue;
			}
			if (i != j) {
				context.footer.push_back(TextFormat("Path from %d to %d: ", i, j));
				for (int tmp = i; tmp != j; tmp = Path[tmp][j]) {
					context.footer.back().append(TextFormat("%d -> ", tmp));
				}
				context.footer.back().append(TextFormat("%d, D[%d][%d] = %g", j, i, j, Distance[i][j]));
			}
		}
		context.footer.push_back("");
	}
	TABLE.tables[0] = context;
	TABLE.tables[1] = table1;
	TABLE.tables[2] = table2;
	PSEUDO.reset();
	PSEUDO.highlight(17, VIS_Pseudo_Color_LightLime);
	player.push();
	//}

	for (int i = 0; i < n; i++) {
		delete[] Distance[i];
		delete[] Path[i];
	}
	delete[] Distance;
	delete[] Path;
	table1.clear();
	table2.clear();
	context.clear();
	return 0;
}

int BellmanFord(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src) {
	//Init Visualizer {
	player.init();
	VIS::Table table;
	table.init(n, "Bellman - Ford Algorithm");
	VIS::TableRow row;
	PSEUDO.codes[0] = "function BellmanFord(Graph, source)";
	PSEUDO.codes[1] = "";
	PSEUDO.codes[2] = "    for each vertex v in Graph.Vertices:";
	PSEUDO.codes[3] = "        dist[v] <- INF";
	PSEUDO.codes[4] = "        previous[v] <- null";
	PSEUDO.codes[5] = "    dist[source] <- 0";
	PSEUDO.codes[6] = "";
	PSEUDO.codes[7] = "    for i from 1 to size(Graph.Vertices) - 1:";
	PSEUDO.codes[8] = "        for each edge (u, v) with weight w in Graph.Edges:";
	PSEUDO.codes[9] = "            if dist[u] + w < dist[v]:";
	PSEUDO.codes[10] = "                dist[v] <- dist[u] + w";
	PSEUDO.codes[11] = "                previous[v] <- u";
	PSEUDO.codes[12] = "";
	PSEUDO.codes[13] = "    for each edge (u, v) with weight w in Graph.Edges:";
	PSEUDO.codes[14] = "        if dist[u] + w < dist[v]:";
	PSEUDO.codes[15] = "            error \"Graph contains a negative-weight cycle\"";
	PSEUDO.codes[16] = "";
	PSEUDO.codes[17] = "    return dist[], previous[]";
	//}

	float* Distance = new float[n]; //Distance from src to each nodes
	int* Previous = new int[n]; //The previous node of the shortest path

	for (int i = 0; i < n; i++) {
		Distance[i] = FLT_MAX;
		Previous[i] = -1;

		//Visualize {
		table.colHeaders[i] = TextFormat("%d", i);
		row[i].text = printNode(Distance[i], false, Previous[i], false);
		GRAPH.updateVertex(i, FLT_MAX);
		//}
	}
	//Visualize {
	table.title = "Bellman - Ford Algorithm";
	table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table.footer.push_back("Initialize");
	table.rowHeaders[0] = "Init";
	table.data[0] = row;
	TABLE.tables.push_back(table);
	PSEUDO.highlight(2, VIS_Pseudo_Color_LightBlue);
	PSEUDO.highlight(3, VIS_Pseudo_Color_LightBlue);
	PSEUDO.highlight(4, VIS_Pseudo_Color_LightBlue);
	player.push();
	PSEUDO.reset();
	//}

	Distance[src] = 0;

	//Visualize {
	row[src].text = printNode(Distance[src], false, Previous[src], false);
	table.data[0] = row;
	table.select(0, src);
	TABLE.tables[0] = table;
	GRAPH.updateVertex(src, 0);
	PSEUDO.highlight(5, VIS_Pseudo_Color_LightGreen);
	player.push();
	table.deselect(0, src);
	PSEUDO.reset();
	//}

	for (int k = 1; k <= n - 1; k++) {

		//Visualize {
		table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
		table.footer[0] = TextFormat("Iteration %d", k);
		table.rowHeaders[k] = TextFormat("Iter %d", k);
		table.data[k] = row;
		TABLE.tables[0] = table;
		PSEUDO.highlight(7, VIS_Pseudo_Color_LightPink);
		player.push();
		PSEUDO.reset();
		//}

		for (int i = 0; i < n; i++) {

			//Visualize {
			GRAPH.visit(i);
			//}

			for (int j = 0; j < n; j++) {
				if (!FloatEquals(graph[i][j], 0)) {
					//Visualize {
					table.footer[0] = TextFormat("Check if D[%d] + %s < D[%d]", i, FloatEquals(graph[i][j], FLT_MAX) ? "INF" : TextFormat("%g", graph[i][j]), j);
					table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
					table.data[k] = row;
					table.select(k, j);
					TABLE.tables[0] = table;
					GRAPH.visit(j, i);
					PSEUDO.highlight(7, VIS_Pseudo_Color_LightPink);
					PSEUDO.highlight(8, VIS_Pseudo_Color_LightOrange);
					PSEUDO.highlight(9, VIS_Pseudo_Color_LightYellow);
					player.push();
					table.deselect(k, j);
					PSEUDO.reset();
					//}

					if (Distance[j] > Distance[i] + graph[i][j]) {
						Distance[j] = Distance[i] + graph[i][j];
						Previous[j] = i;
						//Visualize {
						table.footer[0] = TextFormat("D[%d] + %s < D[%d], update D[%d] = %g, prev[%d] = %d", i, FloatEquals(graph[i][j], FLT_MAX) ? "INF" : TextFormat("%g", graph[i][j]), j, j, Distance[j], j, i);
						table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
						row[j].text = printNode(Distance[j], false, Previous[j], false);
						table.data[k] = row;
						table.patch(k, j);
						TABLE.tables[0] = table;
						GRAPH.visit(j, i, Distance[j]);
						PSEUDO.highlight(7, VIS_Pseudo_Color_LightPink);
						PSEUDO.highlight(8, VIS_Pseudo_Color_LightOrange);
						PSEUDO.highlight(10, VIS_Pseudo_Color_LightBlue);
						PSEUDO.highlight(11, VIS_Pseudo_Color_LightBlue);
						player.push();
						GRAPH.leave(j, i, Distance[j]);
						table.depatch(k, j);
						PSEUDO.reset();
						//}
					}
					GRAPH.leave(j, i, Distance[j]);
				}
			}
			//Visualize {
			GRAPH.leave(i);
			//}
		}
	}

	//Visualize {
	table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table.footer[0] = "Check for negative cycle";
	table.rowHeaders[n] = "Check";
	table.data[n] = row;
	TABLE.tables[0] = table;
	PSEUDO.highlight(13, VIS_Pseudo_Color_LightPink);
	PSEUDO.highlight(14, VIS_Pseudo_Color_LightYellow);
	player.push();
	PSEUDO.reset();

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (!FloatEquals(graph[i][j], 0)) {
				if (Distance[j] > Distance[i] + graph[i][j]) {
					//Visualize {
					table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.773f, 0.067f, 0.384f, 1.00f }));
					table.footer[0] = TextFormat("Negative Cycle Detected: D[%d] > D[%d] + %g", j, i, graph[i][j]);
					row[i].text = printNode(Distance[i], false, Previous[i], false);
					row[j].text = printNode(Distance[j], false, Previous[j], false);
					table.rowHeaders[n] = "Result";
					table.data[n] = row;
					table.mark(n, i);
					table.mark(n, j);
					TABLE.tables[0] = table;
					PSEUDO.highlight(13, VIS_Pseudo_Color_LightPink);
					PSEUDO.highlight(15, VIS_Pseudo_Color_LightRed);
					player.push();
					//}
					return 1;
				}
			}
		}
	}

	//Visualize {
	table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
	table.footer[0] = TextFormat("Result");
	table.rowHeaders[n] = "Result";
	table.data[n] = row;
	for (int i = 0; i < n; i++) {
		table.patch(n, i);
	}
	TABLE.tables[0] = table;
	PSEUDO.highlight(17, VIS_Pseudo_Color_LightLime);
	player.push();
	//}

	delete[] Distance;
	delete[] Previous;
	row.clear();
	table.clear();
	return 0;
}

struct edge {
	int u, v;
	float w;
};

struct Dsu { // Disjoint Set Union
	std::vector<int> parent;

	Dsu(int n) {
		parent.resize((size_t)n + 5, 0);
		for (int i = 1; i <= n; i++) {
			parent[i] = i;
		}
	}

	int find(int u) {
		if (parent[u] == u) {
			return u;
		}
		return parent[u] = find(parent[u]);
	}

	bool join(int u, int v) {
		u = find(u);
		v = find(v);
		if (u == v) {
			return false;
		}
		parent[u] = v;
		return true;
	}

};

bool compareEdges(const edge& e1, const edge& e2) {
	return e1.w < e2.w;
}

bool edgeExists(std::vector<edge>& edgelist, int u, int v) {
	for (int i = 0; i < edgelist.size(); i++) {
		if (edgelist[i].u == u && edgelist[i].v == v) {
			return true;
		}
		if (edgelist[i].u == v && edgelist[i].v == u) {
			return true;
		}
	}
	return false;
}

int Kruskal(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n) {
	//Init Visualizer {
	player.init();
	VIS::Table table;
	table.title = "Kruskal Algorithm";
	table.footer.push_back("Edge List");
	table.rowHeaders[0] = "Edge";
	table.rowHeaders[1] = "Weight";
	VIS::TableRow row1, row2;
	PSEUDO.codes[0] = "function Kruskal(Graph):";
	PSEUDO.codes[1] = "    let E be a set of edges in Graph";
	PSEUDO.codes[2] = "    let V be a set of vertices in Graph";
	PSEUDO.codes[3] = "    let T be a set of edges, initially empty";
	PSEUDO.codes[4] = "    sort E in non-decreasing order by weight";
	PSEUDO.codes[5] = "";
	PSEUDO.codes[6] = "    for each vertex v in V:";
	PSEUDO.codes[7] = "        make-set(v)";
	PSEUDO.codes[8] = "";
	PSEUDO.codes[9] = "    for each edge (u, v) in E, in non-decreasing order by weight:";
	PSEUDO.codes[10] = "        if find-set(u) != find-set(v):";
	PSEUDO.codes[11] = "            add (u, v) to T";
	PSEUDO.codes[12] = "            union(u, v)";
	PSEUDO.codes[13] = "";
	PSEUDO.codes[14] = "    return T";
	if (GRAPH.isDirected) {
		PSEUDO.codes[15] = "";
		PSEUDO.codes[16] = "Warning: Graph is directed, result may not be correct";
	}
	//}

	std::vector<edge> edgelist;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (!FloatEquals(graph[i][j], 0)) {
				if (!GRAPH.isDirected && edgeExists(edgelist, i, j))
					continue;
				edgelist.push_back({ i, j, graph[i][j] });

				//Visualize {
				table.colHeaders[(int)table.colHeaders.size()] = TextFormat("%d", edgelist.size());
				row1[(int)edgelist.size() - 1].text = TextFormat("(%d;%d)", i, j);
				row2[(int)edgelist.size() - 1].text = TextFormat("%g", graph[i][j]);
				//}
			}
		}
	}

	//Visualize {
	table.cols = (int)edgelist.size();
	table.data[0] = row1;
	table.data[1] = row2;
	TABLE.tables.push_back(table);
	PSEUDO.highlight(1, VIS_Pseudo_Color_LightBlue);
	PSEUDO.highlight(2, VIS_Pseudo_Color_LightBlue);
	PSEUDO.highlight(3, VIS_Pseudo_Color_LightBlue);
	if (GRAPH.isDirected)
		PSEUDO.highlight(16, VIS_Pseudo_Color_LightYellow);
	player.push();
	PSEUDO.reset();
	//}

	std::sort(edgelist.begin(), edgelist.end(), compareEdges);

	//Visualize
	for (int i = 0; i < edgelist.size(); i++) {
		table.data[0][i].text = TextFormat("(%d;%d)", edgelist[i].u, edgelist[i].v);
		table.data[1][i].text = TextFormat("%g", edgelist[i].w);
	}
	table.footer[0] = "Sort Edges";
	table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
	TABLE.tables[0] = table;
	PSEUDO.highlight(4, VIS_Pseudo_Color_LightGreen);
	if (GRAPH.isDirected)
		PSEUDO.highlight(16, VIS_Pseudo_Color_LightYellow);
	player.push();
	PSEUDO.reset();
	//}

	Dsu dsu = Dsu(n);

	//Visualize {
	table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table.footer[0] = "Initialize Disjoint Set Union";
	TABLE.tables[0] = table;
	PSEUDO.highlight(6, VIS_Pseudo_Color_LightPink);
	PSEUDO.highlight(7, VIS_Pseudo_Color_LightBlue);
	if (GRAPH.isDirected)
		PSEUDO.highlight(16, VIS_Pseudo_Color_LightYellow);
	player.push();
	PSEUDO.reset();

	float cost = 0;
	int ce = 0;
	int i = 0;
	for (i = 0; i < n - 1 && edgelist.size()>0;) {
		edge e = edgelist.front();
		edgelist.erase(edgelist.begin());

		//Visualize {
		table.footer[0] = TextFormat("Check If Edge %d: (%d;%d) makes a cycle", ce, e.u, e.v);
		table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
		TABLE.tables[0] = table;
		GRAPH.select(e.u);
		GRAPH.select(e.v, e.u);
		PSEUDO.highlight(9, VIS_Pseudo_Color_LightPink);
		PSEUDO.highlight(10, VIS_Pseudo_Color_LightOrange);
		if (GRAPH.isDirected)
			PSEUDO.highlight(16, VIS_Pseudo_Color_LightYellow);
		player.push();
		PSEUDO.reset();
		//}

		if (!dsu.join(e.u, e.v)) {
			//Visualize {
			table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.773f, 0.067f, 0.384f, 1.00f }));
			table.footer[0] = TextFormat("Removed Edge %d: (%d;%d)", ce, e.u, e.v);
			table.mark(0, ce);
			table.mark(1, ce);
			TABLE.tables[0] = table;
			GRAPH.deselect(e.u);
			GRAPH.deselect(e.v, e.u);
			GRAPH.disable(e.v, e.u);
			PSEUDO.highlight(9, VIS_Pseudo_Color_LightPink);
			PSEUDO.highlight(10, VIS_Pseudo_Color_LightOrange);
			if (GRAPH.isDirected)
				PSEUDO.highlight(16, VIS_Pseudo_Color_LightYellow);
			player.push();
			PSEUDO.reset();
			//}

			ce++;
			continue;
		}

		cost += e.w;
		i++;

		//Visualize {
		table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		table.footer[0] = TextFormat("Added Edge %d: (%d;%d)", ce, e.u, e.v);
		table.patch(0, ce);
		table.patch(1, ce);
		TABLE.tables[0] = table;
		GRAPH.deselect(e.u);
		GRAPH.deselect(e.v, e.u);
		GRAPH.visit(e.u);
		GRAPH.visit(e.v, e.u);
		PSEUDO.highlight(9, VIS_Pseudo_Color_LightPink);
		PSEUDO.highlight(11, VIS_Pseudo_Color_LightLime);
		PSEUDO.highlight(12, VIS_Pseudo_Color_LightLime);
		if (GRAPH.isDirected)
			PSEUDO.highlight(16, VIS_Pseudo_Color_LightYellow);
		player.push();
		PSEUDO.reset();
		//}

		ce++;
	}

	//Visualize
	if (i < n - 1) {
		table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.773f, 0.067f, 0.384f, 1.00f }));
		table.footer[0] = TextFormat("Graph is not connected");
		PSEUDO.highlight(14, VIS_Pseudo_Color_LightRed);
	}
	else {
		table.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		table.footer[0] = TextFormat("Result: %g", cost);
		PSEUDO.highlight(14, VIS_Pseudo_Color_LightLime);
	}
	for (i = 0; i < edgelist.size(); i++) {
		table.mark(0, ce + i);
		table.mark(1, ce + i);
		GRAPH.disable(edgelist[i].v, edgelist[i].u);
	}
	TABLE.tables[0] = table;
	if (GRAPH.isDirected)
		PSEUDO.highlight(16, VIS_Pseudo_Color_LightYellow);
	player.push();
	//}

	table.clear();
	row1.clear();
	row2.clear();
	edgelist.clear();
	dsu.parent.clear();
	return 0;
}

int Prim(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src) {
	//Init Visualizer {
	player.init();
	VIS::Table table1, table2;
	table1.init(n, "Prim Algorithm");
	table2.title = "Prim Algorithm";
	PSEUDO.codes[0] = "function Prim(Graph, source):";
	PSEUDO.codes[1] = "    PQ <- empty priority queue, mst <- empty set";
	PSEUDO.codes[2] = "";
	PSEUDO.codes[3] = "    add source to mst";
	PSEUDO.codes[4] = "    enqueue all edges connected to source into PQ";
	PSEUDO.codes[5] = "";
	PSEUDO.codes[6] = "    while PQ is not empty or mst does not contain all vertices:";
	PSEUDO.codes[7] = "        u <- vertex target of min edge in PQ";
	PSEUDO.codes[8] = "        dequeue min edge from PQ";
	PSEUDO.codes[9] = "        if u is already in mst:";
	PSEUDO.codes[10] = "            continue";
	PSEUDO.codes[11] = "        add u to mst";
	PSEUDO.codes[12] = "        for each edge (u, v) connected to u:";
	PSEUDO.codes[13] = "            if v is not in mst:";
	PSEUDO.codes[14] = "                enqueue (u, v) into PQ";
	PSEUDO.codes[15] = "";
	PSEUDO.codes[16] = "    return mst";
	if (GRAPH.isDirected) {
		PSEUDO.codes[17] = "";
		PSEUDO.codes[18] = "Warning: Graph is directed, result may not be correct";
	}
	//}

	std::priority_queue<std::pair<float, std::pair<int, int>>, std::vector<std::pair<float, std::pair<int, int>>>, std::greater<std::pair<float, std::pair<int, int>>>> pq;
	std::priority_queue<std::pair<float, std::pair<int, int>>, std::vector<std::pair<float, std::pair<int, int>>>, std::greater<std::pair<float, std::pair<int, int>>>> temp;
	float* Distance = new float[n]; //Distance from src to each nodes
	int* Path = new int[n]; //The previous node of the shortest path
	bool* Marked = new bool[n]; //Marked[i] = true if node i is in the MST
	for (int i = 0; i < n; i++) {
		Distance[i] = FLT_MAX;
		Path[i] = -1;
		Marked[i] = false;
		//Visualize {
		table1.colHeaders[i] = TextFormat("%d", i);
		table1.data[0][i].text = "null";
		table1.data[1][i].text = "INF";
		//}
	}

	//Visualize {
	table1.rowHeaders[0] = "Path";
	table1.rowHeaders[1] = "Cost";
	table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table1.footer.push_back("Init");
	TABLE.tables.push_back(table1);
	PSEUDO.highlight(1, VIS_Pseudo_Color_LightBlue);
	if (GRAPH.isDirected)
		PSEUDO.highlight(18, VIS_Pseudo_Color_LightYellow);
	player.push();
	PSEUDO.reset();
	//}

	Distance[src] = 0;
	Marked[src] = true;
	for (int v = 0; v < n; v++) {
		if (!FloatEquals(graph[src][v], 0) && !Marked[v]) {
			pq.push(std::make_pair(graph[src][v], std::make_pair(src, v)));
		}
	}
	//Visualize {
	table1.footer[0] = TextFormat("Start from %d, mst = {%d}\nAdded all edges connected to %d into PQ", src, src, src);
	table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
	table1.data[0][src].text = TextFormat("%d", src);
	table1.data[1][src].text = "0";
	TABLE.tables[0] = table1;
	temp = pq;
	for (int i = 0; i < temp.size(); i++) {
		table2.colHeaders[i] = TextFormat("%d", i);
		table2.data[0][i].text = TextFormat("(%d;%d)", pq.top().second.first, pq.top().second.second);
		table2.data[1][i].text = TextFormat("%g", pq.top().first);
		pq.pop();
	}
	pq = temp;
	table2.rowHeaders[0] = "Edge";
	table2.rowHeaders[1] = "Weight";
	table2.footer.push_back("PQ");
	table2.cols = (int)pq.size();
	if (pq.size() == 0)
		table2.footer[0] = "PQ is empty";
	TABLE.tables.push_back(table2);
	GRAPH.visit(src, -1, 0);
	PSEUDO.highlight(3, VIS_Pseudo_Color_LightPink);
	PSEUDO.highlight(4, VIS_Pseudo_Color_LightOrange);
	if (GRAPH.isDirected)
		PSEUDO.highlight(18, VIS_Pseudo_Color_LightYellow);
	player.push();
	PSEUDO.reset();
	//}


	float mst_cost = 0, num_edges = 0;
	bool complete = false;
	while (!pq.empty()) {
		int par = pq.top().second.first, u = pq.top().second.second;
		float w = pq.top().first;
		pq.pop();

		//Visualize {
		table1.footer[0] = TextFormat("Edge (%d,%d),w = %g is removed from PQ\nCheck if %d is in MST", par, u, w, u);
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
		temp = pq;
		for (int i = 0; i < temp.size(); i++) {
			table2.colHeaders[i] = TextFormat("%d", i);
			table2.data[0][i].text = TextFormat("(%d;%d)", pq.top().second.first, pq.top().second.second);
			table2.data[1][i].text = TextFormat("%g", pq.top().first);
			pq.pop();
		}
		pq = temp;
		table2.cols = (int)pq.size();
		if (pq.size() == 0)
			table2.footer[0] = "PQ is empty";
		else
			table2.footer[0] = "PQ";
		TABLE.tables[0] = table1;
		TABLE.tables[1] = table2;
		GRAPH.select(u, par);
		PSEUDO.highlight(6, VIS_Pseudo_Color_LightPink);
		PSEUDO.highlight(7, VIS_Pseudo_Color_LightBlue);
		PSEUDO.highlight(8, VIS_Pseudo_Color_LightRed);
		PSEUDO.highlight(9, VIS_Pseudo_Color_LightYellow);
		if (GRAPH.isDirected)
			PSEUDO.highlight(18, VIS_Pseudo_Color_LightYellow);
		player.push();
		PSEUDO.reset();
		GRAPH.deselect(u, par);
		//}

		if (Marked[u]) {
			//Visualize {
			table1.footer[0] = TextFormat("%d is already in MST\nSkip", u);
			table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.773f, 0.067f, 0.384f, 1.00f }));
			TABLE.tables[0] = table1;
			PSEUDO.highlight(6, VIS_Pseudo_Color_LightPink);
			PSEUDO.highlight(10, VIS_Pseudo_Color_LightMagenta);
			GRAPH.disable(u, par);
			if (GRAPH.isDirected)
				PSEUDO.highlight(18, VIS_Pseudo_Color_LightYellow);
			player.push();
			PSEUDO.reset();
			continue;
		}

		Marked[u] = true;
		Distance[u] = w;
		Path[u] = par;
		mst_cost += Distance[u];
		for (int v = 0; v < n; v++) {
			if (!FloatEquals(graph[u][v], 0) && !Marked[v]) {
				pq.push(std::make_pair(graph[u][v], std::make_pair(u, v)));
			}
		}
		//Visualize {
		table1.data[0][u].text = TextFormat("%d", Path[u]);
		table1.data[1][u].text = TextFormat("%g", Distance[u]);
		table1.footer[0] = TextFormat("%d is not in MST. Added %d to MST\nAdded all edges connected to %d into PQ", u, u, u);
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		temp = pq;
		for (int i = 0; i < temp.size(); i++) {
			table2.colHeaders[i] = TextFormat("%d", i);
			table2.data[0][i].text = TextFormat("(%d;%d)", pq.top().second.first, pq.top().second.second);
			table2.data[1][i].text = TextFormat("%g", pq.top().first);
			pq.pop();
		}
		pq = temp;
		table2.cols = (int)pq.size();
		if (pq.size() == 0)
			table2.footer[0] = "PQ is empty";
		else
			table2.footer[0] = "PQ";
		TABLE.tables[0] = table1;
		TABLE.tables[1] = table2;
		GRAPH.visit(u, par, Distance[u]);
		PSEUDO.highlight(6, VIS_Pseudo_Color_LightPink);
		PSEUDO.highlight(11, VIS_Pseudo_Color_LightLime);
		PSEUDO.highlight(12, VIS_Pseudo_Color_LightLime);
		PSEUDO.highlight(13, VIS_Pseudo_Color_LightOrange);
		PSEUDO.highlight(14, VIS_Pseudo_Color_LightBlue);
		if (GRAPH.isDirected)
			PSEUDO.highlight(18, VIS_Pseudo_Color_LightYellow);
		player.push();
		PSEUDO.reset();
		//}

		++num_edges;
		if (num_edges == n - 1) {
			complete = true;
			//Visualize {
			table1.footer[0] = TextFormat("MST is completed");
			table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
			TABLE.tables[0] = table1;
			PSEUDO.highlight(6, VIS_Pseudo_Color_LightPink);
			if (GRAPH.isDirected)
				PSEUDO.highlight(18, VIS_Pseudo_Color_LightYellow);
			player.push();
			PSEUDO.reset();
			//}
			break;
		}
	}

	if (complete == false) {
		//Visualize {
		table1.footer[0] = TextFormat("Graph is not connected");
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.773f, 0.067f, 0.384f, 1.00f }));
		TABLE.tables[0] = table1;
		PSEUDO.highlight(6, VIS_Pseudo_Color_LightPink);
		if (GRAPH.isDirected)
			PSEUDO.highlight(18, VIS_Pseudo_Color_LightYellow);
		player.push();
		//}
	}

	//Visualize {
	table1.footer[0] = TextFormat("Result: %g", mst_cost);
	table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
	TABLE.tables[0] = table1;
	TABLE.tables.erase(TABLE.tables.begin() + 1, TABLE.tables.end());
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (Path[i] != j)
				GRAPH.disable(j, i);
		}
	}
	if (complete)
		PSEUDO.highlight(16, VIS_Pseudo_Color_LightLime);
	else
		PSEUDO.highlight(16, VIS_Pseudo_Color_LightRed);
	if (GRAPH.isDirected)
		PSEUDO.highlight(18, VIS_Pseudo_Color_LightYellow);
	player.push();
	//}

	delete[] Distance;
	delete[] Path;
	delete[] Marked;
	table1.clear();
	table2.clear();
	while (!pq.empty()) {
		pq.pop();
		temp.pop();
	}
	return 0;
}

bool isInQueue(std::queue<int> Queue, int node) {
	while (!Queue.empty()) {
		if (Queue.front() == node)
			return true;
		Queue.pop();
	}
	return false;
}

int BreadthFirstSearch(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src) {
	//Init Visualizer {
	player.init();
	std::string output = "";
	VIS::Table table1, table2;
	table1.init(n, "Breadth First Search");
	table2.init(n, "Breadth First Search");
	table1.rowHeaders[0] = "Visited";
	table2.rowHeaders[0] = "Queue";
	table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table2.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table1.footer.push_back("Init");
	table2.footer.push_back(" ");
	PSEUDO.codes[0] = "function BFS(Graph, source):";
	PSEUDO.codes[1] = "    Visited <- empty set, Q <- empty queue";
	PSEUDO.codes[2] = "";
	PSEUDO.codes[3] = "    Q.enqueue(source)";
	PSEUDO.codes[4] = "    while Q is not empty:";
	PSEUDO.codes[5] = "        u <- Q.dequeue()";
	PSEUDO.codes[6] = "        add u to Visited";
	PSEUDO.codes[7] = "";
	PSEUDO.codes[8] = "        for each edge (u, v) connected to u:";
	PSEUDO.codes[9] = "            if v is not in Visited and v is not in Q:";
	PSEUDO.codes[10] = "                Q.enqueue(v)";
	PSEUDO.codes[11] = "";
	PSEUDO.codes[12] = "    return Visited";
	//}

	bool* Visited = new bool[n];
	int* Previous = new int[n];
	std::queue<int> Queue, temp;

	for (int i = 0; i < n; i++) {
		Visited[i] = false;
		Previous[i] = -1;

		//Visualize {
		table1.colHeaders[i] = TextFormat("%d", i);
		table2.colHeaders[i] = TextFormat("%d", i);
		table1.data[0][i].text = "F";
		table2.data[0][i].text = "-";
		table2.mark(0, i);
		//}
	}
	//Visualize {
	TABLE.tables.push_back(table1);
	TABLE.tables.push_back(table2);
	PSEUDO.highlight(1, VIS_Pseudo_Color_LightBlue);
	player.push();
	PSEUDO.reset();
	//}

	Queue.push(src);

	//Visualize {
	table1.footer[0] = TextFormat("Start from %d", src);
	table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
	table2.data[0][0].text = TextFormat("%d", src);
	table2.unmark(0, 0);
	table2.patch(0, 0);
	TABLE.tables[0] = table1;
	TABLE.tables[1] = table2;
	PSEUDO.highlight(3, VIS_Pseudo_Color_LightPink);
	player.push();
	PSEUDO.reset();
	//}


	while (!Queue.empty()) {
		int u = Queue.front();
		Queue.pop();
		Visited[u] = true;

		//Visualize {
		output.append(TextFormat("%d ", u));
		GRAPH.deselect(u, Previous[u]);
		GRAPH.visit(u, Previous[u]);
		temp = Queue;
		for (int i = 0; i < temp.size(); i++) {
			table2.unmark(0, i);
			table2.patch(0, i);
			table2.data[0][i].text = TextFormat("%d", Queue.front());
			Queue.push(Queue.front());
			Queue.pop();
		}
		for (int i = (int)temp.size(); i < n; i++) {
			table2.data[0][i].text = "-";
			table2.mark(0, i);
		}
		Queue = temp;
		table1.footer[0] = TextFormat("Pop %d from Queue, added %d to Visited", u, u);
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		table1.data[0][u].text = "T";
		table1.patch(0, u);
		table2.footer[0] = output;
		TABLE.tables[0] = table1;
		TABLE.tables[1] = table2;
		PSEUDO.highlight(4, VIS_Pseudo_Color_LightPink);
		PSEUDO.highlight(5, VIS_Pseudo_Color_LightOrange);
		PSEUDO.highlight(6, VIS_Pseudo_Color_LightLime);
		player.push();
		PSEUDO.reset();
		//}

		int ce = 0;
		for (int i = 0; i < n; i++) {
			if (!FloatEquals(graph[u][i], 0)) {
				if (!Visited[i] && !isInQueue(Queue, i)) {
					Queue.push(i);
					Previous[i] = u;
					GRAPH.select(i, u);
					ce++;
				}
			}
		}

		//Visualize {
		temp = Queue;
		for (int i = 0; i < temp.size(); i++) {
			table2.unmark(0, i);
			table2.patch(0, i);
			table2.data[0][i].text = TextFormat("%d", Queue.front());
			Queue.push(Queue.front());
			Queue.pop();
		}
		for (int i = (int)temp.size(); i < n; i++) {
			table2.data[0][i].text = "-";
			table2.mark(0, i);
		}
		Queue = temp;
		if (ce > 0) {
			table1.footer[0] = TextFormat("Added %d edges connected to %d into Stack", ce, u);
			table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		}
		else {
			table1.footer[0] = TextFormat("%d has no edges connected to unvisited nodes", u);
			table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
		}
		TABLE.tables[0] = table1;
		TABLE.tables[1] = table2;
		PSEUDO.highlight(4, VIS_Pseudo_Color_LightPink);
		PSEUDO.highlight(8, VIS_Pseudo_Color_LightTeal);
		PSEUDO.highlight(9, VIS_Pseudo_Color_LightYellow);
		PSEUDO.highlight(10, VIS_Pseudo_Color_LightGreen);
		player.push();
		PSEUDO.reset();
		//}
	}

	//Visualize {
	bool flag = false;
	for (int i = 0; i < n; i++) {
		if (!Visited[i]) {
			flag = true;
			break;
		}
	}
	if (flag) {
		table1.footer[0] = TextFormat("Graph is not connected");
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.773f, 0.067f, 0.384f, 1.00f }));
		PSEUDO.highlight(12, VIS_Pseudo_Color_LightRed);
	}
	else {
		table1.footer[0] = TextFormat("Result: %s", output.c_str());
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		PSEUDO.highlight(12, VIS_Pseudo_Color_LightGreen);
	}
	TABLE.tables[0] = table1;
	TABLE.tables[1] = table2;
	player.push();
	//}

	table1.clear();
	table2.clear();
	output.clear();
	delete[] Visited;
	delete[] Previous;
	return 0;
}

bool isInStack(std::stack<int> Stack, int node) {
	while (!Stack.empty()) {
		if (Stack.top() == node)
			return true;
		Stack.pop();
	}
	return false;
}

int DepthFirstSearch(Player::Player& player, float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int n, int src) {
	//Init Visualizer {
	player.init();
	std::string output = "";
	VIS::Table table1, table2;
	table1.init(n, "Depth First Search");
	table2.init(n, "Depth First Search");
	table1.rowHeaders[0] = "Visited";
	table2.rowHeaders[0] = "Stack";
	table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table2.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
	table1.footer.push_back("Init");
	table2.footer.push_back(" ");
	PSEUDO.codes[0] = "function DFS(Graph, source):";
	PSEUDO.codes[1] = "    Visited <- empty set, S <- empty stack";
	PSEUDO.codes[2] = "";
	PSEUDO.codes[3] = "    S.push(source)";
	PSEUDO.codes[4] = "    while S is not empty:";
	PSEUDO.codes[5] = "        u <- S.top()";
	PSEUDO.codes[6] = "        S.pop()";
	PSEUDO.codes[7] = "        add u to Visited";
	PSEUDO.codes[8] = "";
	PSEUDO.codes[9] = "        for each edge (u, v) connected to u:";
	PSEUDO.codes[10] = "            if v is not in Visited and v is not in S:";
	PSEUDO.codes[11] = "                S.push(v)";
	PSEUDO.codes[12] = "";
	PSEUDO.codes[13] = "    return Visited";
	//}

	bool* Visited = new bool[n];
	int* Previous = new int[n];
	std::stack<int> Stack, temp;

	for (int i = 0; i < n; i++) {
		Visited[i] = false;
		Previous[i] = -1;

		//Visualize {
		table1.colHeaders[i] = TextFormat("%d", i);
		table2.colHeaders[i] = TextFormat("%d", i);
		table1.data[0][i].text = "F";
		table2.data[0][i].text = "-";
		table2.mark(0, i);
		//}
	}

	//Visualize {
	TABLE.tables.push_back(table1);
	TABLE.tables.push_back(table2);
	PSEUDO.highlight(1, VIS_Pseudo_Color_LightBlue);
	player.push();
	PSEUDO.reset();
	//}

	Stack.push(src);

	//Visualize {
	table1.footer[0] = TextFormat("Start from %d", src);
	table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
	table2.data[0][0].text = TextFormat("%d", src);
	table2.unmark(0, 0);
	table2.patch(0, 0);
	TABLE.tables[0] = table1;
	TABLE.tables[1] = table2;
	PSEUDO.highlight(3, VIS_Pseudo_Color_LightPink);
	player.push();
	PSEUDO.reset();
	//}

	while (!Stack.empty()) {
		int u = Stack.top();
		Stack.pop();
		Visited[u] = true;

		//Visualize {
		output.append(TextFormat("%d ", u));
		GRAPH.deselect(u, Previous[u]);
		GRAPH.visit(u, Previous[u]);
		temp = Stack;
		for (int i = (int)temp.size() - 1; i >= 0; i--) {
			table2.unmark(0, i);
			table2.patch(0, i);
			table2.data[0][i].text = TextFormat("%d", Stack.top());
			Stack.pop();
		}
		for (int i = (int)temp.size(); i < n; i++) {
			table2.data[0][i].text = "-";
			table2.mark(0, i);
		}
		Stack = temp;
		table1.footer[0] = TextFormat("%d is popped from Stack, added %d to Visited", u, u);
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		table1.data[0][u].text = "T";
		table1.patch(0, u);
		table2.footer[0] = output;
		TABLE.tables[0] = table1;
		TABLE.tables[1] = table2;
		PSEUDO.highlight(4, VIS_Pseudo_Color_LightPink);
		PSEUDO.highlight(5, VIS_Pseudo_Color_LightBlue);
		PSEUDO.highlight(6, VIS_Pseudo_Color_LightRed);
		PSEUDO.highlight(7, VIS_Pseudo_Color_LightLime);
		player.push();
		table1.depatch(0, u);
		PSEUDO.reset();
		//}

		int ce = 0;
		for (int v = n - 1; v >= 0; v--) {
			if (!FloatEquals(graph[u][v], 0)) {
				if (!Visited[v] && !isInStack(Stack, v)) {
					Stack.push(v);
					Previous[v] = u;
					GRAPH.select(v, u);
					ce++;
				}
			}
		}
		//Visualize {
		temp = Stack;
		for (int i = (int)temp.size() - 1; i >= 0; i--) {
			table2.unmark(0, i);
			table2.patch(0, i);
			table2.data[0][i].text = TextFormat("%d", Stack.top());
			Stack.pop();
		}
		for (int i = (int)temp.size(); i < n; i++) {
			table2.data[0][i].text = "-";
			table2.mark(0, i);
		}
		Stack = temp;
		if (ce > 0) {
			table1.footer[0] = TextFormat("Added %d edges connected to %d into Stack", ce, u);
			table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		}
		else {
			table1.footer[0] = TextFormat("%d has no edges connected to unvisited nodes", u);
			table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.86f,0.86f,0.67f,1.0f }));
		}
		TABLE.tables[0] = table1;
		TABLE.tables[1] = table2;
		PSEUDO.highlight(4, VIS_Pseudo_Color_LightPink);
		PSEUDO.highlight(9, VIS_Pseudo_Color_LightTeal);
		PSEUDO.highlight(10, VIS_Pseudo_Color_LightYellow);
		PSEUDO.highlight(11, VIS_Pseudo_Color_LightGreen);
		player.push();
		PSEUDO.reset();
		//}
	}

	//Visualize {
	bool flag = false;
	for (int i = 0; i < n; i++) {
		if (!Visited[i]) {
			flag = true;
			break;
		}
	}
	if (flag) {
		table1.footer[0] = TextFormat("Graph is not connected");
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.773f, 0.067f, 0.384f, 1.00f }));
		PSEUDO.highlight(13, VIS_Pseudo_Color_LightRed);
	}
	else {
		table1.footer[0] = TextFormat("Result: %s", output.c_str());
		table1.FooterColor = rlImGuiColors::Convert(ImVec4({ 0.00f, 0.902f, 0.463f, 1.00f }));
		PSEUDO.highlight(13, VIS_Pseudo_Color_LightGreen);
	}
	TABLE.tables[0] = table1;
	TABLE.tables[1] = table2;
	player.push();
	//}

	table1.clear();
	table2.clear();
	output.clear();
	delete[] Visited;
	delete[] Previous;
	return 0;
}

#endif // SSSPALGO_IMPLEMENTATION