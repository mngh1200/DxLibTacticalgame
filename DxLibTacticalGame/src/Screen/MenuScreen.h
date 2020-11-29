#pragma once
#include <climits>
#include <memory>
#include "ScreenBase.h"
#include "Utility/FontManager.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/Button.h"
#include "Entity/View/Text.h"

using namespace std;

/**
 * @file MenuScreen.h
 * @brief メニュー画面の進行を管理するクラス
 */

namespace Screen
{
	class MenuScreen : public ScreenBase
	{
	public:
		MenuScreen() {};
		~MenuScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		enum Layer
		{
			MASK,
			UI,
			BACK,
			LEN
		};
	};
}