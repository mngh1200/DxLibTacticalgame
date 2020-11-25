#pragma once
#include <climits>
#include <iostream>
#include <memory>
#include <map>
#include "Define.h"
#include "DxLib.h"
#include "Controller.h"
#include "Screen/ScreenBase.h"
#include "Entity/ObjectsControl.h"
#include "Entity/Object.h"

using namespace std;

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
		int process();

		void setScreen(Screen::ScreenBase* newScreen);

		void finish();

		int getFrame() const;
		void increaseFrame();

		//! 各レイヤーのオブジェクト管理
		Entity::ObjectsControl objectsControl;


	private:
		Game() : frame_(0), prevTime_(0), isFinish_(false), waitTime_(0), nowScreen_() {};
		~Game() {};
		void controlFps();

		//! フレームカウント
		int frame_;

		//! 処理時間
		int waitTime_;

		//! 前回時刻
		int prevTime_;

		//! 終了判定
		bool isFinish_;

		//! 画面制御管理クラス
		unique_ptr<Screen::ScreenBase> nowScreen_;
	};
}