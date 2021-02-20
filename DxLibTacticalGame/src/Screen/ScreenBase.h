#pragma once
#include <climits>
#include <memory>
#include "Entity/Object.h"


using namespace std;

/**
 * @file ScreenBase.h
 * @brief �e��ʂ̐i�s���Ǘ�����N���X�̊��N���X
 */

namespace FrameWork
{
	class Game;
}

namespace Entity
{
	class Overlay;
}

namespace
{
	constexpr int OVERLAY_ID = 99999;
	constexpr int START_OVERLAY_SCENE = -1;
	constexpr int END_OVERLAY_SCENE = -2;
}

namespace Screen
{
	class ScreenBase
	{
	public:
		ScreenBase() : isInited_(false), nowScene_(START_OVERLAY_SCENE) {};
		virtual ~ScreenBase() {};

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
		void createOverlay(bool isOpen);

		bool isOpenOverlayEnded();
		bool isCloseOverlayEnded();

		//! ���݂̃V�[����ێ�
		int nowScene_;

	private:
		//! �����������������Ă��邩
		bool isInited_;
	};
}