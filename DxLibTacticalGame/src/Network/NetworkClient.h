#pragma once
#include <climits>
#include "FrameWork/Game.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/UI/ModalFrame.h"
#include "Entity/View/Text.h"
#include "Entity/UI/TextBox.h"

using namespace std;
using namespace Entity;

/**
 * @file NetworkClient.h
 * @brief ネットワーク接続待ち（部屋を作る）側の処理管理
 */

namespace Network
{
	class NetworkClient
	{
	public:
		NetworkClient() : state_(State::NONE), netHandle_(-1), ableConnecButton_(false) {};
		~NetworkClient() {};

		void start();
		void end();


		// checkAndUpdateの返し値用（Screenクラスに状況を返す）
		enum Result
		{
			CONTINUE,	//! 継続
			CANCEL,		//! キャンセル
			BATTLE		//! バトル画面へ
		};

		int checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType);

	private:
		void connectNetwork();
		void getIpData(IPDATA& ipData);

		constexpr static int MODAL_W = 600; //! 小画面 幅
		constexpr static int MODAL_H = 400; //! 小画面 高さ
		constexpr static int MODAL_X = (WIN_W - MODAL_W) / 2; //! 小画面 x座標
		constexpr static int MODAL_Y = (WIN_H - MODAL_H) / 2; //! 小画面 y座標

		constexpr static int MODAL_PADDING = 20; //! 小画面余白

		constexpr static int CONTENT_X = MODAL_X + MODAL_PADDING; //! 小画面要素のx座標
		constexpr static int CONTENT_Y = MODAL_Y + MODAL_PADDING + ModalFrame::HEAD_H; //! 小画面要素のy座標

		constexpr static int IP_INPUT_W = 85; //! IPアドレス欄の幅
		constexpr static int IP_INPUT_H = 50;  //! IPアドレス欄の高さ
		constexpr static int IP_INPUT_MARGIN = 20; //! IPアドレス欄の余白
		constexpr static int IP_INPUT_Y = CONTENT_Y + 24 + 20; //! IPアドレス欄のy座標


		constexpr static int BUTTON_H = 75; //! ボタンの高さ
		constexpr static int BUTTON_Y = MODAL_Y + MODAL_H - MODAL_PADDING - BUTTON_H; //! ボタンのy座標
		constexpr static int BUTTON_MARGIN_X = 20; //! ボタンの余白
		constexpr static int BUTTON_W = (MODAL_W - MODAL_PADDING * 2 - BUTTON_MARGIN_X) / 2; //! ボタンの幅

		constexpr static int STATUS_LINE_H = 50; //! 状況の一行の高さ

		const static int LAYER_FRAME; //! フレームのレイヤーID
		const static int LAYER_CONTENT; //! モーダル内の内容のレイヤーID

		shared_ptr<Text> statusText_; //! 接続状況を示すテキスト
		shared_ptr<TextButton> connectButton_; //! 接続ボタン

		vector<shared_ptr<TextBox>> ipAdressInputList_; //! IPアドレス入力欄のリスト

		// コンテントのオブジェクトID
		enum ContentId
		{
			CANCEL_BTN
		};

		// 状況の種類
		enum State
		{
			NONE,
			CONNECTED	//! 接続済み（ホストのルール設定待機中）
		};
		
		void setState(State state);
		void adjustConnectButtonValid();

		State state_; //! 状況

		int netHandle_; //! ネットワークハンドル

		bool ableConnecButton_; //! IPアドレス入力済みであるか
	};


}