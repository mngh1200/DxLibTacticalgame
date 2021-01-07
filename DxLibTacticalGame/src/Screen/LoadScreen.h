#pragma once
#include <climits>
#include <memory>
#include "ScreenBase.h"
#include "StartScreen.h"
#include "Entity/UI/Back.h"
#include "Entity/View/Text.h"

using namespace std;

/**
 * @file LoadScreen.h
 * @brief ロード画面の進行を管理するクラス
 */

namespace Screen
{
	class LoadScreen : public ScreenBase
	{
	public:
		LoadScreen() {};
		~LoadScreen() {};

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