#pragma once
#include <climits>
#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include "Define.h"
#include "DxLib.h"
#include "Controller.h"
#include "Entity/Object.h"
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
		
		//! �e���C���[�̃I�u�W�F�N�g�Ǘ��p�ϐ� vector(�e���C���[)�Alist(�e���C���[�ŊǗ����Ă���Object)
		vector<list<shared_ptr<Entity::Object>>> layerObjList;

	private:
		Game() : frame_(0), prevTime_(0) {};
		~Game() {};
		void controlFps();

		//! �t���[���J�E���g
		int frame_;

		//! ��������
		int waitTime_;

		//! �O�񎞍�
		int prevTime_;

		//! �e�X�g�p
		bool isInit = true;
	};
}