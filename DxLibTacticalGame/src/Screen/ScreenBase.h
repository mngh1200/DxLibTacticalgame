#pragma once
#include <climits>
#include <memory>
#include "Entity/Object.h"

using namespace std;

namespace FrameWork
{
	class Game;
}

namespace Screen
{
	class ScreenBase
	{
	public:
		ScreenBase() : isInited_(false), nowScene_(-1) {};
		~ScreenBase() {};

		bool isInited() const;
		void inited();

		/**
		 * @fn
		 * ��������
		*/
		virtual void init() = 0;

		/**
		 * @fn
		 * �C�x���g�擾��̃X�N���[���X�V����
		*/
		virtual void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType) = 0;
		
		/**
		 * @fn
		 * �A�j���[�V�����I����̃X�N���[���X�V����
		*/
		virtual void updateByAnimation() = 0;

	protected:
		//! ���݂̃V�[����ێ�
		int nowScene_;

	private:
		//! �����������������Ă��邩
		bool isInited_;
	};
}