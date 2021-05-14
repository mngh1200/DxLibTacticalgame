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
		NetworkScreen() : 
			nextScreen_(), hostManager_{}, clientManager_{}, 
			initScene_(Scene::INIT), 
			inheritNetHandle_(-1),
			inheritMapId_(-1),
			inheritUnitNum_(-1)
		{};
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

		void setRetryParam(bool isServer, int netHandle, int mapId, int unitNum);
	private:
		void startBattle(int netHandler_, const RuleData& ruleData, bool isServer);

		constexpr static int BUTTON_W = 600;	//! ボタン幅
		constexpr static int BUTTON_H = 100;		//! ボタン高さ
		constexpr static int BUTTON_X = (WIN_W - BUTTON_W) / 2;	//! ボタン座標x
		constexpr static int BUTTON_Y = 150;	//! ボタン座標y
		constexpr static int BUTTON_MARGIN = 30;//! ボタン余白

		constexpr static int BACK_BTN_W = 300;	//! 戻るボタンの幅
		constexpr static int BACK_BTN_H = 60;	//! 戻るボタンの高さ
		constexpr static int BACK_BTN_X = (WIN_W - BACK_BTN_W) / 2;	//! 戻るボタン座標x
		constexpr static int BACK_BTN_Y = WIN_H - 200;	//! 戻るボタン座標y

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

		int initScene_;			//! 初期シーン
		int inheritNetHandle_;	//! 前対戦から継承したネットハンドル
		int inheritMapId_;		//! 前対戦から継承したマップID
		int inheritUnitNum_;	//! 前対戦から継承したユニット数

		ScreenBase *nextScreen_; //! 次に開く画面

		NetworkHost hostManager_; //! ネットワーク接続のホストになった時用のクラス
		NetworkClient clientManager_; //! ネットワーク接続のクライアントになった時用のクラス
		RuleSetting ruleSetting_; //! ルール設定時に使用するクラス
	};
}