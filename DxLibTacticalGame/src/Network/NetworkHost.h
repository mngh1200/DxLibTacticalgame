#pragma once
#include <climits>
#include "FrameWork/Game.h"
#include "NetworkDefine.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/UI/ModalFrame.h"
#include "Entity/View/Text.h"
#include "Utility/Timer.h"

using namespace std;
using namespace Entity;

/**
 * @file NetworkHost.h
 * @brief ネットワーク接続待ち（部屋を作る）側の処理管理
 */

namespace Network
{
	class NetworkHost
	{
	public:
		NetworkHost() : isConnected_(false), clientIp_{}, textUpdataTimer_{}, dotCount_(1) {};
		~NetworkHost() {};

		void start();
		void end();

		// checkAndUpdateの返し値用（Screenクラスに状況を返す）
		enum Result
		{
			CONTINUE,	//! 継続
			CANCEL,		//! キャンセル
			SET_RULE	//! ルール設定へ進む
		};

		int checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType);

	private:
		void checkAcceptNetwork();

		constexpr static int MODAL_W = 600; //! 小画面 幅
		constexpr static int MODAL_H = 330; //! 小画面 高さ
		constexpr static int MODAL_X = (WIN_W - MODAL_W) / 2; //! 小画面 x座標
		constexpr static int MODAL_Y = (WIN_H - MODAL_H) / 2; //! 小画面 y座標

		constexpr static int MODAL_PADDING = 20; //! 小画面余白

		constexpr static int CONTENT_X = MODAL_X + MODAL_PADDING; //! 小画面要素のx座標
		constexpr static int CONTENT_Y = MODAL_Y + MODAL_PADDING + ModalFrame::HEAD_H; //! 小画面要素のy座標

		constexpr static int BUTTON_H = 75; //! ボタンの高さ
		constexpr static int BUTTON_Y = MODAL_Y + MODAL_H - MODAL_PADDING - BUTTON_H; //! ボタンのy座標
		constexpr static int BUTTON_MARGIN_X = 20; //! ボタンの余白
		constexpr static int BUTTON_W = (MODAL_W - MODAL_PADDING * 2 - BUTTON_MARGIN_X) / 2; //! ボタンの幅

		constexpr static int LINE_H = 32; //! 一行サイズ
		constexpr static int LINE_MARGIN = 30; //! 一行の余白

		constexpr static int TIMER_MS = 500; //! 状況テキスト更新タイマー


		const static int LAYER_FRAME; //! フレームのレイヤーID
		const static int LAYER_CONTENT; //! モーダル内の内容のレイヤーID

		const static string WAIT_CONNECT_TEXT; //! 接続待ち状況でのテキスト

		// コンテントのオブジェクトID
		enum ContentId
		{
			CANCEL_BTN
		};

		shared_ptr<Text> statusText_; //! 接続状況を示すテキスト
		shared_ptr<TextButton> ruleSetButton_; //! ルール設定ボタン

		bool isConnected_; //! 接続済みであるか

		IPDATA clientIp_; //! クライアント側のIPアドレス

		int netHandle_; //! ネットワークハンドル

		Utility::Timer textUpdataTimer_; //! 接続状況テキストの状態を更新するためのタイマー

		int dotCount_; //! 接続状況テキストのドット数
	};
}