#pragma once
#include <climits>
#include <memory>
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Utility/SaveManager.h"
#include "SelectScreen.h"
#include "NetworkScreen.h"
#include "Entity/UI/Back.h"
#include "Entity/View/ImageView.h"
#include "Entity/UI/Button/MenuScreenButton.h"
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
		MenuScreen() : nextScreen_() {};
		~MenuScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		constexpr static int TITLE_X = 300; //! タイトルのX座標
		constexpr static int TITLE_Y = 80; //! タイトルのY座標

		// レイヤーの種類
		enum Layer
		{
			MASK,
			UI,
			BACK,
			LEN
		};

		// メニューボタンの種類
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