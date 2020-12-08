#pragma once
#include <climits>
#include <memory>
#include "ScreenBase.h"
#include "FrameWork/Game.h"
#include "Utility/FontManager.h"
#include "Entity/UI/Back.h"
#include "Entity/View/Text.h"

using namespace std;

/**
 * @file SelectScreen.h
 * @brief �L�����y�[���Z���N�g���
 */

namespace Screen
{
	class SelectScreen : public ScreenBase
	{
	public:
		SelectScreen() {};
		~SelectScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		const static int LEFT_AREA_WIDTH = 300; //! �����̃G���A�̕�

		enum Layer
		{
			MASK,
			UI,
			BACK,
			LEN
		};

		enum UIid
		{
			BACK_BTN
		};
	};
}