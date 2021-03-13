#include "NetworkHost.h"
#include "Screen/NetworkScreen.h"

namespace Network
{
	const int NetworkHost::LAYER_CONTENT = Screen::NetworkScreen::Layer::MODAL_CONTENT;
	const int NetworkHost::LAYER_FRAME = Screen::NetworkScreen::Layer::MODAL_FRAME;
	const string NetworkHost::WAIT_CONNECT_TEXT = "クライアント側の接続待ち";

	/**
	 * @fn
	 * 自分のIPアドレスを取得
	*/
	string getIpAdress()
	{
		IPDATA IPAddress[16];
		int IPNum;

		DxLib::GetMyIPAddress(IPAddress, 16, &IPNum);

		return to_string(IPAddress->d1) + "." +
			to_string(IPAddress->d2) + "." +
			to_string(IPAddress->d3) + "." +
			to_string(IPAddress->d4);
	}

	/**
	 * @fn
	 * 開始
	*/
	void NetworkHost::start()
	{
		// 小画面生成
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// フレーム
		shared_ptr<Entity::ModalFrame> frame = make_shared<ModalFrame>();
		frame->setColor(ColorType::MAIN_COLOR);
		frame->setShape(MODAL_X, MODAL_Y, MODAL_W, MODAL_H);
		frame->setTitle("部屋を作る");
		objectsControl.addObject(LAYER_FRAME, frame);

		//自分のIPアドレス
		shared_ptr<Text> ipText = make_shared<Text>("自分のIPアドレス : " + getIpAdress(), CONTENT_X, CONTENT_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_BIT_LITE);
		objectsControl.addFigure(LAYER_CONTENT, ipText);

		// 状況メッセージ
		statusText_ = make_shared<Text>(WAIT_CONNECT_TEXT + ".", CONTENT_X, CONTENT_Y + LINE_H + LINE_MARGIN, FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(LAYER_CONTENT, statusText_);

		// 状況メッセージ更新用タイマー
		textUpdataTimer_.setTimer(TIMER_MS);

		// ルール設定ボタン
		ruleSetButton_ = make_shared<TextButton>();
		ruleSetButton_->setShape(CONTENT_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		ruleSetButton_->setText("ルール設定", FontType::BLACK_S32);
		ruleSetButton_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
		objectsControl.addObject(LAYER_CONTENT, ruleSetButton_);

		// キャンセルボタン
		shared_ptr<TextButton> canceltButton = make_shared<TextButton>();
		canceltButton->setShape(CONTENT_X + BUTTON_W + BUTTON_MARGIN_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		canceltButton->setText("キャンセル", FontType::BLACK_S32);
		canceltButton->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, TextButton::State::NORMAL);
		canceltButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_DOWN);
		canceltButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_OVER);
		objectsControl.addObject(LAYER_CONTENT, ContentId::CANCEL_BTN, canceltButton);

		// 接続待ち状態にする
		DxLib::PreparationListenNetWork(PORT);
	}

	/**
	 * @fn
	 * 終了
	*/
	void NetworkHost::end()
	{
		isConnected_ = false;

		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// 全要素削除
		objectsControl.removeObject(LAYER_FRAME);
		objectsControl.removeObject(LAYER_CONTENT);
		objectsControl.removeFigure(LAYER_CONTENT);
	}

	/**
	 * @fn
	 * 状況取得及び更新処理
	 * @param (hitObjWp) 接触オブジェクトの弱参照
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType) イベントの種類（マウスダウン or マウスアップ or マウスクリック）
	 * @return 状況を返す
	*/
	int NetworkHost::checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{

		// 接続待ち時処理
		if (!isConnected_)
		{
			checkAcceptNetwork();

			// テキスト更新
			if (textUpdataTimer_.checkAndCountTimer())
			{
				++dotCount_;
				if (dotCount_ > 3)
				{
					dotCount_ = 1;
				}

				string dotText = "";
				for (int i = 0; i < dotCount_; ++i)
				{
					dotText += ".";
					statusText_->setText(WAIT_CONNECT_TEXT + dotText);
				}
				textUpdataTimer_.setTimer(TIMER_MS);
			}
		}
		else // 接続済み
		{
			// 送信されてきたデータを描画（テスト）
			int dataLength = DxLib::GetNetWorkDataLength(netHandle_);
			if (dataLength > 0)
			{
				char strBuf[256];
				if (DxLib::NetWorkRecv(netHandle_, &strBuf, dataLength) == 0)
				{
					DxLib::printfDx(strBuf);
					// テスト処理
					DxLib::NetWorkSend(netHandle_, "繋がったぞ～！！", 17);
				}
			}

			// 切断された場合
			if (int lostHandle = GetLostNetWork() == netHandle_)
			{
				statusText_->setText("クライアントが切断\n部屋を作り直してください");
				DxLib::CloseNetWork(netHandle_);
			}
		}

		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{
			// ボタンのクリックイベント
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				if (objId == ContentId::CANCEL_BTN) // キャンセル
				{
					closeNetwork();
					end();
					Utility::ResourceManager::playSound(SoundKind::BACK);
					return Result::CANCEL;
				}
				else if (isConnected_ && hitObjSp == ruleSetButton_) // ルール設定
				{
					end();
					Utility::ResourceManager::playSound(SoundKind::CHECK);
					return Result::SET_RULE;
				}
			}
		}

		return Result::CONTINUE;
	}

	/**
	 * @fn
	 * ネットワーク接続の受付
	*/
	void NetworkHost::checkAcceptNetwork()
	{
		// 接続された時
		netHandle_ = DxLib::GetNewAcceptNetWork();
		if (netHandle_ != -1)
		{
			// 接続の受付を終了する
			DxLib::StopListenNetWork();

			// 接続してきたマシンのＩＰアドレスを得る
			DxLib::GetNetWorkIP(netHandle_, &clientIp_);

			isConnected_ = true;

			statusText_->setText("クライアントと接続完了");

			ruleSetButton_->setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, Entity::TextButton::State::NORMAL);
			ruleSetButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
			ruleSetButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		}
	}

	/**
	 * @fn
	 * ネットワークを切断
	*/
	void NetworkHost::closeNetwork()
	{
		// 接続解除
		if (!isConnected_)
		{
			DxLib::StopListenNetWork();
		}
		else
		{
			DxLib::CloseNetWork(netHandle_);
		}
	}
}