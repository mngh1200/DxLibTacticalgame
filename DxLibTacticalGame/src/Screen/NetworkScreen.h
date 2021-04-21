#pragma once
#include <climits>
#include <memory>
#include "MenuScreen.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/TextBox.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/View/Text.h"
#include "Network/NetworkHost.h"
#include "Network/NetworkClient.h"
#include "Network/RuleSetting.h"

using namespace std;
using namespace Network;

/**
 * @file NetworkScreen.h
 * @brief 通信対戦準備用の画面
 */


namespace Screen
{
	class NetworkScreen : public ScreenBase
	{
	public:
		NetworkScreen() : nextScreen_(), hostManager_{}, clientManager_{} {};
		~NetworkScreen() {};

		// レイヤーの種類
		enum Layer
		{
			MASK,
			MODAL_CONTENT,
			MODAL_FRAME,
			UI,
			BACK,
			LEN
		};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		void startBattle(int netHandler_, const RuleData& ruleData, bool isServer);

		constexpr static int BUTTON_W = 600;	//! ボタン幅
		constexpr static int BUTTON_H = 100;		//! ボタン高さ
		constexpr static int BUTTON_X = (WIN_W - BUTTON_W) / 2;	//! ボタン座標x
		constexpr static int BUTTON_Y = 140;	//! ボタン座標y
		constexpr static int BUTTON_MARGIN = 30;//! ボタン余白

		constexpr static int BACK_BTN_W = 300;	//! 戻るボタンの幅
		constexpr static int BACK_BTN_H = 60;	//! 戻るボタンの高さ
		constexpr static int BACK_BTN_X = (WIN_W - BACK_BTN_W) / 2;	//! 戻るボタン座標x
		constexpr static int BACK_BTN_Y = WIN_H - 120;	//! 戻るボタン座標y

		constexpr static int COMMENT_MARGIN = 10;	//! コメントの余白
		constexpr static int COMMENT_LINE_H = 26;		//! コメントの１行分の高さ

		// シーンの種類
		enum Scene
		{
			INIT,
			HOST,
			CLIENT,
			RULE_SET
		};

		// UIの種類
		enum UIid
		{
			CREATE_ROOM_BUTTON,
			SEARCH_ROOM_BUTTON,
			QUIT_BUTTON
		};

		ScreenBase *nextScreen_; //! 次に開く画面

		NetworkHost hostManager_; //! ネットワーク接続のホストになった時用のクラス
		NetworkClient clientManager_; //! ネットワーク接続のクライアントになった時用のクラス
		RuleSetting ruleSetting_; //! ルール設定時に使用するクラス
	};
}