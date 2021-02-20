#pragma once
#include <climits>
#include <memory>
#include "ScreenBase.h"
#include "MenuScreen.h"
#include "Entity/UI/Back.h"
#include "Entity/View/Text.h"

using namespace std;

/**
 * @file StartScreen.h
 * @brief スタート画面の進行を管理するクラス
 */

namespace Screen
{
	class StartScreen : public ScreenBase
	{
	public:
		StartScreen() {};
		~StartScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		enum Layer
		{
			MASK,
			TEXT,
			BACK,
			LEN
		};
	};
}