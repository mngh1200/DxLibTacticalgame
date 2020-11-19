#pragma once
#include <climits>
#include <iostream>
#include <memory>
#include <map>
#include "Define.h"
#include "DxLib.h"
#include "Controller.h"
#include "Entity/ObjectsControl.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/Button.h"
#include "Entity/Shape.h"

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
		int getFrame() const;
		void increaseFrame();
		
		//! 各レイヤーのオブジェクト管理
		Entity::ObjectsControl objectsControl;
		

	private:
		Game() : frame_(0), prevTime_(0), waitTime_(0){};
		~Game() {};
		void controlFps();

		//! フレームカウント
		int frame_;

		//! 処理時間
		int waitTime_;

		//! 前回時刻
		int prevTime_;

		//! テスト用
		bool isInit = true;
	};
}