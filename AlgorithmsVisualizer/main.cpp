#define NO_FONT_AWESOME
#include "imgui/imgui.h"
#include "rlImGui/imgui_impl_raylib.h"
#include "raylib/include/raylib.h"
#include "raylib/include/raymath.h"

#define LUI_IMPLEMENTATION
#include "Visualizer/LUI.h"

#define PLAYER_IMPLEMENTATION
#include "Visualizer/Player.h"

#define VIS_IMPLEMENTATION
#include "Visualizer/Visualizer.h"

#define SSSPALGO_IMPLEMENTATION
#include "Visualizer/Algorithms.h"

#include "Visualizer/Randomize.h"

#include "Visualizer/CompressedFont/PTSans_Bold.cpp"
#include "Visualizer/CompressedFont/tahoma.cpp"

#define FPS 60
#define WINDOW_FACTOR 80

size_t WINDOW_WIDTH = (16 * WINDOW_FACTOR);
size_t WINDOW_HEIGHT = (9 * WINDOW_FACTOR);

Font font;
ImFont* ifont = NULL;

bool showInputGraph = false;
bool showRendererSetting = false;
bool selectingStartVertex = false;
bool showSaveMenu = false;
bool showRandomGraph = false;

Player::Player player;
InputType inputType = InputType::ADJACENCY_MATRIX;
Algorithm algorithm = Algorithm::NONE;

bool calculating = false;
float speed = 2;

float graph[GRAPH_MAXSIZE][GRAPH_MAXSIZE];
int n = 8, src = 0;

/*float points[TABLE_SIZE][TABLE_SIZE] = {
							{0,4,0,0,0,1,0,0},
							{4,0,7,3,0,2,0,0},
							{0,7,0,3,1,0,0,0},
							{0,3,3,0,1,0,4,0},
							{0,0,1,1,0,0,0,3},
							{1,2,0,0,0,0,3,0},
							{0,0,0,4,0,3,0,5},
							{0,0,0,0,3,0,5,0}
};*/

void openWindow();
void closeWindow();
void loadStyle();
void init();
void deinit();
void loop();
void calculate();
void handleInput();
void render();

int main(void) {
	openWindow();
	loadStyle();
	init();
	loop();
	closeWindow();
	deinit();
	return 0;
}

void openWindow() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow((int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, "Algorithms Visualizer - Group 13");
	SetTargetFPS(FPS);

	ImGui::CreateContext();
}

void closeWindow() {
	ImGui_ImplRaylib_Shutdown();
	ImGui::DestroyContext();

	CloseWindow();
}

void loadStyle() {
	//raylib font
	font = LoadFontFromMemory(".ttf", PTSANS_BOLD_DATA, PTSANS_BOLD_SIZE, 500, NULL, 0);
	GenTextureMipmaps(&font.texture);
	SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

	//ImGui style
	ImGuiStyle& style = ImGui::GetStyle();
	LUI_loadTheme(style);

	//ImGui font
	ImGuiIO& io = ImGui::GetIO();
	ifont = io.Fonts->AddFontFromMemoryCompressedBase85TTF(tahoma_compressed_data_base85, 20.f);
	if (ifont)
		io.FontDefault = ifont;
	ImGui_ImplRaylib_Init();
}

void init() {
	Randomize::Graph<float>(n, 0.4, *new Randomize::Float(1, 10, 0)).weighted().directed(false).fill(&graph[0][0], GRAPH_MAXSIZE);
	player.init();
	GRAPH.init(500, 500, font, 20, 2);
	GRAPH.set(&graph[0][0], n);
	TABLE.init();
	PSEUDO.init();
}

void deinit() {
	player.clear();
	UnloadFont(font);
}

void loop() {
	while (!WindowShouldClose()) {
		if (GetScreenHeight() < WINDOW_HEIGHT && GetScreenHeight() < GetMonitorHeight(GetCurrentMonitor()))
			SetWindowSize(GetScreenWidth(), (int)WINDOW_HEIGHT);
		if (GetScreenWidth() < WINDOW_WIDTH && GetScreenWidth() < GetMonitorWidth(GetCurrentMonitor()))
			SetWindowSize((int)WINDOW_WIDTH, GetScreenHeight());

		calculate();

		ImGui_ImplRaylib_ProcessEvents();

		handleInput();

		render();
	}
}

void calculate() {
	if (calculating) {
		switch (algorithm)
		{
		case DIJKSTRA:
			Dijkstra(player, graph, n, src);
			break;
		case BELLMAN_FORD:
			BellmanFord(player, graph, n, src);
			break;
		case FLOYD_WARSHALL:
			FloydWarshall(player, graph, n);
			break;
		case PRIM:
			Prim(player, graph, n, src);
			break;
		case KRUSKAL:
			Kruskal(player, graph, n);
			break;
		case DFS:
			DepthFirstSearch(player, graph, n, src);
			break;
		case BFS:
			BreadthFirstSearch(player, graph, n, src);
			break;
		case NONE:
			break;
		}
		calculating = false;
		if (player.States.size() > 0) {
			player.CurrentStep = 0;
			player.load(player.CurrentStep);
		}
	}
}

void handleInput() {
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse) {
		GRAPH.renderer.handleMouseDown();
		GRAPH.renderer.handleMouseMove();
	}

	if (player.States.size() < 1 || io.WantCaptureKeyboard)
		return;

	if (IsKeyPressed(KEY_SPACE)) {
		player.pause = !player.pause;
		if (player.CurrentStep == player.States.size() - 1)
			player.CurrentStep = 0;
		player.Timer.start(1 / speed);
	}
	if (player.pause) {
		if (IsKeyPressed(KEY_RIGHT)) {
			player.CurrentStep++;
			if (player.CurrentStep >= player.States.size())
				player.CurrentStep = (int)player.States.size() - 1;
			player.load(player.CurrentStep);
		}
		if (IsKeyPressed(KEY_LEFT)) {
			player.CurrentStep--;
			if (player.CurrentStep < 0)
				player.CurrentStep = 0;
			player.load(player.CurrentStep);
		}
	}
}

void render() {
	if (!player.pause)
		player.run(speed);

	ImGui_ImplRaylib_NewFrame();
	ImGui::NewFrame();
	{
		LUI_Navigator(player, speed);
		LUI_AlgorithmSelectMenu(selectingStartVertex, algorithm);
		LUI_SettingMenu(showInputGraph, showSaveMenu, showRandomGraph, showRendererSetting);

		if (showRendererSetting)
			player.Graph.renderer.RendererSetting(showRendererSetting);
		if (showInputGraph) {
			player.pause = true;
			LUI_InputGraph(player, graph, n, showInputGraph, inputType, player.Graph.layout);
		}
		if (selectingStartVertex) {
			player.pause = true;
			LUI_StartVertexSelect(selectingStartVertex, src, n, algorithm, calculating);
		}
		if (showSaveMenu) {
			player.pause = true;
			LUI_SaveMenu(player.Graph, showSaveMenu);
		}
		if (showRandomGraph) {
			player.pause = true;
			LUI_RandomGraph(player, graph, n, showRandomGraph);
		}

		TABLE.renderer.draw();
		PSEUDO.renderer.draw();
	}

	ImGui::Render();
	BeginDrawing();
	{
		ClearBackground({ 36,36,36,255 });
		DrawFPS(10, GetScreenHeight() - 80);
		GRAPH.renderer.draw();
		ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
	}
	EndDrawing();
}