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

		//! �e���C���[�̃I�u�W�F�N�g�Ǘ�
		Entity::ObjectsControl objectsControl;


	private:
		Game() : frame_(0), prevTime_(0), isFinish_(false), waitTime_(0), nowScreen_() {};
		~Game() {};
		void controlFps();

		//! �t���[���J�E���g
		int frame_;

		//! ��������
		int waitTime_;

		//! �O�񎞍�
		int prevTime_;

		//! �I������
		bool isFinish_;

		//! ��ʐ���Ǘ��N���X
		unique_ptr<Screen::ScreenBase> nowScreen_;
	};
}