#pragma once
#include <climits>
#include <memory>
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Utility/SaveManager.h"
#include "SelectScreen.h"
#include "NetworkScreen.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/Button/MenuScreenButton.h"
#include "Entity/View/Text.h"

using namespace std;

/**
 * @file MenuScreen.h
 * @brief ���j���[��ʂ̐i�s���Ǘ�����N���X
 */

namespace Screen
{
	class MenuScreen : public ScreenBase
	{
	public:
		MenuScreen() : nextScreen_() {};
		~MenuScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		// ���C���[�̎��
		enum Layer
		{
			MASK,
			UI,
			BACK,
			LEN
		};

		// ���j���[�{�^���̎��
		enum UIid
		{
			TITLE,
			CAMPAIN_BUTTON,
			NETWORK_BUTTON,
			QUIT_BUTTON
		};

		ScreenBase* nextScreen_;
	};
}