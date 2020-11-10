#pragma once
#include <climits>
#include "DxLib.h"

namespace
{
	const int FPS = 60;				// FPS
	const int ONE_FRAME_MS = 16;	// millsec per frame (16ms)
}

namespace FrameWork
{
	class Game
	{
	public:
		// for singleton
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;

		static Game& getInstance();
		void control();
		int getFrame() const;
		void increaseFrame();

	private:
		Game() : frame_(0), prevTime_(0) {};
		~Game() {};
		void controlFps();

		int frame_;
		int waitTime_;  // for adjusting fps
		int prevTime_;
	};
}