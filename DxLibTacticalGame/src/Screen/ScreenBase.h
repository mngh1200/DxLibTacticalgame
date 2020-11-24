#pragma once
#include <climits>
#include <memory>
#include "Entity/Object.h"

using namespace std;

namespace Screen
{
	class ScreenBase
	{
	public:
		ScreenBase() : isInited_(false) {};
		~ScreenBase() {};

		/**
		 * @fn
		 * ��������
		*/
		virtual void init() = 0;

		/**
		 * @fn
		 * �C�x���g�擾��̃X�N���[���X�V����
		*/
		virtual void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int button, int eventType, int x, int y) = 0;
		
		/**
		 * @fn
		 * �A�j���[�V�����I����̃X�N���[���X�V����
		*/
		virtual void updateByAnimation() = 0;
	private:
		//! �����������������Ă��邩
		bool isInited_;
	};
}