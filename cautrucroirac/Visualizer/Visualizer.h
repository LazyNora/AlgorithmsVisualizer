// A UI framework built on top of algorithms.h and raylib.
// It provides some useful and basic widgets for setting up the UI.
// Because logging in terminal is boring.

#ifndef VIS_H_
#define VIS_H_

#include <float.h>
#include <math.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iterator>

#include "../raylib/include/raylib.h"
#include "../raylib/include/raymath.h"

#include "../imgui/imgui.h"
#include "../rlImGui/rlImGuiColors.h"

#define GRAPH_MAXSIZE 25
#define TRUE_FLT_MIN std::numeric_limits<float>::lowest()

//Default Coloring Prob
#define Color_Font CLITERAL(Color) { 187, 187, 187, 255 }
#define Color_Selected CLITERAL(Color) { 41, 98, 220, 255 }
#define Color_Visited CLITERAL(Color) { 197, 17, 98, 255 }
#define Color_Hovering CLITERAL(Color) { 0, 230, 118, 255 }
#define Theme_Light CLITERAL(Color) { 80, 80, 80, 255 }
#define Color_Deactive CLITERAL(Color) { 0x53, 0x53, 0x53, 0xFF }

typedef int VIS_Edge_Type;
typedef int VIS_Graph_Layout;
typedef int VIS_Pseudo_Color;

enum VIS_Edge_Type_ {
	VIS_Edge_Type_Undirected,
	VIS_Edge_Type_Directed
};

enum VIS_Graph_Layout_ {
	VIS_Graph_Layout_Circle,
	VIS_Graph_Layout_Tree,
	VIS_Graph_Layout_Random
};

enum VIS_Pseudo_Color_ {
	VIS_Pseudo_Color_Default,
	VIS_Pseudo_Color_Selected,
	VIS_Pseudo_Color_Visited,
	VIS_Pseudo_Color_Marked,
	VIS_Pseudo_Color_LightBlue,
	VIS_Pseudo_Color_LightGreen,
	VIS_Pseudo_Color_LightRed,
	VIS_Pseudo_Color_LightYellow,
	VIS_Pseudo_Color_LightPurple,
	VIS_Pseudo_Color_LightOrange,
	VIS_Pseudo_Color_LightBrown,
	VIS_Pseudo_Color_LightPink,
	VIS_Pseudo_Color_LightGray,
	VIS_Pseudo_Color_LightTeal,
	VIS_Pseudo_Color_LightLime,
	VIS_Pseudo_Color_LightMagenta
};

namespace VIS {
	struct GraphRenderer;
	struct Colors;
	struct Dimensions;
	struct GraphTracer;
	struct Rect;
	struct Vertex;
	struct Edge;
	struct GraphMemento;

	struct TableRenderer;
	struct TableTracer;
	struct TableColors;
	struct Table;
	struct TableMemento;
	struct TableCell;

	struct PseudoCodeRenderer;
	struct PseudoCodeTracer;
	struct PseudoCodeMemento;
	struct PseudoCodeColors;

	typedef std::unordered_map<int, std::string> PseudoCode;

	typedef struct PseudoCodeColors {
		::Color Selected = Color_Selected;
		::Color Visited = Color_Visited;
		::Color Marked = Color_Hovering;
		::Color LightBlue = { 146, 207, 227, 255 };
		::Color LightGreen = { 161, 214, 174, 255 };
		::Color LightPink = { 244, 154, 194, 255 };
		::Color LightYellow = { 255, 255, 153, 255 };
		::Color LightPurple = { 178, 171, 210, 255 };
		::Color LightOrange = { 253, 191, 111, 255 };
		::Color LightBrown = { 166, 97, 26, 255 };
		::Color LightRed = { 255, 93, 84, 255 };
		::Color LightGray = { 200, 200, 200, 255 };
		::Color LightTeal = { 127, 201, 127, 255 };
		::Color LightLime = { 82, 255, 148, 255 };
		::Color LightMagenta = { 255, 119, 255, 255 };
		ImVec4 getColor(VIS_Pseudo_Color color);
	} PseudoCodeColors;

	typedef struct PseudoCodeRenderer {
		PseudoCodeTracer* tracer = NULL;
		PseudoCodeColors color;

		void draw();
		//void RendererSetting(bool& showRendererSetting);
	} PseudoCodeRenderer;

	typedef struct PseudoCodeTracer {
		PseudoCodeRenderer renderer;
		PseudoCode codes;
		std::unordered_map<int, VIS_Pseudo_Color> lines;

		void init();
		void select(int line);
		void visit(int line);
		void mark(int line);
		void highlight(int line, VIS_Pseudo_Color color = VIS_Pseudo_Color_LightBlue);
		void reset(int line);
		void reset();
		void clear();
		PseudoCodeMemento* save();
		void load(PseudoCodeMemento* const m);
	} PseudoCodeTracer;

	typedef struct PseudoCodeMemento {
		std::unordered_map<int, VIS_Pseudo_Color> lines;

		PseudoCodeMemento(const std::unordered_map<int, VIS_Pseudo_Color> lines);
	} PseudoCodeMemento;

	typedef struct TableCell {
		std::string text = "##";
		bool selected = false;
		bool patched = false;
		bool marked = false;
		bool isDisable = false;

		TableCell();
		TableCell& operator=(const TableCell& other);
		TableCell& operator=(const TableCell* other);
	} TableCell;

	typedef std::unordered_map<int, TableCell> TableRow;

	typedef struct Table {
		int cols = 0;
		std::string title;
		std::vector<std::string> footer;
		std::unordered_map<int, std::string> rowHeaders;
		std::unordered_map<int, std::string> colHeaders;
		std::unordered_map<int, TableRow> data;
		bool showRowHeaders = true;
		bool showColHeaders = true;
		::Color FooterColor = WHITE;

		void init(int cols, std::string title = "##");
		void set(float* array2D = NULL, int N = 0, int MaxN = GRAPH_MAXSIZE);
		void set(float** array2D = NULL, int rows = 0, int cols = 0);
		void set(int* array2D = NULL, int N = 0, int MaxN = GRAPH_MAXSIZE);
		void set(int** array2D = NULL, int rows = 0, int cols = 0);
		void create(int cols = 0);
		void select(int row, int col);
		void deselect(int row, int col);
		void selectCol(int col);
		void deselectCol(int col);
		void selectRow(int row);
		void deselectRow(int row);
		void disable(int row, int col, bool disable = true);
		void disableCol(int col, bool disable = true);
		void disableRow(int row, bool disable = true);
		void patch(int row, int col);
		void depatch(int row, int col);
		void mark(int row, int col);
		void unmark(int row, int col);
		void clear();
	} Table;

	typedef struct TableColors {
		::Color Cell = WHITE;
		::Color CellSelected = Color_Selected;
		::Color CellPatched = Color_Hovering;
		::Color CellMarked = Color_Visited;

		::Color Header = { 48, 48, 51, 255 };
	} TableColors;

	typedef struct TableRenderer {
		TableTracer* tracer = NULL;
		TableColors color;

		void draw();
		//void RendererSetting(bool& showRendererSetting);
	} TableRenderer;

	typedef struct TableTracer {
		TableRenderer renderer;
		std::vector<Table> tables;

		void init();
		void set(float* array2D, int N, int MaxN = GRAPH_MAXSIZE);
		void set(int* array2D, int N, int MaxN = GRAPH_MAXSIZE);
		void addTable(int cols, std::string title = "##");
		void updateTable(int index, int cols, std::string title = "##");
		void removeTable(int index);
		void select(int tableIndex, int row, int col);
		void deselect(int tableIndex, int row, int col);
		void selectCol(int tableIndex, int col);
		void deselectCol(int tableIndex, int col);
		void selectRow(int tableIndex, int row);
		void deselectRow(int tableIndex, int row);
		void disable(int tableIndex, int row, int col, bool disable = true);
		void disableCol(int tableIndex, int col, bool disable = true);
		void disableRow(int tableIndex, int row, bool disable = true);
		void patch(int tableIndex, int row, int col);
		void depatch(int tableIndex, int row, int col);
		void clear();
		TableMemento* save();
		void load(TableMemento* const m);
	} TableTracer;

	typedef struct TableMemento {
		std::vector<Table> tables;
		TableMemento(const std::vector<Table> tables);
	} TableMemento;

	typedef struct GraphMemento {
		std::vector<Vertex> vertices;
		std::vector<Edge> edges;
		GraphMemento(const std::vector<Vertex*> vertices, const std::vector<Edge*> edges);
	} GraphMemento;

	typedef struct Vertex {
		int id = -1;
		Vector2 position = { 0,0 };
		float weight = TRUE_FLT_MIN;
		int visitedCount = 0;
		int selectedCount = 0;
		bool dragging = false;
		bool hovering = false;
		Vertex& operator=(const Vertex& other);
		Vertex& operator=(const Vertex* other);
	} Vertex;

	typedef struct Edge {
		int source = -1;
		int target = -1;
		float weight = 0;
		int visitedCount = 0;
		int selectedCount = 0;
		bool disabled = false;
		bool hovering = false;
		Edge& operator=(const Edge& other);
		Edge& operator=(const Edge* other);
	} Edge;

	typedef struct Rect {
		float left, top, right, bottom, width, height;
	} Rect;

	typedef struct GraphColors {
		//Vertex Coloring Prob
		::Color Vertex = Theme_Light;
		::Color VertexBorder = Color_Font;
		::Color VertexLabel = Color_Font;
		::Color VertexWeight = WHITE;

		::Color VertexHover = Color_Hovering;
		::Color VertexBorderHover = Color_Hovering;
		::Color VertexLabelHover = Color_Font;
		::Color VertexWeightHover = WHITE;

		::Color VertexSelected = Color_Selected;
		::Color VertexBorderSelected = Color_Selected;
		::Color VertexLabelSelected = Color_Font;
		::Color VertexWeightSelected = WHITE;

		::Color VertexVisited = Color_Visited;
		::Color VertexBorderVisited = Color_Visited;
		::Color VertexLabelVisited = Color_Font;
		::Color VertexWeightVisited = WHITE;

		//Edge Coloring Prob
		::Color Edge = Color_Font;
		::Color EdgeWeight = Color_Font;
		::Color EdgeArrow = Color_Font;

		::Color EdgeHover = Color_Hovering;
		::Color EdgeWeightHover = Color_Hovering;
		::Color EdgeArrowHover = Color_Hovering;

		::Color EdgeSelected = Color_Selected;
		::Color EdgeWeightSelected = Color_Selected;
		::Color EdgeArrowSelected = Color_Selected;

		::Color EdgeVisited = Color_Visited;
		::Color EdgeWeightVisited = Color_Visited;
		::Color EdgeArrowVisited = Color_Visited;

		::Color EdgeDisabled = Color_Deactive;
		::Color EdgeWeightDisabled = Color_Deactive;
		::Color EdgeArrowDisabled = Color_Deactive;

		void loadColor();
		void ConvertToFloatArray(float* colorArr);
	} GraphColors;

	typedef struct Dimensions {
		float baseWidth = 400;
		float baseHeight = 400;
		float padding = 32;
		float vertexRadius = 20;
		float vertexStroke = 2;
		float vertexWeightGap = 8;
		float edgeStroke = 4;
		float arrowGap = 8;
		float edgeWeightGap = 8;
	} Dimensions;

	typedef struct GraphRenderer {
		GraphTracer* tracer = NULL;
		Font Font = { 0 };
		float Font_Size = 20;
		float Font_Spacing = 2;
		Dimensions dimensions;
		GraphColors color;
		int selectedVertex = -1;
		float offsetX = 0, offsetY = 0;

		Rect getRect();
		void handleMouseDown();
		void handleMouseMove();
		void draw();
		void drawVertex(Vertex& vertex);
		void drawEdge(Edge& edge);
		void RendererSetting(bool& showRendererSetting);
	} GraphRenderer;

	typedef struct GraphTracer {
		GraphRenderer renderer;
		std::vector<Vertex*> vertices;
		std::vector<Edge*> edges;
		bool isDirected = false;
		bool isWeighted = true;
		VIS_Graph_Layout layout = VIS_Graph_Layout_Circle;
		int root = 0;

		void init(float baseWidth = 400, float baseHeight = 400, Font font = { 0 }, float fontSize = 20, float fontSpacing = 2);
		void set(float* array2D, int N, int MaxN = GRAPH_MAXSIZE);
		void directed(bool isDirected = true);
		void weighted(bool isWeighted = true);
		void addVertex(int id, float weight = TRUE_FLT_MIN, Vector2 position = { 0, 0 }, int visitedCount = 0, int selectedCount = 0);
		void updateVertex(int id, float weight = TRUE_FLT_MIN, int visitedCount = 0, int selectedCount = 0, Vector2 position = { -1,-1 });
		void removeVertex(int id);
		void addEdge(int source, int target, float weight = 0, int visitedCount = 0, int selectedCount = 0, bool disabled = false);
		void updateEdge(int source, int target, float weight = 0, int visitedCount = 0, int selectedCount = 0, bool disabled = false);
		void removeEdge(int source, int target);
		Vertex* findVertex(int id);
		Edge* findEdge(int source, int target);
		std::vector<Edge*> findLinkedEdges(int source);
		std::vector<int> findLinkedNodeIds(int source);
		std::vector<Vertex*> findLinkedNodes(int source);
		void setLayout(VIS_Graph_Layout layout = VIS_Graph_Layout_Circle);
		void layoutCircle();
		void layoutTree(int root, bool sorted);
		int recursiveAnalyze(int& maxDepth, std::unordered_map<int, int>& leafCounts, std::unordered_map<int, bool>& marked, int id, int depth);
		void recursivePosition(std::unordered_map<int, int> leafCounts, std::unordered_map<int, bool>& marked, Rect rect, float hGap, float vGap, bool sorted, Vertex* vertex, int h, int v);
		void layoutRandom();
		void calculatePosition(int root = 0, bool sorted = false);
		void visit(int target, int source = -1, float weight = TRUE_FLT_MIN);
		void leave(int target, int source = -1, float weight = TRUE_FLT_MIN);
		void visitOrLeave(bool visit, int target, int source = -1, float weight = TRUE_FLT_MIN);
		void select(int target, int source = -1);
		void deselect(int target, int source = -1);
		void selectOrDeselect(bool select, int target, int source = -1);
		void disable(int target, int source, bool disable = true);
		void leaveAndDeselectAll();
		void clear();
		GraphMemento* save();
		void load(GraphMemento* const m);
	} GraphTracer;

	void Convert(Color color, float* colorArr, int& i);
	void Convert(float* colorArr, int i, ::Color& color);
} // namespace VIS

#endif // VIS_H_

#ifdef VIS_IMPLEMENTATION

namespace VIS {

	void PseudoCodeTracer::init() {
		this->clear();
		this->renderer.tracer = this;
	}

	void PseudoCodeTracer::select(int line) {
		this->lines[line] = VIS_Pseudo_Color_Selected;
	}

	void PseudoCodeTracer::visit(int line) {
		this->lines[line] = VIS_Pseudo_Color_Visited;
	}

	void PseudoCodeTracer::mark(int line) {
		this->lines[line] = VIS_Pseudo_Color_Marked;
	}

	void PseudoCodeTracer::highlight(int line, VIS_Pseudo_Color color) {
		this->lines[line] = color;
	}

	void PseudoCodeTracer::reset(int line) {
		this->lines[line] = VIS_Pseudo_Color_Default;
	}

	void PseudoCodeTracer::reset() {
		this->lines.clear();
	}

	void PseudoCodeTracer::clear() {
		this->codes.clear();
		this->lines.clear();
	}

	PseudoCodeMemento* PseudoCodeTracer::save() {
		return new PseudoCodeMemento(this->lines);
	}

	void PseudoCodeTracer::load(PseudoCodeMemento* const m) {
		this->lines.clear();
		for (auto& line : m->lines) {
			this->lines[line.first] = line.second;
		}
	}

	PseudoCodeMemento::PseudoCodeMemento(const std::unordered_map<int, VIS_Pseudo_Color> lines) {
		this->lines = lines;
	}

	ImVec4 PseudoCodeColors::getColor(VIS_Pseudo_Color color) {
		ImVec4 c;

		switch (color)
		{
		case VIS_Pseudo_Color_Selected:
			c = rlImGuiColors::Convert(this->Selected);
			break;
		case VIS_Pseudo_Color_Visited:
			c = rlImGuiColors::Convert(this->Visited);
			break;
		case VIS_Pseudo_Color_Marked:
			c = rlImGuiColors::Convert(this->Marked);
			break;
		case VIS_Pseudo_Color_LightBlue:
			c = rlImGuiColors::Convert(this->LightBlue);
			break;
		case VIS_Pseudo_Color_LightGreen:
			c = rlImGuiColors::Convert(this->LightGreen);
			break;
		case VIS_Pseudo_Color_LightRed:
			c = rlImGuiColors::Convert(this->LightRed);
			break;
		case VIS_Pseudo_Color_LightYellow:
			c = rlImGuiColors::Convert(this->LightYellow);
			break;
		case VIS_Pseudo_Color_LightPurple:
			c = rlImGuiColors::Convert(this->LightPurple);
			break;
		case VIS_Pseudo_Color_LightOrange:
			c = rlImGuiColors::Convert(this->LightOrange);
			break;
		case VIS_Pseudo_Color_LightBrown:
			c = rlImGuiColors::Convert(this->LightBrown);
			break;
		case VIS_Pseudo_Color_LightPink:
			c = rlImGuiColors::Convert(this->LightPink);
			break;
		case VIS_Pseudo_Color_LightGray:
			c = rlImGuiColors::Convert(this->LightGray);
			break;
		case VIS_Pseudo_Color_LightTeal:
			c = rlImGuiColors::Convert(this->LightTeal);
			break;
		case VIS_Pseudo_Color_LightLime:
			c = rlImGuiColors::Convert(this->LightLime);
			break;
		case VIS_Pseudo_Color_LightMagenta:
			c = rlImGuiColors::Convert(this->LightMagenta);
			break;
		default:
			c = rlImGuiColors::Convert(WHITE);
			break;
		}
		return c;
	}

	void PseudoCodeRenderer::draw() {
		if (this->tracer->codes.size() == 0)
			return;
		LUI_DefaultWindowPos(-3, ImGuiCond_Appearing, { 0.f, 0.3f });
		ImGui::SetNextWindowBgAlpha(0.35f);
		if (ImGui::Begin("Pseudo Code", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize)) {
			for (int i = 0; i < this->tracer->codes.size(); i++) {
				ImVec4 color = this->color.getColor(this->tracer->lines[i]);
				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::Text("%s", this->tracer->codes[i].c_str());
				ImGui::PopStyleColor();
			}
		}
		ImGui::End();
	}

	void TableTracer::init() {
		this->clear();
		this->renderer.tracer = this;
	}

	void TableTracer::set(float* array2D, int N, int MaxN) {
		this->clear();
		if (!array2D || N <= 0 || MaxN < N)
			return;
		this->addTable(N);
		this->tables[0].set(array2D, N, MaxN);
	}

	void TableTracer::set(int* array2D, int N, int MaxN) {
		this->clear();
		if (!array2D || N <= 0 || MaxN < N)
			return;
		this->addTable(N);
		this->tables[0].set(array2D, N, MaxN);
	}

	void TableTracer::addTable(int cols, std::string title) {
		Table table;
		table.init(cols, title);
		this->tables.push_back(table);
	}

	void TableTracer::updateTable(int index, int cols, std::string title) {
		if (index < 0 || index >= this->tables.size()) return;
		this->tables[index].init(cols, title);
	}

	void TableTracer::removeTable(int index) {
		if (index < 0 || index >= this->tables.size()) return;
		this->tables.erase(this->tables.begin() + index);
	}

	void TableTracer::select(int tableIndex, int row, int col) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].select(row, col);
	}

	void TableTracer::deselect(int tableIndex, int row, int col) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].deselect(row, col);
	}

	void TableTracer::selectCol(int tableIndex, int col) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].selectCol(col);
	}

	void TableTracer::deselectCol(int tableIndex, int col) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].deselectCol(col);
	}

	void TableTracer::selectRow(int tableIndex, int row) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].selectRow(row);
	}

	void TableTracer::deselectRow(int tableIndex, int row) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].deselectRow(row);
	}

	void TableTracer::disable(int tableIndex, int row, int col, bool disable) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].disable(row, col, disable);
	}

	void TableTracer::disableCol(int tableIndex, int col, bool disable) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].disableCol(col, disable);
	}

	void TableTracer::disableRow(int tableIndex, int row, bool disable) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].disableRow(row, disable);
	}

	void TableTracer::patch(int tableIndex, int row, int col) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].patch(row, col);
	}

	void TableTracer::depatch(int tableIndex, int row, int col) {
		if (tableIndex < 0 || tableIndex >= this->tables.size()) return;
		this->tables[tableIndex].depatch(row, col);
	}

	void TableTracer::clear() {
		for (Table& table : this->tables) {
			table.clear();
		}
		this->tables.clear();
	}

	TableMemento* TableTracer::save() {
		return new TableMemento(this->tables);
	}

	void TableTracer::load(TableMemento* const m) {
		this->tables.clear();
		for (Table table : m->tables) {
			this->tables.push_back(table);
		}
	}

	TableCell::TableCell() {
		this->text = "##";
		this->selected = false;
		this->patched = false;
		this->isDisable = false;
	}

	TableCell& TableCell::operator=(const TableCell& other) {
		this->text = other.text;
		this->selected = other.selected;
		this->patched = other.patched;
		this->marked = other.marked;
		this->isDisable = other.isDisable;
		return *this;
	}

	TableCell& TableCell::operator=(const TableCell* other) {
		this->text = other->text;
		this->selected = other->selected;
		this->patched = other->patched;
		this->marked = other->marked;
		this->isDisable = other->isDisable;
		return *this;
	}

	void Table::init(int cols, std::string title) {
		this->cols = cols;
		this->title = title;
		this->create(cols);
	}

	void Table::set(float* array2D, int N, int MaxN) {
		this->create(N);
		if (!array2D || N <= 0 || MaxN < N)
			return;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				float value = array2D[i * MaxN + j];
				this->data[i][j].text = FloatEquals(value, TRUE_FLT_MIN) ? "" : FloatEquals(value, FLT_MAX) ? "INF" : TextFormat("%g", value);
			}
		}
	}

	void Table::set(float** array2D, int rows, int cols) {
		this->cols = cols;
		this->data.clear();
		if (!array2D || cols <= 0 || rows <= 0)
			return;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				float value = array2D[i][j];
				this->data[i][j].text = FloatEquals(value, TRUE_FLT_MIN) ? "" : FloatEquals(value, FLT_MAX) ? "INF" : TextFormat("%g", value);
			}
		}
	}

	void Table::set(int* array2D, int N, int MaxN) {
		this->create(N);
		if (!array2D || N <= 0 || MaxN < N)
			return;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int value = array2D[i * MaxN + j];
				this->data[i][j].text = value == INT_MIN ? "" : value == INT_MAX ? "INF" : TextFormat("%d", value);
			}
		}
	}

	void Table::set(int** array2D, int rows, int cols) {
		this->cols = cols;
		this->data.clear();
		if (!array2D || cols <= 0 || rows <= 0)
			return;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				int value = array2D[i][j];
				this->data[i][j].text = value == INT_MIN ? "" : value == INT_MAX ? "INF" : value == -1 ? "null" : TextFormat("%d", value);
			}
		}
	}

	void Table::create(int cols) {
		this->cols = cols;
		this->data.clear();
		this->rowHeaders.clear();
		this->colHeaders.clear();
		this->footer.clear();
	}

	void Table::select(int row, int col) {
		this->data[row][col].selected = true;
	}

	void Table::deselect(int row, int col) {
		this->data[row][col].selected = false;
	}

	void Table::selectCol(int col) {
		for (int i = 0; i < this->data.size(); i++) {
			this->data[i][col].selected = true;
		}
	}

	void Table::deselectCol(int col) {
		for (int i = 0; i < this->data.size(); i++) {
			this->data[i][col].selected = false;
		}
	}

	void Table::selectRow(int row) {
		for (int j = 0; j < this->cols; j++) {
			this->data[row][j].selected = true;
		}
	}

	void Table::deselectRow(int row) {
		for (int j = 0; j < this->cols; j++) {
			this->data[row][j].selected = false;
		}
	}

	void Table::disable(int row, int col, bool disable) {
		this->data[row][col].isDisable = disable;
	}

	void Table::disableCol(int col, bool disable) {
		for (int i = 0; i < this->data.size(); i++) {
			this->data[i][col].isDisable = disable;
		}
	}

	void Table::disableRow(int row, bool disable) {
		for (int j = 0; j < this->cols; j++) {
			this->data[row][j].isDisable = disable;
		}
	}

	void Table::patch(int row, int col) {
		this->data[row][col].patched = true;
	}

	void Table::depatch(int row, int col) {
		this->data[row][col].patched = false;
	}

	void Table::mark(int row, int col) {
		this->data[row][col].marked = true;
	}

	void Table::unmark(int row, int col) {
		this->data[row][col].marked = false;
	}

	void Table::clear() {
		this->data.clear();
		this->rowHeaders.clear();
		this->colHeaders.clear();
		this->footer.clear();
	}

	TableMemento::TableMemento(const std::vector<Table> tables) {
		for (Table table : tables) {
			this->tables.push_back(table);
		}
	}

	void TableRenderer::draw() {
		static bool resized = false;
		int rows = 0;
		if (this->tracer->tables.size() == 0)
			resized = false;
		for (Table& table : this->tracer->tables) {
			rows += (int)(table.data.size() < 8 ? table.data.size() : 8) + (int)table.footer.size() + (table.showColHeaders ? 1 : 0);
		}
		ImVec2 size = { 500, ImGui::GetTextLineHeightWithSpacing() * (2 + rows) + 15 + (this->tracer->tables.size() > 1 ? 15 : 0) };
		int n = 0;
		for (Table& table : this->tracer->tables) {
			LUI_DefaultWindowPos(3);
			ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);
			if (!resized)
				ImGui::SetNextWindowSize(size, ImGuiCond_Always);
			if (ImGui::Begin(table.title.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings)) {
				if (!FloatEquals(size.x, ImGui::GetWindowWidth()) || !FloatEquals(size.y, ImGui::GetWindowHeight()))
					resized = true;
				else
					resized = false;
				for (std::string& footer : table.footer) {
					ImGui::TextColored(rlImGuiColors::Convert(table.FooterColor), footer.c_str());
				}
				ImGui::Spacing();
				if (table.cols < 1) {
					ImGui::End();
					continue;
				}
				if (ImGui::BeginTable(TextFormat("Table%d", n++), table.cols + (table.showRowHeaders ? 1 : 0), ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, { 0.0f , ImGui::GetTextLineHeightWithSpacing() * (table.data.size() < 8 ? (table.data.size() + (table.showColHeaders ? 1 : 0)) : 9) + (table.cols > 8 ? 15 : 0) + (table.cols < 8 && table.data.size() < 8 ? 15 : 0) }, table.cols < 8 ? 500.f : 60.f * table.cols)) {
					ImGui::TableSetupScrollFreeze(table.showRowHeaders ? 1 : 0, table.showColHeaders ? 1 : 0);
					if (table.showRowHeaders) {
						ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_NoHide);
					}
					if (table.showColHeaders) {
						for (int j = 0; j < table.cols; j++) {
							ImGui::TableSetupColumn(table.colHeaders[j].c_str());
						}
						ImGui::TableSetupScrollFreeze(table.showRowHeaders ? 1 : 0, table.showColHeaders ? 1 : 0);
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.57f, 0.86f, 0.95f, 1.00f));
						ImGui::TableHeadersRow();
						ImGui::PopStyleColor();
					}
					for (int i = 0; i < table.data.size(); i++) {
						ImGui::TableNextRow();
						if (table.showRowHeaders) {
							ImGui::TableSetColumnIndex(0);
							ImGui::TextColored(ImVec4(0.57f, 0.86f, 0.95f, 1.00f), table.rowHeaders[i].c_str());
							ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.19f, 0.19f, 0.20f, 1.00f)));
						}
						for (int j = 0; j < table.cols; j++) {
							ImGui::TableSetColumnIndex(j + (table.showRowHeaders ? 1 : 0));
							TableCell& cell = table.data[i][j];
							if (cell.isDisable)
								ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.19f, 0.19f, 0.42f, 1.00f)));
							if (cell.patched)
								ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(this->color.CellPatched));
							if (cell.selected)
								ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(this->color.CellSelected));
							if (cell.marked)
								ImGui::PushStyleColor(ImGuiCol_Text, rlImGuiColors::Convert(this->color.CellMarked));
							if (strcmp(cell.text.c_str(), "##"))
								ImGui::Text(cell.text.c_str());
							if (cell.marked)
								ImGui::PopStyleColor();
							if (cell.selected)
								ImGui::PopStyleColor();
							if (cell.patched)
								ImGui::PopStyleColor();
						}
					}
					ImGui::EndTable();
				}
			}
			ImGui::End();
		}
	}

	GraphMemento::GraphMemento(const std::vector<Vertex*> vertices, const std::vector<Edge*> edges) {
		for (Vertex* vertex : vertices) {
			Vertex v;
			v = vertex;
			this->vertices.push_back(v);
		}
		for (Edge* edge : edges) {
			Edge e;
			e = edge;
			this->edges.push_back(e);
		}
	}

	Vertex& Vertex::operator=(const Vertex& other) {
		this->id = other.id;
		this->weight = other.weight;
		this->visitedCount = other.visitedCount;
		this->selectedCount = other.selectedCount;
		this->dragging = other.dragging;
		this->hovering = other.hovering;
		return *this;
	}

	Vertex& Vertex::operator=(const Vertex* other) {
		this->id = other->id;
		this->weight = other->weight;
		this->visitedCount = other->visitedCount;
		this->selectedCount = other->selectedCount;
		this->dragging = other->dragging;
		this->hovering = other->hovering;
		return *this;
	}

	Edge& Edge::operator=(const Edge& other) {
		this->source = other.source;
		this->target = other.target;
		this->weight = other.weight;
		this->visitedCount = other.visitedCount;
		this->selectedCount = other.selectedCount;
		this->disabled = other.disabled;
		this->hovering = other.hovering;
		return *this;
	}

	Edge& Edge::operator=(const Edge* other) {
		this->source = other->source;
		this->target = other->target;
		this->weight = other->weight;
		this->visitedCount = other->visitedCount;
		this->selectedCount = other->selectedCount;
		this->disabled = other->disabled;
		this->hovering = other->hovering;
		return *this;
	}

	void GraphTracer::init(float baseWidth, float baseHeight, Font font, float fontSize, float fontSpacing) {
		this->clear();
		this->renderer.tracer = this;
		this->renderer.Font = font;
		this->renderer.Font_Size = fontSize;
		this->renderer.Font_Spacing = fontSpacing;
		this->renderer.dimensions.baseWidth = baseWidth;
		this->renderer.dimensions.baseHeight = baseHeight;
	}

	void GraphColors::loadColor() {
		//Vertex Coloring Prob
		this->Vertex = Theme_Light;
		this->VertexBorder = Color_Font;
		this->VertexLabel = Color_Font;
		this->VertexWeight = WHITE;

		this->VertexSelected = Color_Selected;
		this->VertexBorderSelected = Color_Selected;
		this->VertexLabelSelected = Color_Font;
		this->VertexWeightSelected = WHITE;

		this->VertexVisited = Color_Visited;
		this->VertexBorderVisited = Color_Visited;
		this->VertexLabelVisited = Color_Font;
		this->VertexWeightVisited = WHITE;

		this->VertexHover = Color_Hovering;
		this->VertexBorderHover = Color_Hovering;
		this->VertexLabelHover = Color_Font;
		this->VertexWeightHover = WHITE;

		//Edge Coloring Prob
		this->Edge = Color_Font;
		this->EdgeWeight = Color_Font;
		this->EdgeArrow = Color_Font;

		this->EdgeSelected = Color_Selected;
		this->EdgeWeightSelected = Color_Selected;
		this->EdgeArrowSelected = Color_Selected;

		this->EdgeVisited = Color_Visited;
		this->EdgeWeightVisited = Color_Visited;
		this->EdgeArrowVisited = Color_Visited;

		this->EdgeHover = Color_Hovering;
		this->EdgeWeightHover = Color_Hovering;
		this->EdgeArrowHover = Color_Hovering;
	}

	void GraphTracer::set(float* array2D, int N, int MaxN) {
		this->clear();
		if (!array2D || N <= 0 || MaxN < N)
			return;
		for (int i = 0; i < N; i++) {
			this->addVertex(i);
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				float weight = array2D[i * MaxN + j];
				if (!FloatEquals(weight, 0))
					this->addEdge(i, j, weight);
			}
		}
		this->calculatePosition(this->root);
	}

	void GraphTracer::directed(bool isDirected) {
		this->isDirected = isDirected;
	}

	void GraphTracer::weighted(bool isWeighted) {
		this->isWeighted = isWeighted;
	}

	void GraphTracer::addVertex(int id, float weight, Vector2 position, int visitedCount, int selectedCount) {
		if (this->findVertex(id)) return;
		Vertex* vertex = new Vertex();
		vertex->id = id;
		vertex->weight = weight;
		vertex->position = position;
		vertex->visitedCount = visitedCount;
		vertex->selectedCount = selectedCount;
		this->vertices.push_back(vertex);
	}

	void GraphTracer::updateVertex(int id, float weight, int visitedCount, int selectedCount, Vector2 position) {
		Vertex* vertex = this->findVertex(id);
		if (!vertex) return;
		vertex->weight = weight;
		vertex->visitedCount = visitedCount;
		vertex->selectedCount = selectedCount;
		if (!Vector2Equals(position, { -1,-1 }))
			vertex->position = position;
	}

	void GraphTracer::removeVertex(int id) {
		for (int i = 0; i < this->vertices.size(); i++) {
			if (this->vertices[i]->id == id) {
				delete this->vertices[i];
				this->vertices.erase(this->vertices.begin() + i);
				break;
			}
		}
	}

	void GraphTracer::addEdge(int source, int target, float weight, int visitedCount, int selectedCount, bool disabled) {
		if (findEdge(source, target) && !this->isDirected) return;
		Edge* edge = new Edge();
		edge->source = source;
		edge->target = target;
		edge->weight = weight;
		edge->visitedCount = visitedCount;
		edge->selectedCount = selectedCount;
		edge->disabled = disabled;
		this->edges.push_back(edge);
	}

	void GraphTracer::updateEdge(int source, int target, float weight, int visitedCount, int selectedCount, bool disabled) {
		Edge* edge = findEdge(source, target);
		if (!edge) return;
		edge->weight = weight;
		edge->visitedCount = visitedCount;
		edge->selectedCount = selectedCount;
		edge->disabled = disabled;
	}

	void GraphTracer::removeEdge(int source, int target) {
		for (int i = 0; i < this->edges.size(); i++) {
			if (this->edges[i]->source == source && this->edges[i]->target == target) {
				delete this->edges[i];
				this->edges.erase(this->edges.begin() + i);
				break;
			}
		}
	}

	Vertex* GraphTracer::findVertex(int id) {
		if (id < 0) return NULL;
		for (int i = 0; i < this->vertices.size(); i++) {
			if (this->vertices[i]->id == id) return this->vertices[i];
		}
		return NULL;
	}

	Edge* GraphTracer::findEdge(int source, int target) {
		if (source < 0 || target < 0) return NULL;
		if (this->isDirected)
			for (int i = 0; i < this->edges.size(); i++) {
				if (this->edges[i]->source == source && this->edges[i]->target == target) return this->edges[i];
			}
		else
			for (int i = 0; i < this->edges.size(); i++) {
				if ((this->edges[i]->source == source && this->edges[i]->target == target) || (this->edges[i]->source == target && this->edges[i]->target == source)) return this->edges[i];
			}
		return NULL;
	}

	std::vector<Edge*> GraphTracer::findLinkedEdges(int source) {
		std::vector<Edge*> linkedEdges;
		if (this->isDirected) {
			std::copy_if(this->edges.begin(), this->edges.end(), std::back_inserter(linkedEdges), [source](Edge* edge) { return edge->source == source; });
		}
		else {
			std::copy_if(this->edges.begin(), this->edges.end(), std::back_inserter(linkedEdges), [source](Edge* edge) { return edge->source == source || edge->target == source; });
		}

		return linkedEdges;
	}

	std::vector<int> GraphTracer::findLinkedNodeIds(int source) {
		std::vector<Edge*> linkedEdges = this->findLinkedEdges(source);
		std::vector<int> linkedNodeIds;
		std::transform(linkedEdges.begin(), linkedEdges.end(), std::back_inserter(linkedNodeIds), [source](Edge* edge) { return edge->source == source ? edge->target : edge->source; });
		return linkedNodeIds;
	}

	std::vector<Vertex*> GraphTracer::findLinkedNodes(int source) {
		std::vector<int> linkedNodeIds = this->findLinkedNodeIds(source);
		std::vector<Vertex*> linkedNodes;
		std::transform(linkedNodeIds.begin(), linkedNodeIds.end(), std::back_inserter(linkedNodes), [this](int id) { return this->findVertex(id); });
		return linkedNodes;
	}

	Rect GraphRenderer::getRect() {
		Rect rect{};
		rect.left = -this->dimensions.baseWidth / 2 + this->dimensions.padding;
		rect.top = -this->dimensions.baseHeight / 2 + this->dimensions.padding;
		rect.right = this->dimensions.baseWidth / 2 - this->dimensions.padding;
		rect.bottom = this->dimensions.baseHeight / 2 - this->dimensions.padding;
		rect.width = rect.right - rect.left;
		rect.height = rect.bottom - rect.top;
		return rect;
	}

	void GraphTracer::setLayout(VIS_Graph_Layout layout) {
		this->layout = layout;
	}

	void GraphTracer::layoutCircle() {
		Rect rect = this->renderer.getRect();
		float unitAngle = 2 * PI / this->vertices.size();
		float angle = -PI / 2;
		for (Vertex*& vertex : this->vertices) {
			vertex->position.x = cos(angle) * rect.width / 2;
			vertex->position.y = sin(angle) * rect.height / 2;
			angle += unitAngle;
		}
	}

	void GraphTracer::layoutTree(int root, bool sorted) {
		Rect rect = this->renderer.getRect();
		if (this->vertices.size() == 1) {
			this->vertices[0]->position.x = rect.left + rect.right / 2;
			this->vertices[0]->position.y = rect.top + rect.bottom / 2;
			return;
		}
		int maxDepth = 0;
		std::unordered_map<int, int> leafCounts;
		std::unordered_map<int, bool> marked;
		this->recursiveAnalyze(maxDepth, leafCounts, marked, root, 0);
		float hGap = rect.width / leafCounts[root];
		float vGap = rect.height / maxDepth;
		marked.clear();
		Vertex* rootVertex = this->findVertex(root);
		this->recursivePosition(leafCounts, marked, rect, hGap, vGap, sorted, rootVertex, 0, 0);
	}

	int GraphTracer::recursiveAnalyze(int& maxDepth, std::unordered_map<int, int>& leafCounts, std::unordered_map<int, bool>& marked, int id, int depth) {
		marked[id] = true;
		leafCounts[id] = 0;
		if (maxDepth < depth) maxDepth = depth;
		std::vector<int> linkedNodeIds = this->findLinkedNodeIds(id);
		for (int linkedNodeId : linkedNodeIds) {
			if (marked[linkedNodeId]) continue;
			leafCounts[id] += this->recursiveAnalyze(maxDepth, leafCounts, marked, linkedNodeId, depth + 1);
		}
		if (leafCounts[id] == 0) leafCounts[id] = 1;
		return leafCounts[id];
	}

	void GraphTracer::recursivePosition(std::unordered_map<int, int> leafCounts, std::unordered_map<int, bool>& marked, Rect rect, float hGap, float vGap, bool sorted, Vertex* vertex, int h, int v) {
		marked[vertex->id] = true;
		vertex->position.x = rect.left + (h + leafCounts[vertex->id] / 2) * hGap;
		vertex->position.y = rect.top + v * vGap;
		std::vector<Vertex*> linkedNodes = this->findLinkedNodes(vertex->id);
		if (sorted) {
			std::sort(linkedNodes.begin(), linkedNodes.end(), [](Vertex* a, Vertex* b) { return a->id < b->id; });
		}
		for (Vertex* linkedNode : linkedNodes) {
			if (marked[linkedNode->id]) continue;
			this->recursivePosition(leafCounts, marked, rect, hGap, vGap, sorted, linkedNode, h, v + 1);
			h += leafCounts[linkedNode->id];
		}
	}

	void GraphTracer::layoutRandom() {
		Rect rect = this->renderer.getRect();
		std::vector<Vertex*> placedVertices;
		for (Vertex*& vertex : this->vertices) {
			do {
				vertex->position.x = rect.left + (float)rand() / (float)RAND_MAX * rect.width;
				vertex->position.y = rect.top + (float)rand() / (float)RAND_MAX * rect.height;
			} while (std::any_of(placedVertices.begin(), placedVertices.end(), [vertex](Vertex* placedVertex) { return Vector2Distance(vertex->position, placedVertex->position) < 48; }));
			placedVertices.push_back(vertex);
		}
	}

	void GraphTracer::calculatePosition(int root, bool sorted) {
		switch (this->layout) {
		case VIS_Graph_Layout_Circle:
			this->layoutCircle();
			break;
		case VIS_Graph_Layout_Tree:
			this->layoutTree(root, sorted);
			break;
		case VIS_Graph_Layout_Random:
			this->layoutRandom();
			break;
		}
		return;
	}

	void GraphTracer::visit(int target, int source, float weight) {
		this->visitOrLeave(true, target, source, weight);
	}

	void GraphTracer::leave(int target, int source, float weight) {
		this->visitOrLeave(false, target, source, weight);
	}

	void GraphTracer::visitOrLeave(bool visit, int target, int source, float weight) {
		Edge* edge = this->findEdge(source, target);
		if (edge) edge->visitedCount += visit ? 1 : -1;
		Vertex* vertex = this->findVertex(target);
		if (vertex) {
			if (!FloatEquals(weight, TRUE_FLT_MIN))
				vertex->weight = weight;
			vertex->visitedCount += visit ? 1 : -1;
		}
	}

	void GraphTracer::select(int target, int source) {
		this->selectOrDeselect(true, target, source);
	}

	void GraphTracer::deselect(int target, int source) {
		this->selectOrDeselect(false, target, source);
	}

	void GraphTracer::selectOrDeselect(bool select, int target, int source) {
		Edge* edge = this->findEdge(source, target);
		if (edge) edge->selectedCount += select ? 1 : -1;
		Vertex* vertex = this->findVertex(target);
		if (vertex) vertex->selectedCount += select ? 1 : -1;
	}

	void GraphTracer::disable(int target, int source, bool disable) {
		Edge* edge = this->findEdge(source, target);
		if (edge) edge->disabled = disable;
	}

	void GraphTracer::leaveAndDeselectAll() {
		for (Edge*& edge : this->edges) {
			edge->visitedCount = 0;
			edge->selectedCount = 0;
			edge->disabled = false;
		}
		for (Vertex*& vertex : this->vertices) {
			vertex->visitedCount = 0;
			vertex->selectedCount = 0;
			vertex->weight = TRUE_FLT_MIN;
		}
	}

	void GraphTracer::clear() {
		for (Vertex*& vertex : this->vertices) {
			delete vertex;
		}
		for (Edge*& edge : this->edges) {
			delete edge;
		}
		this->edges.clear();
		this->vertices.clear();
	}

	GraphMemento* GraphTracer::save() {
		return new GraphMemento(this->vertices, this->edges);
	}

	void GraphTracer::load(GraphMemento* const m) {
		for (Vertex& vertex : m->vertices) {
			this->updateVertex(vertex.id, vertex.weight, vertex.visitedCount, vertex.selectedCount);
		}
		for (Edge& edge : m->edges) {
			this->updateEdge(edge.source, edge.target, edge.weight, edge.visitedCount, edge.selectedCount, edge.disabled);
		}
	}

	void GraphRenderer::handleMouseDown() {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePosition = GetMousePosition();
			for (int i = 0; i < this->tracer->vertices.size(); i++) {
				if (this->selectedVertex == -1 && CheckCollisionPointCircle(mousePosition, { this->tracer->vertices[i]->position.x + GetScreenWidth() / 2 + this->offsetX, this->tracer->vertices[i]->position.y + GetScreenHeight() / 2 + this->offsetY }, this->dimensions.vertexRadius)) {
					this->selectedVertex = i;
					SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
					break;
				}
			}
			if (this->selectedVertex == -1)
				this->selectedVertex = -2;
		}
		else {
			this->selectedVertex = -1;
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}
	}

	void GraphRenderer::handleMouseMove() {
		static bool dragging = false;
		static Vector2 lastMousePosition = { 0,0 };
		if (this->selectedVertex != -1 && this->selectedVertex != -2) {
			Vector2 mousePosition = GetMousePosition();
			this->tracer->vertices[this->selectedVertex]->position = { mousePosition.x - GetScreenWidth() / 2 - this->offsetX, mousePosition.y - GetScreenHeight() / 2 - this->offsetY };
		}
		if (this->selectedVertex == -2 && !dragging) {
			lastMousePosition = GetMousePosition();
			dragging = true;
			SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
		}
		if (dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePosition = GetMousePosition();
			this->offsetX += mousePosition.x - lastMousePosition.x;
			this->offsetY += mousePosition.y - lastMousePosition.y;
			lastMousePosition = mousePosition;
		}
		else {
			dragging = false;
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}
	}

	void GraphRenderer::draw() {
		std::sort(this->tracer->edges.begin(), this->tracer->edges.end(), [](Edge* a, Edge* b) { return a->visitedCount < b->visitedCount; });
		for (Edge*& edge : this->tracer->edges) {
			this->drawEdge(*edge);
		}
		for (Vertex*& vertex : this->tracer->vertices) {
			this->drawVertex(*vertex);
		}
	}

	void GraphRenderer::drawVertex(Vertex& vertex) {
		Vector2 mouse_position = GetMousePosition();
		::Color circle, label, border, weight;
		if (vertex.hovering) {
			circle = this->color.VertexHover;
			label = this->color.VertexLabelHover;
			border = this->color.VertexBorderHover;
			weight = this->color.VertexWeightHover;
		}
		else if (vertex.visitedCount > 0) {
			circle = this->color.VertexVisited;
			label = this->color.VertexLabelVisited;
			border = this->color.VertexBorderVisited;
			weight = this->color.VertexWeightVisited;
		}
		else if (vertex.selectedCount > 0) {
			circle = this->color.VertexSelected;
			label = this->color.VertexLabelSelected;
			border = this->color.VertexBorderSelected;
			weight = this->color.VertexWeightSelected;
		}
		else {
			circle = this->color.Vertex;
			label = this->color.VertexLabel;
			border = this->color.VertexBorder;
			weight = this->color.VertexWeight;
		}
		Vector2 pos = { vertex.position.x + GetScreenWidth() / 2 + this->offsetX, vertex.position.y + GetScreenHeight() / 2 + this->offsetY };
		Vector2 origin = MeasureTextEx(this->Font, TextFormat("%d", vertex.id), this->Font_Size, this->Font_Spacing);
		DrawCircleV(pos, this->dimensions.vertexRadius, border);
		DrawCircleV(pos, this->dimensions.vertexRadius - this->dimensions.vertexStroke, circle);
		DrawTextEx(this->Font, TextFormat("%d", vertex.id), { pos.x - origin.x / 2, pos.y - origin.y / 2 }, this->Font_Size, this->Font_Spacing, label);
		if (FloatEquals(vertex.weight, FLT_MAX))
			DrawTextEx(this->Font, "INF", { pos.x + this->dimensions.vertexRadius + this->dimensions.vertexWeightGap,pos.y }, this->Font_Size, this->Font_Spacing, weight);
		else if (!FloatEquals(vertex.weight, TRUE_FLT_MIN))
			DrawTextEx(this->Font, TextFormat("%g", vertex.weight), { pos.x + this->dimensions.vertexRadius + this->dimensions.vertexWeightGap,pos.y }, this->Font_Size, this->Font_Spacing, weight);

		if (CheckCollisionPointCircle(mouse_position, pos, this->dimensions.vertexRadius) && !ImGui::GetIO().WantCaptureMouse) {
			vertex.hovering = true;
		}
		else {
			vertex.hovering = false;
		}

		/*if (vertex.dragging) {
			vertex.position = { mouse_position.x - GetScreenWidth() / 2, mouse_position.y - GetScreenHeight() / 2 };
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (Vector2Distance(mouse_position, pos) <= this->dimensions.vertexRadius) {
				vertex.dragging = true;
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			vertex.dragging = false;
		}*/
	}

	void GraphRenderer::drawEdge(Edge& edge) {
		Vector2 mouse_position = GetMousePosition();
		::Color line, weight, arrow;
		if (edge.hovering) {
			line = this->color.EdgeHover;
			weight = this->color.EdgeWeightHover;
			arrow = this->color.EdgeArrowHover;
		}
		else if (edge.visitedCount > 0) {
			line = this->color.EdgeVisited;
			weight = this->color.EdgeWeightVisited;
			arrow = this->color.EdgeArrowVisited;
		}
		else if (edge.selectedCount > 0) {
			line = this->color.EdgeSelected;
			weight = this->color.EdgeWeightSelected;
			arrow = this->color.EdgeArrowSelected;
		}
		else if (edge.disabled) {
			line = this->color.EdgeDisabled;
			weight = this->color.EdgeWeightDisabled;
			arrow = this->color.EdgeArrowDisabled;
		}
		else {
			line = this->color.Edge;
			weight = this->color.EdgeWeight;
			arrow = this->color.EdgeArrow;
		}
		Vector2 origin = MeasureTextEx(this->Font, TextFormat("%g", edge.weight), this->Font_Size, this->Font_Spacing);
		Vector2 from = { this->tracer->findVertex(edge.source)->position.x + GetScreenWidth() / 2 + this->offsetX, this->tracer->findVertex(edge.source)->position.y + GetScreenHeight() / 2 + this->offsetY };
		Vector2 to = { this->tracer->findVertex(edge.target)->position.x + GetScreenWidth() / 2 + this->offsetX, this->tracer->findVertex(edge.target)->position.y + GetScreenHeight() / 2 + this->offsetY };
		double angle = atan2((double)to.y - from.y, (double)to.x - from.x);
		to.x -= this->dimensions.vertexRadius * (float)cos(angle);
		to.y -= this->dimensions.vertexRadius * (float)sin(angle);
		from.x += this->dimensions.vertexRadius * (float)cos(angle);
		from.y += this->dimensions.vertexRadius * (float)sin(angle);

		if (this->tracer->isWeighted)
			DrawTextPro(this->Font, TextFormat("%g", edge.weight), { from.x + (to.x - from.x) / 2, from.y + (to.y - from.y) / 2 }, { origin.x / 2, origin.y + this->dimensions.edgeWeightGap }, atan2f(to.y - from.y, to.x - from.x) * 180 / PI, this->Font_Size, this->Font_Spacing, weight);
		if (this->tracer->isDirected) {
			if (this->tracer->findEdge(edge.target, edge.source) != NULL) {
				float offset = 5.0f;
				to.x += offset * (float)sin(angle);
				to.y -= offset * (float)cos(angle);
				from.x += offset * (float)sin(angle);
				from.y -= offset * (float)cos(angle);
			}
			angle += PI;
			Vector2 p1 = { (float)(to.x + this->dimensions.arrowGap * cos(angle - 0.8)), (float)(to.y + this->dimensions.arrowGap * sin(angle - 0.8)) };
			Vector2 p2 = { (float)(to.x + this->dimensions.arrowGap * cos(angle + 0.8)), (float)(to.y + this->dimensions.arrowGap * sin(angle + 0.8)) };
			DrawLineEx(to, p1, this->dimensions.edgeStroke, arrow);
			DrawLineEx(to, p2, this->dimensions.edgeStroke, arrow);
		}

		DrawLineEx(from, to, this->dimensions.edgeStroke, line);

		if (CheckCollisionPointLine(mouse_position, from, to, (int)this->dimensions.edgeStroke) && !ImGui::GetIO().WantCaptureMouse) {
			edge.hovering = true;
		}
		else {
			edge.hovering = false;
		}
	}

	void GraphColors::ConvertToFloatArray(float* colorArr) {
		int i = 0;
		Convert(this->Vertex, colorArr, i);
		Convert(this->VertexBorder, colorArr, i);
		Convert(this->VertexLabel, colorArr, i);
		Convert(this->VertexWeight, colorArr, i);

		Convert(this->VertexSelected, colorArr, i);
		Convert(this->VertexBorderSelected, colorArr, i);
		Convert(this->VertexLabelSelected, colorArr, i);
		Convert(this->VertexWeightSelected, colorArr, i);

		Convert(this->VertexVisited, colorArr, i);
		Convert(this->VertexBorderVisited, colorArr, i);
		Convert(this->VertexLabelVisited, colorArr, i);
		Convert(this->VertexWeightVisited, colorArr, i);

		Convert(this->VertexHover, colorArr, i);
		Convert(this->VertexBorderHover, colorArr, i);
		Convert(this->VertexLabelHover, colorArr, i);
		Convert(this->VertexWeightHover, colorArr, i);

		Convert(this->Edge, colorArr, i);
		Convert(this->EdgeWeight, colorArr, i);
		Convert(this->EdgeArrow, colorArr, i);

		Convert(this->EdgeSelected, colorArr, i);
		Convert(this->EdgeWeightSelected, colorArr, i);
		Convert(this->EdgeArrowSelected, colorArr, i);

		Convert(this->EdgeVisited, colorArr, i);
		Convert(this->EdgeWeightVisited, colorArr, i);
		Convert(this->EdgeArrowVisited, colorArr, i);

		Convert(this->EdgeHover, colorArr, i);
		Convert(this->EdgeWeightHover, colorArr, i);
		Convert(this->EdgeArrowHover, colorArr, i);
	}

	void Convert(Color color, float* colorArr, int& i) {
		colorArr[i++] = color.r / 255.0f;
		colorArr[i++] = color.g / 255.0f;
		colorArr[i++] = color.b / 255.0f;
		colorArr[i++] = color.a / 255.0f;
	}

	void Convert(float* colorArr, int i, ::Color& color) {
		color.r = (unsigned char)(colorArr[i++] * 255.0f);
		color.g = (unsigned char)(colorArr[i++] * 255.0f);
		color.b = (unsigned char)(colorArr[i++] * 255.0f);
		color.a = (unsigned char)(colorArr[i++] * 255.0f);
	}

	void GraphRenderer::RendererSetting(bool& showRendererSetting) {
		static bool isConverted = false;
		static float colorArr[28 * 4];
		if (!isConverted) {
			this->color.ConvertToFloatArray(colorArr);
			isConverted = true;
		}
		LUI_DefaultWindowPos(4, ImGuiCond_Appearing);
		ImGui::Begin("Renderer", &showRendererSetting);
		{
			ImGui::SeparatorText("Coloring");
			if (ImGui::TreeNode("Vertex")) {
				if (ImGui::TreeNode("Normal")) {
					if (ImGui::ColorEdit4("Vertex", colorArr + 0)) Convert(colorArr, 0, this->color.Vertex);
					if (ImGui::ColorEdit4("Vertex Border", colorArr + 4)) Convert(colorArr, 4, this->color.VertexBorder);
					if (ImGui::ColorEdit4("Vertex Label", colorArr + 8)) Convert(colorArr, 8, this->color.VertexLabel);
					if (ImGui::ColorEdit4("Vertex Weight", colorArr + 12)) Convert(colorArr, 12, this->color.VertexWeight);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Selected")) {
					if (ImGui::ColorEdit4("Vertex", colorArr + 16)) Convert(colorArr, 16, this->color.VertexSelected);
					if (ImGui::ColorEdit4("Vertex Border", colorArr + 20)) Convert(colorArr, 20, this->color.VertexBorderSelected);
					if (ImGui::ColorEdit4("Vertex Label", colorArr + 24)) Convert(colorArr, 24, this->color.VertexLabelSelected);
					if (ImGui::ColorEdit4("Vertex Weight", colorArr + 28)) Convert(colorArr, 28, this->color.VertexWeightSelected);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Visited")) {
					if (ImGui::ColorEdit4("Vertex", colorArr + 32)) Convert(colorArr, 32, this->color.VertexVisited);
					if (ImGui::ColorEdit4("Vertex Border", colorArr + 36)) Convert(colorArr, 36, this->color.VertexBorderVisited);
					if (ImGui::ColorEdit4("Vertex Label", colorArr + 40)) Convert(colorArr, 40, this->color.VertexLabelVisited);
					if (ImGui::ColorEdit4("Vertex Weight", colorArr + 44)) Convert(colorArr, 44, this->color.VertexWeightVisited);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Hover")) {
					if (ImGui::ColorEdit4("Vertex", colorArr + 48)) Convert(colorArr, 48, this->color.VertexHover);
					if (ImGui::ColorEdit4("Vertex Border", colorArr + 52)) Convert(colorArr, 52, this->color.VertexBorderHover);
					if (ImGui::ColorEdit4("Vertex Label", colorArr + 56)) Convert(colorArr, 56, this->color.VertexLabelHover);
					if (ImGui::ColorEdit4("Vertex Weight", colorArr + 60)) Convert(colorArr, 60, this->color.VertexWeightHover);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Edge")) {
				if (ImGui::TreeNode("Normal")) {
					if (ImGui::ColorEdit4("Edge", colorArr + 64)) Convert(colorArr, 64, this->color.Edge);
					if (ImGui::ColorEdit4("Edge Weight", colorArr + 68)) Convert(colorArr, 68, this->color.EdgeWeight);
					if (ImGui::ColorEdit4("Edge Arrow", colorArr + 72)) Convert(colorArr, 72, this->color.EdgeArrow);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Selected")) {
					if (ImGui::ColorEdit4("Edge", colorArr + 76)) Convert(colorArr, 76, this->color.EdgeSelected);
					if (ImGui::ColorEdit4("Edge Weight", colorArr + 80)) Convert(colorArr, 80, this->color.EdgeWeightSelected);
					if (ImGui::ColorEdit4("Edge Arrow", colorArr + 84)) Convert(colorArr, 84, this->color.EdgeArrowSelected);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Visited")) {
					if (ImGui::ColorEdit4("Edge", colorArr + 88)) Convert(colorArr, 88, this->color.EdgeVisited);
					if (ImGui::ColorEdit4("Edge Weight", colorArr + 92)) Convert(colorArr, 92, this->color.EdgeWeightVisited);
					if (ImGui::ColorEdit4("Edge Arrow", colorArr + 96)) Convert(colorArr, 96, this->color.EdgeArrowVisited);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Hover")) {
					if (ImGui::ColorEdit4("Edge", colorArr + 100)) Convert(colorArr, 100, this->color.EdgeHover);
					if (ImGui::ColorEdit4("Edge Weight", colorArr + 104)) Convert(colorArr, 104, this->color.EdgeWeightHover);
					if (ImGui::ColorEdit4("Edge Arrow", colorArr + 108)) Convert(colorArr, 108, this->color.EdgeArrowHover);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
			if (ImGui::Button("Reset Color", { ImGui::GetContentRegionAvail().x,0 })) {
				this->color.loadColor();
				isConverted = false;
			}
			ImGui::SeparatorText("Size & Spacing");
			ImGui::DragFloat("Font Size", &this->Font_Size, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Font Spacing", &this->Font_Spacing, 0.1f, 0.0f, 100.0f);
			ImGui::Separator();
			ImGui::DragFloat("Vertex Radius", &this->dimensions.vertexRadius, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Vertex Stroke", &this->dimensions.vertexStroke, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Vertex Weight Gap", &this->dimensions.vertexWeightGap, 0.1f, 0.0f, 100.0f);
			ImGui::Separator();
			ImGui::DragFloat("Edge Stroke", &this->dimensions.edgeStroke, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Edge Weight Gap", &this->dimensions.edgeWeightGap, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Arrow Gap", &this->dimensions.arrowGap, 0.1f, 0.0f, 100.0f);
			if (ImGui::Button("Reset Size & Spacing", { ImGui::GetContentRegionAvail().x,0 })) {
				this->Font_Size = 20;
				this->Font_Spacing = 2.0f;
				this->dimensions.vertexRadius = 20;
				this->dimensions.vertexStroke = 2;
				this->dimensions.vertexWeightGap = 8;
				this->dimensions.edgeStroke = 4;
				this->dimensions.edgeWeightGap = 8;
				this->dimensions.arrowGap = 8;
			}
		}
		ImGui::End();
	}

} // namespace VIS

#endif // VIS_IMPLEMENTATION