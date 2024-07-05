#ifndef LUI_H_
#define LUI_H_

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "../raylib/include/raylib.h"
#include "../raylib/include/raymath.h"

#include "../imgui/imgui.h"
#include "../ImGuiFileDialog/ImGuiFileDialog.h"
#include "Algorithms.h"

#include "Visualizer.h"
#include "Randomize.h"

typedef enum Algorithm {
	DIJKSTRA,
	BELLMAN_FORD,
	FLOYD_WARSHALL,
	PRIM,
	KRUSKAL,
	DFS,
	BFS,
	NONE
} Algorithm;

typedef enum InputType {
	ADJACENCY_MATRIX,
	ADJACENCY_LIST,
	EDGE_LIST
} InputType;

void LUI_loadTheme(ImGuiStyle& style);
void LUI_DefaultWindowPos(int location, ImGuiCond cond = 0, ImVec2 pivot = { -1.f, -1.f }); // 0: Top-left, 1: Top-right, 2: Bottom-left, 3: Bottom-right, 4: Center, -1: Top-center, -2: Bottom-center, -3: Left-center, -4: Right-center
void LUI_Navigator(Player::Player& player, float& speed);
void LUI_StartVertexSelect(bool& IsStartVertexActive, int& startVertex, int max, Algorithm algorithm, bool& calculating);
void LUI_SettingMenu(bool& showInputGraph, bool& showSaveMenu, bool& showRandomGraph, bool& showRendererSetting);
void LUI_AlgorithmSelectMenu(bool& IsStartVertexActive, Algorithm& algorithm);
static void LUI_HelpMarker(const char* desc);
static void LUI_Tooltip(const char* desc);
void LUI_SaveMenu(VIS::GraphTracer& Tracer, bool& showSaveMenu);
int LUI_SaveFile(char OutputBuffer[], const char* fileName, char ErrorBuffer[]);
void LUI_RandomGraph(Player::Player& player, float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, bool& showRandomGraph);
void LUI_InputGraph(Player::Player& player, float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, bool& showInputGraph, InputType& InputType, VIS_Graph_Layout& Layout);
void LUI_ConvertTracerToString(char Buffer[], VIS::GraphTracer Tracer, InputType InputType);
int LUI_ParseInputFile(char InputBuffer[], const char* fileName, char ErrorBuffer[]);
int LUI_ParseInput(char InputBuffer[], float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, InputType InputType, char ErrorBuffer[]);
int LUI_ParseAM(char InputBuffer[], float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, char ErrorBuffer[]);
int LUI_ParseAL(char InputBuffer[], float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, char ErrorBuffer[]);
int LUI_ParseEL(char InputBuffer[], float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, char ErrorBuffer[]);

#endif // !LUI_H_

#ifdef LUI_IMPLEMENTATION

void LUI_loadTheme(ImGuiStyle& style) {
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.75f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void LUI_DefaultWindowPos(int location, ImGuiCond cond, ImVec2 pivot) {
	if (location >= 0 && location < 4) {
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, cond, (Vector2Equals({ pivot.x,pivot.y }, { -1.f, -1.f })) ? window_pos_pivot : pivot);
	}
	else if (location == 4) {
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), cond, (Vector2Equals({ pivot.x,pivot.y }, { -1.f, -1.f })) ? ImVec2(0.5f, 0.5f) : pivot);
	}
	else if (location >= -2) {
		location = -location - 1;
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (work_pos.x + work_size.x - PAD) / 2;
		window_pos.y = (location & 1) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = 0.5f;
		window_pos_pivot.y = (location & 1) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, cond, (Vector2Equals({ pivot.x,pivot.y }, { -1.f, -1.f })) ? window_pos_pivot : pivot);
	}
	else {
		location = -location - 1;
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = ((location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD)) / 2;
		window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = 0.5f;
		ImGui::SetNextWindowPos(window_pos, cond, (Vector2Equals({ pivot.x,pivot.y }, { -1.f, -1.f })) ? window_pos_pivot : pivot);
	}
}

void LUI_Navigator(Player::Player& player, float& speed) {
	LUI_DefaultWindowPos(2);
	ImGui::Begin("Navigator", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
	{
		ImGui::Text("Step: %d/%d", player.CurrentStep + 1, player.States.size());
		if (player.CurrentStep == -1)
			ImGui::BeginDisabled();
		ImGui::SameLine();
		if (ImGui::Button("<<")) {
			player.CurrentStep = 0;
			player.load(0);
		}
		LUI_Tooltip("Go to the first step");
		ImGui::SameLine();
		if (ImGui::Button("<")) {
			player.CurrentStep -= 1;
			if (player.CurrentStep < 0)
				player.CurrentStep = 0;
			player.load(player.CurrentStep);
		}
		LUI_Tooltip("Go to the previous step");
		ImGui::SameLine();
		if (player.pause) {
			if (ImGui::Button("Run")) {
				player.pause = false;
				if (player.CurrentStep == player.States.size() - 1)
					player.CurrentStep = 0;
				player.Timer.start(1 / speed);
			}
			LUI_Tooltip("Run the algorithm");
		}
		else {
			if (ImGui::Button("Pause")) {
				player.pause = true;
				if (player.CurrentStep == player.States.size() - 1)
					player.CurrentStep = 0;
			}
			LUI_Tooltip("Pause the algorithm");
		}
		ImGui::SameLine();
		if (ImGui::Button(">")) {
			player.CurrentStep += 1;
			if (player.CurrentStep >= player.States.size())
				player.CurrentStep = (int)player.States.size() - 1;
			player.load(player.CurrentStep);
		}
		LUI_Tooltip("Go to the next step");
		ImGui::SameLine();
		if (ImGui::Button(">>")) {
			player.CurrentStep = (int)player.States.size() - 1;
			player.load((int)player.States.size() - 1);
		}
		LUI_Tooltip("Go to the last step");
		if (player.CurrentStep == -1)
			ImGui::EndDisabled();
		ImGui::SameLine();
		ImGui::SliderFloat("Speed", &speed, 1, 10, "%.2f", ImGuiSliderFlags_AlwaysClamp);
		LUI_Tooltip("Choose the speed of the algorithm (steps per second)");
	}
	ImGui::End();
}

void LUI_StartVertexSelect(bool& IsStartVertexActive, int& startVertex, int max, Algorithm algorithm, bool& calculating) {
	if (algorithm == Algorithm::FLOYD_WARSHALL || algorithm == Algorithm::KRUSKAL) {
		calculating = true;
		IsStartVertexActive = false;
	}
	else {
		ImGui::SetNextWindowPos({ 230, 10 }, ImGuiCond_Appearing);
		ImGui::Begin("Start Vertex", &IsStartVertexActive, ImGuiWindowFlags_AlwaysAutoResize);
		{
			if (ImGui::InputInt("##", &startVertex)) {
				if (startVertex > max - 1) startVertex = max - 1;
				if (startVertex < 0) startVertex = 0;
			}
			LUI_Tooltip("Choose the start vertex");
			ImGui::SameLine();
			if (ImGui::Button("Start", { 70,0 })) {
				calculating = true;
				IsStartVertexActive = false;
			}
			LUI_Tooltip("Start the algorithm");
		}
		ImGui::End();
	}
}

void LUI_RandomGraph(Player::Player& player, float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, bool& showRandomGraph) {
	static bool isDirected = false;
	static float weight[2] = { 0.f, 10.f };
	static float ratio = 0.5;
	static int decimal = 0, nn = n;
	LUI_DefaultWindowPos(-1, ImGuiCond_Once);
	if (!ImGui::IsPopupOpen("Random Graph")) ImGui::OpenPopup("Random Graph");
	if (ImGui::BeginPopupModal("Random Graph", &showRandomGraph, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::InputInt("Vertices", &nn)) {
			if (nn < 0) nn = 0;
			if (nn > GRAPH_MAXSIZE) nn = GRAPH_MAXSIZE;
		}
		LUI_Tooltip("Choose number of vertices");
		ImGui::Checkbox("Directed", &isDirected);
		LUI_Tooltip("If the graph is directed or not");
		ImGui::InputFloat2("Weight Range", weight);
		LUI_Tooltip("Choose the range of weight of edges");
		if (ImGui::InputInt("Decimal", &decimal)) {
			if (decimal < 0) decimal = 0;
			if (decimal > 10) decimal = 10;
		}
		LUI_Tooltip("Choose the number of decimal places");
		ImGui::SliderFloat("Edge Ratio", &ratio, 0, 1, "%.2f", ImGuiSliderFlags_AlwaysClamp);
		LUI_Tooltip("Choose the ratio of edges between vertices");
		ImGui::Separator();
		if (ImGui::Button("Generate", { ImGui::GetContentRegionAvail().x / 2, 0 })) { /*generate random graph*/
			player.init();
			player.Graph.isDirected = isDirected;
			n = nn;
			Randomize::Graph<float>(n, ratio, *new Randomize::Float(weight[0], weight[1], decimal)).weighted().directed(isDirected).fill(&Data[0][0], GRAPH_MAXSIZE);
			player.Graph.set(&Data[0][0], n);
			showRandomGraph = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", { ImGui::GetContentRegionAvail().x, 0 })) {
			showRandomGraph = false;
		}
		ImGui::EndPopup();
	}
}

void LUI_SettingMenu(bool& showInputGraph, bool& showSaveMenu, bool& showRandomGraph, bool& showRendererSetting) {
	LUI_DefaultWindowPos(1);
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		if (ImGui::Button("Open File", { 80,0 }) && !ImGuiFileDialog::Instance()->IsOpened("SaveFileDlgKey")) { /*open txt file*/
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".txt", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
			showInputGraph = true;
		}
		LUI_Tooltip("Load graph from txt file");
		ImGui::SameLine();
		ImVec2 ButtonSize = { ImGui::GetContentRegionAvail().x / 2 - 5, 0 };
		if (ImGui::Button("Save Graph", ButtonSize)) { /*save txt file*/ showSaveMenu = true; }
		LUI_Tooltip("Save current graph to txt file");
		ImGui::SameLine();
		if (ImGui::Button("Input Graph", { ImGui::GetContentRegionAvail().x,0 })) { showInputGraph = !showInputGraph; }
		LUI_Tooltip("Create a new graph/Edit current graph");
		ImGui::Separator();
		if (ImGui::Button("Random", { 110,0 })) { /*random graph*/ showRandomGraph = true; }
		LUI_Tooltip("Generate a random graph");
		ImGui::SameLine();
		if (ImGui::Button("Renderer Setting", { 170,0 })) { showRendererSetting = true; }
		LUI_Tooltip("Change color and size of vertices and edges");
		ImGui::SameLine();
	}
	ImGui::End();
}

void LUI_AlgorithmSelectMenu(bool& IsStartVertexActive, Algorithm& algorithm) {
	LUI_DefaultWindowPos(0);
	ImGui::Begin("Algorithms", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		float x = ImGui::GetContentRegionAvail().x;
		if (ImGui::TreeNodeEx("Brute Force", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::Selectable("BFS", algorithm == BFS)) { /*BFS*/ IsStartVertexActive = true; algorithm = BFS; }
			if (ImGui::Selectable("DFS", algorithm == DFS)) { /*DFS*/ IsStartVertexActive = true; algorithm = DFS; }
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Dynamic Programming", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::Selectable("Bellman-Ford", algorithm == BELLMAN_FORD)) { /*Bellman-Ford*/ IsStartVertexActive = true; algorithm = BELLMAN_FORD; }
			if (ImGui::Selectable("Floyd-Warshall", algorithm == FLOYD_WARSHALL)) { /*Floyd-Warshall*/ IsStartVertexActive = true; algorithm = FLOYD_WARSHALL; }
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Greedy", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::Selectable("Dijkstra", algorithm == DIJKSTRA)) { /*Dijkstra*/ IsStartVertexActive = true; algorithm = DIJKSTRA; }
			if (ImGui::Selectable("Kruskal", algorithm == KRUSKAL)) { /*Kruskal*/ IsStartVertexActive = true; algorithm = KRUSKAL; }
			if (ImGui::Selectable("Prim", algorithm == PRIM)) { /*Prim*/ IsStartVertexActive = true; algorithm = PRIM; }
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

static void LUI_HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static void LUI_Tooltip(const char* desc)
{
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void LUI_InputGraph(Player::Player& player, float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, bool& showInputGraph, InputType& inputType, VIS_Graph_Layout& Layout) {
	static char InputBuffer[1024 * 16] = { 0 };
	static char ErrorBuffer[256] = { 0 };
	static int InputInvalid = 0, temproot = player.Graph.root;
	static bool isConverted = false, parseFile = false;
	static bool isDirected = player.Graph.isDirected;
	static float temp[GRAPH_MAXSIZE][GRAPH_MAXSIZE] = { 0 };
	static VIS_Graph_Layout tempLayout = Layout;
	static InputType tempInputType = inputType;
	static int temp_n = 0;
	LUI_DefaultWindowPos(4, ImGuiCond_Appearing);
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			InputInvalid = LUI_ParseInputFile(InputBuffer, ImGuiFileDialog::Instance()->GetFilePathName().c_str(), ErrorBuffer);
			isConverted = true;
			parseFile = true;
			if (!InputInvalid) {
				if (!LUI_ParseAM(InputBuffer, temp, temp_n, ErrorBuffer)) tempInputType = ADJACENCY_MATRIX;
				else if (!LUI_ParseAL(InputBuffer, temp, temp_n, ErrorBuffer)) tempInputType = ADJACENCY_LIST;
				else if (!LUI_ParseEL(InputBuffer, temp, temp_n, ErrorBuffer)) tempInputType = EDGE_LIST;
			}
		}
		else {
			showInputGraph = false;
		}
		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->IsOpened("ChooseFileDlgKey")) {
		return;
	}
	if (!isConverted) {
		LUI_ConvertTracerToString(InputBuffer, player.Graph, tempInputType);
		isConverted = true;
	}
	LUI_DefaultWindowPos(-4, ImGuiCond_Appearing);
	if (!ImGui::IsPopupOpen("Input Graph")) ImGui::OpenPopup("Input Graph");
	if (ImGui::BeginPopupModal("Input Graph", &showInputGraph))
	{
		if (ImGui::IsWindowAppearing() && !parseFile) {
			tempInputType = inputType;
			isConverted = false;
			tempLayout = Layout;
			temproot = player.Graph.root;
			isDirected = player.Graph.isDirected;
			temp_n = n;
		}
		else
			parseFile = false;
		ImGui::SeparatorText("Graph Input Type");
		if (ImGui::RadioButton("Adjacency Matrix", (int*)&tempInputType, ADJACENCY_MATRIX))
			isConverted = false;
		LUI_Tooltip("Input graph as an adjacency matrix");
		ImGui::SameLine();
		if (ImGui::RadioButton("Adjacency List", (int*)&tempInputType, ADJACENCY_LIST))
			isConverted = false;
		LUI_Tooltip("Input graph as an adjacency list");
		ImGui::SameLine();
		if (ImGui::RadioButton("Edge List", (int*)&tempInputType, EDGE_LIST))
			isConverted = false;
		LUI_Tooltip("Input graph as an edge list");
		ImGui::SameLine();
		LUI_HelpMarker("Click Help for more information");
		ImGui::SeparatorText("Output Graph Layout");
		ImGui::RadioButton("Circle", (int*)&tempLayout, VIS_Graph_Layout_Circle);
		LUI_Tooltip("Place vertices in a circle");
		ImGui::SameLine();
		ImGui::RadioButton("Tree", (int*)&tempLayout, VIS_Graph_Layout_Tree);
		LUI_Tooltip("Place vertices as a tree, default root is vertex 0");
		ImGui::SameLine();
		ImGui::RadioButton("Random", (int*)&tempLayout, VIS_Graph_Layout_Random);
		LUI_Tooltip("Place vertices randomly");
		if (tempLayout == VIS_Graph_Layout_Tree) {
			if (ImGui::InputInt("Root", &temproot)) {
				if (temproot < 0) temproot = 0;
				InputInvalid = 0;
			}
			LUI_Tooltip("Choose root of the tree");
		}
		ImGui::Separator();
		ImGui::Checkbox("Directed", &isDirected);
		LUI_Tooltip("If the graph is directed or not");
		ImGui::Separator();
		if (InputInvalid) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 0.00f, 0.00f, 1.00f));
			ImGui::Text("%s", ErrorBuffer);
			ImGui::PopStyleColor();
			ImGui::Separator();
		}
		if (ImGui::InputTextMultiline("Input", InputBuffer, IM_ARRAYSIZE(InputBuffer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput) && InputInvalid) InputInvalid = 0;
		ImGui::SameLine();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, ImGui::GetTextLineHeight() * 16), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::TextUnformatted(InputBuffer);
		ImGui::EndChild();
		ImGui::Separator();
		ImVec2 ButtonSize = { ImGui::GetContentRegionAvail().x / 4 - 3, 0 };
		if (ImGui::Button("Submit", ButtonSize)) { /*submit input*/
			InputInvalid = LUI_ParseInput(InputBuffer, temp, temp_n, tempInputType, ErrorBuffer);
			if (temp_n > GRAPH_MAXSIZE) {
				InputInvalid = 1;
				strcpy(ErrorBuffer, "Number of vertices is too large");
			}
			if (temproot >= temp_n) {
				InputInvalid = 1;
				strcpy(ErrorBuffer, "Root is out of range");
			}
			if (!InputInvalid) {
				inputType = tempInputType;
				Layout = tempLayout;
				player.Graph.root = temproot;
				n = temp_n;
				memcpy(Data, temp, sizeof(temp));
				showInputGraph = false;
				player.init();
				player.Graph.isDirected = isDirected;
				player.Graph.set(&Data[0][0], n);
				isConverted = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear", ButtonSize)) {
			memset(InputBuffer, 0, sizeof(InputBuffer));
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ButtonSize)) {
			isConverted = false;
			showInputGraph = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Help", { ImGui::GetContentRegionAvail().x,0 })) { /*show help*/ }
		ImGui::EndPopup();
	}
}

void LUI_ConvertTracerToString(char Buffer[], VIS::GraphTracer Tracer, InputType InputType) {
	switch (InputType)
	{
	case ADJACENCY_MATRIX:
		strcpy(Buffer, TextFormat("%d", Tracer.vertices.size()));
		for (int i = 0; i < Tracer.vertices.size(); i++) {
			strcat(Buffer, "\n");
			for (int j = 0; j < Tracer.vertices.size(); j++) {
				VIS::Edge* edge = Tracer.findEdge(i, j);
				strcat(Buffer, TextFormat("%g ", edge ? edge->weight : 0));
			}
		}
		break;
	case ADJACENCY_LIST:
		strcpy(Buffer, TextFormat("%d", Tracer.vertices.size()));
		for (int i = 0; i < Tracer.vertices.size(); i++) {
			strcat(Buffer, "\n");
			int count = 0;
			char temp[256] = { 0 };
			for (int j = 0; j < Tracer.vertices.size(); j++) {
				VIS::Edge* edge = Tracer.findEdge(i, j);
				if (edge) {
					strcat(temp, TextFormat("%d %g ", j, edge->weight));
					count++;
				}
			}
			strcat(Buffer, TextFormat("%d %s", count, temp));
		}
		break;
	case EDGE_LIST:
		strcpy(Buffer, TextFormat("%d %d", Tracer.vertices.size(), Tracer.edges.size()));
		for (int i = 0; i < Tracer.edges.size(); i++) {
			strcat(Buffer, "\n");
			strcat(Buffer, TextFormat("%d %d %g", Tracer.edges[i]->source, Tracer.edges[i]->target, Tracer.edges[i]->weight));
		}
		break;
	default:
		assert("Invalid InputType");
		break;
	}
	return;
}


void LUI_SaveMenu(VIS::GraphTracer& Tracer, bool& showSaveMenu) {
	static char FileName[256] = "graph.txt";
	static char OutputBuffer[1024 * 16];
	static char ErrorBuffer[256];
	static bool isError = false;
	static InputType InputType = ADJACENCY_MATRIX;
	if (Tracer.vertices.size() < 1) {
		showSaveMenu = false;
		return;
	}
	LUI_DefaultWindowPos(4, ImGuiCond_Appearing);
	if (!ImGui::IsPopupOpen("Save Menu")) ImGui::OpenPopup("Save Menu");
	if (ImGui::BeginPopupModal("Save Menu", &showSaveMenu))
	{
		ImGui::SeparatorText("Save Graph");
		ImGui::RadioButton("Adjacency Matrix", (int*)&InputType, ADJACENCY_MATRIX);
		LUI_Tooltip("Save graph as an adjacency matrix");
		ImGui::SameLine();
		ImGui::RadioButton("Adjacency List", (int*)&InputType, ADJACENCY_LIST);
		LUI_Tooltip("Save graph as an adjacency list");
		ImGui::SameLine();
		ImGui::RadioButton("Edge List", (int*)&InputType, EDGE_LIST);
		LUI_Tooltip("Save graph as an edge list");
		ImGui::SeparatorText("File Path");
		if (isError) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 0.00f, 0.00f, 1.00f));
			ImGui::Text("%s", ErrorBuffer);
			ImGui::PopStyleColor();
			ImGui::Separator();
		}
		if (ImGui::InputText("##", FileName, IM_ARRAYSIZE(FileName))) { isError = false; }
		ImGui::SameLine();
		if (ImGui::Button("Choose Path", { ImGui::GetContentRegionAvail().x,0 }) && !ImGuiFileDialog::Instance()->IsOpened("ChooseFileDlgKey")) {
			ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose Path", ".txt\0", FileName, 1, nullptr, ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite);
		}
		LUI_Tooltip("Choose path to save the file");
		ImVec2 ButtonSize = { ImGui::GetContentRegionAvail().x / 2, 0 };
		if (ImGui::Button("Save", ButtonSize)) {
			LUI_ConvertTracerToString(OutputBuffer, Tracer, InputType);
			if (!LUI_SaveFile(OutputBuffer, FileName, ErrorBuffer)) {
				showSaveMenu = false;
			}
			else
				isError = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ButtonSize)) {
			showSaveMenu = false;
		}
		LUI_DefaultWindowPos(4, ImGuiCond_Appearing);
		if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey")) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				strcpy(FileName, ImGuiFileDialog::Instance()->GetFilePathName().c_str());
			}
			ImGuiFileDialog::Instance()->Close();
		}
		ImGui::EndPopup();
	}
}

int LUI_SaveFile(char OutputBuffer[], const char* fileName, char ErrorBuffer[]) {
	FILE* file;
	fopen_s(&file, fileName, "w");
	if (file == NULL) {
		strcpy(ErrorBuffer, "Failed to open the file.");
		return 1;
	}
	fprintf(file, "%s", OutputBuffer);
	fclose(file);
	return 0;
}

int LUI_ParseInputFile(char InputBuffer[], const char* fileName, char ErrorBuffer[]) {
	memset(InputBuffer, 0, sizeof(InputBuffer));
	char temp[1024];
	FILE* file;
	fopen_s(&file, fileName, "r");
	if (file == NULL) {
		strcpy(ErrorBuffer, "Failed to open the file.");
		return 1;
	}
	while (fgets(temp, sizeof(temp), file) != NULL) {
		strcat(InputBuffer, temp);
		if (strlen(InputBuffer) >= 16383)
			break;
	}
	fclose(file);
	return 0;
}

int LUI_ParseInput(char InputBuffer[], float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, InputType InputType, char ErrorBuffer[]) {
	switch (InputType)
	{
	case ADJACENCY_MATRIX:
		return LUI_ParseAM(InputBuffer, Data, n, ErrorBuffer);
	case ADJACENCY_LIST:
		return LUI_ParseAL(InputBuffer, Data, n, ErrorBuffer);
	case EDGE_LIST:
		return LUI_ParseEL(InputBuffer, Data, n, ErrorBuffer);
	default:
		break;
	}
	strcpy(ErrorBuffer, "Invalid Input Type!");
	return 1;
}

int LUI_ParseAM(char InputBuffer[], float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, char ErrorBuffer[]) {
	// Step 1: Extract the value of n from the first line of the buffer
	int cn = -1;
	if (!sscanf(InputBuffer, "%d", &cn) || cn < 1) {
		strcpy(ErrorBuffer, "Invalid number of vertices!");
		return 1;
	}
	n = cn;
	const char* line = strchr(InputBuffer, '\n') + 1;
	if (line == NULL) {
		strcpy(ErrorBuffer, "Invalid line 1");
		return 1;
	}
	int row = 0;
	while (line != NULL && row < n) {
		if (line == (char*)+1) {
			strcpy(ErrorBuffer, TextFormat("Invalid line %d", row + 1));
			return 1;
		}
		char* end;
		for (int col = 0; col < n; col++) {
			Data[row][col] = strtof(line, &end);
			if (line == end) {
				strcpy(ErrorBuffer, TextFormat("Invalid element (%d:%d)", row, col));
				return 1;
			}
			line = end;
			if (row == col && Data[row][col] != 0) {
				strcpy(ErrorBuffer, TextFormat("Invalid element (%d:%d)", row, col));
				return 1;
			}
		}
		row++;
		line = strchr(line, '\n') + 1;
		if (line == NULL) {
			strcpy(ErrorBuffer, TextFormat("Invalid line %d", row + 1));
			return 1;
		}
	}
	return 0;
}

int LUI_ParseAL(char InputBuffer[], float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, char ErrorBuffer[]) {
	int cn = -1;
	if (!sscanf(InputBuffer, "%d", &cn) || cn < 1) {
		strcpy(ErrorBuffer, "Invalid number of vertices!");
		return 1;
	}
	n = cn;

	std::vector<std::vector<std::pair<int, float>>> adjacencyList(n); // Create an empty adjacency list

	const char* line = strchr(InputBuffer, '\n') + 1;
	if (line == NULL) {
		strcpy(ErrorBuffer, "Invalid line 1");
		return 1;
	}
	int vertex = 0;
	while (line != NULL && vertex < n) {
		if (line == (char*)+1) {
			strcpy(ErrorBuffer, TextFormat("Invalid line %d", vertex + 1));
			return 1;
		}
		char* end;
		int numEdges;
		numEdges = (int)strtol(line, &end, 10);
		if (line == end || numEdges < 0 || numEdges >= n) {
			strcpy(ErrorBuffer, TextFormat("Invalid number of edges, line %d", vertex + 1));
			return 1;
		}
		line = end;
		for (int i = 0; i < numEdges; i++) {
			int target;
			float weight;
			target = (int)strtol(line, &end, 10);
			if (line == end || target < 0 || target >= n || target == vertex) {
				strcpy(ErrorBuffer, TextFormat("Invalid target vertex, line %d", vertex + 1));
				return 1;
			}
			line = end;
			weight = strtof(line, &end);
			if (line == end) {
				strcpy(ErrorBuffer, TextFormat("Invalid weight, line %d", vertex + 1));
				return 1;
			}
			line = end;
			adjacencyList[vertex].push_back(std::make_pair(target, weight));
		}
		vertex++;
		line = strchr(line, '\n') + 1;
		if (line == NULL) {
			strcpy(ErrorBuffer, TextFormat("Invalid line %d", vertex + 1));
			return 1;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Data[i][j] = 0.0;
		}
		for (const auto& edge : adjacencyList[i]) {
			int target = edge.first;
			float weight = edge.second;
			Data[i][target] = weight;
		}
	}
	return 0;
}

int LUI_ParseEL(char InputBuffer[], float Data[GRAPH_MAXSIZE][GRAPH_MAXSIZE], int& n, char ErrorBuffer[]) {
	int numVertices = -1, numEdges = -1;
	if (sscanf(InputBuffer, "%d %d", &numVertices, &numEdges) != 2 || numVertices < 1 || numEdges < 0) {
		strcpy(ErrorBuffer, "Invalid number of vertices or edges!");
		return 1;
	}
	n = numVertices;
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++) {
			Data[i][j] = 0.0;
		}
	}
	const char* line = strchr(InputBuffer, '\n') + 1;
	if (line == NULL) {
		strcpy(ErrorBuffer, "Invalid line 1");
		return 1;
	}
	for (int i = 0; i < numEdges; i++) {
		if (line == (char*)+1) {
			strcpy(ErrorBuffer, TextFormat("Invalid line %d", i + 1));
			return 1;
		}
		char* end;
		int source, target;
		float weight;
		source = (int)strtol(line, &end, 10);
		if (line == end || source < 0 || source >= numVertices) {
			strcpy(ErrorBuffer, TextFormat("Invalid source vertex, line %d", i + 1));
			return 1;
		}
		line = end;
		target = (int)strtol(line, &end, 10);
		if (line == end || target < 0 || target >= numVertices || target == source) {
			strcpy(ErrorBuffer, TextFormat("Invalid target vertex, line %d", i + 1));
			return 1;
		}
		line = end;
		weight = strtof(line, &end);
		if (line == end) {
			strcpy(ErrorBuffer, TextFormat("Invalid weight, line %d", i + 1));
			return 1;
		}
		line = end;
		Data[source][target] = weight;
		line = strchr(line, '\n') + 1;
		if (line == NULL) {
			strcpy(ErrorBuffer, TextFormat("Invalid line %d", i + 1));
			return 1;
		}
	}
	return 0;
}

#endif // LUI_IMPLEMENTATION