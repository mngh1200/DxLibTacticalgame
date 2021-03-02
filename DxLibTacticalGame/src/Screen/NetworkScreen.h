#pragma once
#include <climits>
#include <memory>
#include "MenuScreen.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/View/Text.h"

using namespace std;

/**
 * @file NetworkScreen.h
 * @brief 通信対戦準備用の画面
 */


namespace Screen
{
	class NetworkScreen : public ScreenBase
	{
	public:
		NetworkScreen() : nextScreen_() {};
		~NetworkScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		constexpr static int BUTTON_W = 600;	//! ボタン幅
		constexpr static int BUTTON_H = 100;		//! ボタン高さ
		constexpr static int BUTTON_X = (WIN_W - BUTTON_W) / 2;	//! ボタン座標x
		constexpr static int BUTTON_Y = 200;	//! ボタン座標y
		constexpr static int BUTTON_MARGIN = 30;//! ボタン余白

		constexpr static int BACK_SIZE = 60;	//! 戻るボタンのサイズ

		void createRoom();
		void searchRoom();

		// レイヤーの種類
		enum Layer
		{
			MASK,
			UI,
			BACK,
			LEN
		};

		// UIの種類
		enum UIid
		{
			CREATE_ROOM_BUTTON,
			SEARCH_ROOM_BUTTON,
			QUIT_BUTTON
		};

		ScreenBase *nextScreen_; //! 次に開く画面
	};
}