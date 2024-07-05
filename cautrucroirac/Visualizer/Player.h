#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdio.h>
#include <vector>

#include "../raylib/include/raylib.h"
#include "../raylib/include/raymath.h"
#include "../imgui/imgui.h"
#include "../rlImGui/rlImGui.h"

#include "Visualizer.h"

#define GRAPH player.Graph
#define TABLE player.Table
#define PSEUDO player.Pseudo

namespace Player {
	typedef struct Timer {
		double startTime = 0;   // Start time (seconds)
		double lifeTime = 0;    // Lifetime (seconds)

		void start(double lifetime);
		bool isDone();
		double elapsed();
	} Timer;

	typedef struct State {
		VIS::GraphMemento* GraphMementos;
		VIS::TableMemento* TableMementos;
		VIS::PseudoCodeMemento* PseudoMementos;

		void clear();
	} State;

	typedef struct Player {
		VIS::GraphTracer Graph;
		VIS::TableTracer Table;
		VIS::PseudoCodeTracer Pseudo;
		int CurrentStep = -1;
		std::vector<State> States;
		bool pause = true;
		Timer Timer;

		void init();
		void push();
		void pop();
		void load(int step);
		void run(float speed = 1);
		void clear();
	} Player;

} // namespace Player

#endif // !PLAYER_H_

#ifdef PLAYER_IMPLEMENTATION
namespace Player {
	void Timer::start(double lifetime)
	{
		this->startTime = GetTime();
		this->lifeTime = lifetime;
	}

	bool Timer::isDone()
	{
		return GetTime() - this->startTime >= this->lifeTime;
	}

	double Timer::elapsed()
	{
		return GetTime() - this->startTime;
	}

	void State::clear()
	{
		if (this->GraphMementos != nullptr) {
			this->GraphMementos->edges.clear();
			this->GraphMementos->vertices.clear();
			delete this->GraphMementos;
			this->GraphMementos = nullptr;
		}
		if (this->TableMementos != nullptr) {
			this->TableMementos->tables.clear();
			delete this->TableMementos;
			this->TableMementos = nullptr;
		}
		if (this->PseudoMementos != nullptr) {
			this->PseudoMementos->lines.clear();
			delete this->PseudoMementos;
			this->PseudoMementos = nullptr;
		}
	}

	void Player::init()
	{
		this->Graph.leaveAndDeselectAll();
		this->Table.clear();
		this->Pseudo.clear();
		this->CurrentStep = -1;
		for (State& state : this->States) state.clear();
		this->States.clear();
		this->pause = true;
		this->Timer = { 0, 0 };
	}

	void Player::push()
	{
		State state;
		state.GraphMementos = this->Graph.save();
		state.TableMementos = this->Table.save();
		state.PseudoMementos = this->Pseudo.save();
		this->States.push_back(state);
	}

	void Player::pop()
	{
		if (this->States.size() == 0) return;
		State state = this->States.back();
		this->States.pop_back();
		this->Graph.load(state.GraphMementos);
		this->Table.load(state.TableMementos);
		this->Pseudo.load(state.PseudoMementos);
		state.clear();
	}

	void Player::load(int step)
	{
		if (step < 0 || step > this->States.size() - 1) return;
		this->CurrentStep = step;
		this->Graph.load(this->States[step].GraphMementos);
		this->Table.load(this->States[step].TableMementos);
		this->Pseudo.load(this->States[step].PseudoMementos);
	}

	void Player::run(float speed)
	{
		if (this->pause) return;
		if (speed <= 0) {
			speed = 0;
			this->pause = true;
		}
		if (this->CurrentStep == -1) return;
		if (this->CurrentStep >= this->States.size() - 1) {
			this->pause = true;
			return;
		}
		if (this->Timer.isDone()) {
			this->CurrentStep++;
			this->load(this->CurrentStep);
			this->Timer.start(1 / speed);
		}
	}

	void Player::clear()
	{
		this->Graph.clear();
		this->Table.clear();
		this->Pseudo.clear();
		this->CurrentStep = -1;
		this->States.clear();
		this->pause = true;
		this->Timer = { 0, 0 };
	}

} // namespace Player
#endif // PLAYER_IMPLEMENTATION
