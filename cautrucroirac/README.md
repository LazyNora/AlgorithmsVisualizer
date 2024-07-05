# Documentation

## Purpose
An app for visualizing common graph algorithms.
## Features
- Visualize graph algorithms
- Create your own graphs
- Save and load graphs
## Usage
### Creating a graph
To create a graph, click on the "Input Graph" button. This will open a new window where you can create a graph.

[![Input Graph](https://i.imgur.com/r1pp0E9.gif)](https://i.imgur.com/r1pp0E9.gif)
#### Input format for Adjacency Matrix
```
V
Adjacency Matrix
```
*Example*
```
5
0 7 9 0 3 
7 0 6 0 0 
9 6 0 0 0 
0 0 0 0 7 
3 0 0 7 0 
```
#### Input format for Adjacency List
```
V
for each vertex:
n(v) v1 w1 v2 w2 ...
```
*Example*
```
5
3 1 7 2 9 4 3 
2 0 7 2 6 
2 0 9 1 6 
1 4 7 
2 0 3 3 7 
```
#### Input format for Edge List
```
V E
for each edge:
u v w
```
*Example*
```
5 5
0 1 7
0 2 9
0 4 3
1 2 6
3 4 7
```
==Note:==
- **V** is the number of vertices. 
- **E** is the number of edges.
- **n(v)** is the number of neighbors of vertex v.
- **u** and **v** are the vertices of the edge.
- **w** is the weight of the edge.
- Vertices are numbered from ==*0*== to ==*V-1*==.
- For the **Adjacency Matrix**, the weight of the edge is the value in the matrix.
- For **Directed Graph**, check the **Directed Graph checkbox**.
- The **Output Graph Layout** is mainly specific to whether the graph is a ==Tree== or not, with the <ins>root</ins> being vertex 0;  Although it has a random layout, we advise you not to use it.
- You can adjust the position of all vertices by dragging them on the screen.
### Save & load graph
To save the current graph, click on the "Save File" button. This will open a window where you can save the graph.

[![Save Graph](https://i.imgur.com/VrEZQjU.png)](https://i.imgur.com/VrEZQjU.png)

To load the saved graph, click on the "Open File" button. This will open a new window where you can choose the path to the saved .txt file and load it.

[![Open File](https://i.imgur.com/6OYmrQ7.png)](https://i.imgur.com/6OYmrQ7.png)
### Choose Algorithms
Choose the algorithm you want to run on the graph.

[![Algorithms](https://i.imgur.com/njrXjSA.png)](https://i.imgur.com/njrXjSA.png)
### Navigator
The navigator is used to navigate through the steps of the algorithm.

[![Navigator](https://i.imgur.com/EuO3FR9.png)](https://i.imgur.com/EuO3FR9.png)
### Output
The output is the result of the algorithm.

[![Result](https://i.imgur.com/eoYcuTY.png)](https://i.imgur.com/eoYcuTY.png)
### Psuedocode
The psuedocode is the psuedocode of the algorithm.

[![Psuedocode](https://i.imgur.com/RB3hT78.png)](https://i.imgur.com/RB3hT78.png)

## Libraries
- [raylib](https://www.raylib.com/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [rlImGui](https://github.com/raylib-extras/rlImGui)
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)
## Resources
- Fonts: [tahoma](https://www.fontpalace.com/font-details/tahoma/), [PT Sans](https://fonts.google.com/specimen/PT+Sans)