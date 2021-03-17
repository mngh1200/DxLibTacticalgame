#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Network/SendManager.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/UI/RadioButton.h"
#include "Entity/UI/ModalFrame.h"
#include "Entity/View/Text.h"
#include "Utility/Timer.h"

using namespace std;
using namespace Entity;

/**
 * @file RuleSetting.h
 * @brief 通信対戦のルール設定のUIセットと処理
 */

namespace Network
{
	class RuleSetting
	{
	public:
		RuleSetting() : 
			netHandle_(-1),
			isConnect_(true)
		{};
		~RuleSetting() {};

		void start(int netHandle);
		void end();

		// checkAndUpdateの返し値用（Screenクラスに状況を返す）
		enum Result
		{
			CONTINUE,		//! 継続
			CANCEL,			//! キャンセル
			START_BATTLE	//! バトル画面へ進む
		};

		int checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType);

		const RuleData& getRuleData() const { return ruleData_; }; //! ルールデータを返す
		int getNetHandle() const { return netHandle_; } //! ネットハンドルを返す

	private:
		bool keepRuleData();

		constexpr static int MODAL_W = 600; //! 小画面 幅
		constexpr static int MODAL_H = 600; //! 小画面 高さ
		constexpr static int MODAL_X = (WIN_W - MODAL_W) / 2; //! 小画面 x座標
		constexpr static int MODAL_Y = (WIN_H - MODAL_H) / 2; //! 小画面 y座標

		constexpr static int MODAL_PADDING = 20; //! 小画面余白

		constexpr static int CONTENT_X = MODAL_X + MODAL_PADDING; //! 小画面要素のx座標
		constexpr static int CONTENT_Y = MODAL_Y + MODAL_PADDING + ModalFrame::HEAD_H; //! 小画面要素のy座標

		constexpr static int SELECT_MAP_Y = CONTENT_Y + 140; //! マップ選択のy座標

		constexpr static int RADIO_W = 90; //! ラジオボタンの幅
		constexpr static int RADIO_H = 50;  //! ラジオボタンも高さ

		constexpr static int BUTTON_H = 75; //! ボタンの高さ
		constexpr static int BUTTON_Y = MODAL_Y + MODAL_H - MODAL_PADDING - BUTTON_H; //! ボタンのy座標
		constexpr static int BUTTON_MARGIN_X = 20; //! ボタンの余白
		constexpr static int BUTTON_W = (MODAL_W - MODAL_PADDING * 2 - BUTTON_MARGIN_X) / 2; //! ボタンの幅

		constexpr static int LINE_H = 32; //! 一行サイズ
		constexpr static int LINE_MARGIN = 6; //! 一行の余白


		const static int LAYER_FRAME; //! フレームのレイヤーID
		const static int LAYER_CONTENT; //! モーダル内の内容のレイヤーID


		bool isConnect_; //! 接続状況

		int netHandle_; //! ネットワークハンドル

		RuleData ruleData_; //! 作成したルール設定データ

		SendManager sendManager_; //! データ送信マネージャー

		shared_ptr<Text> statusText_; //! 接続状況を示すテキスト

		shared_ptr<TextButton> startButton_; //! 開始ボタン
		shared_ptr<TextButton> closeButton_; //! 閉じるor切断ボタン

		shared_ptr<RadioButton> unitNum_;   //! ユニット数指定のラジオボタン
		shared_ptr<RadioButton> mapSelect_; //! マップ指定のラジオボタン

	};
}