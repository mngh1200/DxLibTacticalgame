#include "NetworkClient.h"
#include "Screen/NetworkScreen.h"

namespace Network
{
	const int NetworkClient::LAYER_CONTENT = Screen::NetworkScreen::Layer::MODAL_CONTENT;
	const int NetworkClient::LAYER_FRAME = Screen::NetworkScreen::Layer::MODAL_FRAME;

	/**
	 * @fn
	 * 開始
	*/
	void NetworkClient::start()
	{
		// 小画面生成
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// フレーム
		shared_ptr<ModalFrame> frame = make_shared<Entity::ModalFrame>();
		frame->setColor(ColorType::MAIN_COLOR);
		frame->setShape(MODAL_X, MODAL_Y, MODAL_W, MODAL_H);
		frame->setTitle("部屋を探す");
		objectsControl.addObject(LAYER_FRAME, frame);

		// テキスト
		objectsControl.addFigure(LAYER_CONTENT, make_shared<Text>("接続先のIPアドレスを入力してください",
			CONTENT_X, CONTENT_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_BIT_LITE));

		// IPアドレス入力エリア
		shared_ptr<TextBox> nextInput; // 次のIPアドレス入力欄
		for (int i = 3; i >= 0; --i)
		{
			shared_ptr<Entity::TextBox> ipInput = make_shared<Entity::TextBox>();
			int x = CONTENT_X + i * (IP_INPUT_W + IP_INPUT_MARGIN);
			ipInput->setShape(x, IP_INPUT_Y, IP_INPUT_W, IP_INPUT_H);
			ipInput->setMaxLength(3);
			ipInput->setDataType(TextBox::DataType::NUM);
			ipInput->setFont(FontType::NORMAL_S24);
			ipInput->setPaddingLeft(16);

			// デフォルト
			if (i == 0)
			{
				ipInput->setText("192");
			}
			else if (i == 1)
			{
				ipInput->setText("168");
			}

			objectsControl.addObject(LAYER_CONTENT, ipInput);

			if (nextInput)
			{
				ipInput->setNextInput(nextInput);
			}
			nextInput = ipInput;
			ipAdressInputList_.push_back(ipInput);

			// IPアドレスの間のドット
			if (i == 3)
			{
				continue;
			}

			objectsControl.addFigure(LAYER_CONTENT, make_shared<Text>(".", x + IP_INPUT_W + IP_INPUT_MARGIN / 3, IP_INPUT_Y + 20, FontType::NORMAL_S24, ColorType::SUB_COLOR));
		}

		// 状況メッセージ
		statusText_ = make_shared<Text>("", CONTENT_X, BUTTON_Y - STATUS_LINE_H * 2, FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(LAYER_CONTENT, statusText_);

		// 接続ボタン
		connectButton_ = make_shared<TextButton>();
		connectButton_->setShape(CONTENT_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		connectButton_->setText("接続", FontType::BLACK_S32);
		objectsControl.addObject(LAYER_CONTENT, connectButton_);

		adjustConnectButtonValid();

		// キャンセルボタン
		shared_ptr<TextButton> canceltButton = make_shared<TextButton>();
		canceltButton->setShape(CONTENT_X + BUTTON_MARGIN_X + BUTTON_W, BUTTON_Y, BUTTON_W, BUTTON_H);
		canceltButton->setText("キャンセル", FontType::BLACK_S32);
		canceltButton->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, TextButton::State::NORMAL);
		canceltButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_DOWN);
		canceltButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_OVER);
		objectsControl.addObject(LAYER_CONTENT, ContentId::CANCEL_BTN, canceltButton);

	}

	/**
	 * @fn
	 * 終了
	*/
	void NetworkClient::end()
	{
		state_ = State::NONE;

		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// 全要素削除
		objectsControl.removeObject(LAYER_FRAME);
		objectsControl.removeObject(LAYER_CONTENT);
		objectsControl.removeFigure(LAYER_CONTENT);

		// 接続解除
		DxLib::CloseNetWork(netHandle_);
	}

	/**
	 * @fn
	 * 更新処理
	 * @param (hitObjWp) 接触オブジェクトの弱参照
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType) イベントの種類（マウスダウン or マウスアップ or マウスクリック）
	 * @return 状況を返す
	*/
	int NetworkClient::checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		// IPアドレスリストの更新処理
		bool isChange = false; //! 変更があったか
		for (auto itr = ipAdressInputList_.begin(); itr != ipAdressInputList_.end(); ++itr)
		{
			if ((*itr)->checkChangeAndUpdate())
			{
				isChange = true;
			}
		}

		if (isChange) // 変更があった場合、接続ボタンの状態を調整
		{
			adjustConnectButtonValid();
		}

		// 接続済みのとき
		if (state_ == State::CONNECTED)
		{
			// 取得していない受信データ量を得る(テスト)
			int dataLength = DxLib::GetNetWorkDataLength(netHandle_);
			if (dataLength > 0)
			{
				// データ受信
				char strBuf[256];
				DxLib::NetWorkRecv(netHandle_, &strBuf, dataLength);    // データをバッファに取得
				DxLib::printfDx(strBuf);
			}

			// 切断された場合
			if (int lostHandle = DxLib::GetLostNetWork() == netHandle_)
			{
				statusText_->setText("ホストから切断されました\n再接続してください");
				setState(State::NONE);
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
					Utility::ResourceManager::playSound(SoundKind::BACK);
					end();
					return Result::CANCEL;
				}
				
				// 初期状態
				if (state_ == State::NONE)
				{
					if (hitObjSp == connectButton_ && ableConnecButton_) // 接続
					{
						Utility::ResourceManager::playSound(SoundKind::CHECK);
						connectNetwork();
						return Result::CONTINUE;
					}
				}

			}
		}

		// キーイベント
		FrameWork::Controller& cont = FrameWork::Controller::getInstance();
		if (cont.isKeyPressedNow(KEY_INPUT_RETURN) && ableConnecButton_) // 接続可能な時にエンターキーを押下した場合
		{
			connectNetwork();
		}

		return Result::CONTINUE;
	}


	/**
	 * @fn
	 * 状況を変更
	 * @param (state) 状況
	*/
	void NetworkClient::setState(State state)
	{
		state_ = state;
		adjustConnectButtonValid();
	}

	/**
	 * @fn
	 * 接続ボタンの有効無効状態を調整
	*/
	void NetworkClient::adjustConnectButtonValid()
	{
		ableConnecButton_ = false; // 有効 / 無効

		if (state_ == State::NONE) // 初期状態
		{
			bool isBlank = false; // 空欄の項目があるか
			for (auto itr = ipAdressInputList_.begin(); itr != ipAdressInputList_.end(); ++itr)
			{
				string text;
				(*itr)->getText(text);
				if (text == "")
				{
					isBlank = true;
					break;
				}
			}

			ableConnecButton_ = !isBlank;
		}
		
		if (!ableConnecButton_) // 無効
		{
			connectButton_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
		}
		else // 有効
		{
			connectButton_->setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, Entity::TextButton::State::NORMAL);
			connectButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
			connectButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		}
	}

	/**
	 * @fn
	 * ネットワークに接続
	*/
	void NetworkClient::connectNetwork()
	{
		// 通信を確立
		IPDATA ipData;
		getIpData(ipData);
		netHandle_ = DxLib::ConnectNetWork(ipData, PORT);

		// 確立が成功した場合のみ中の処理をする
		if (netHandle_ != -1)
		{
			statusText_->setText("ホストに接続できました\nホストがルール設定中です");

			// データ送信
			DxLib::NetWorkSend(netHandle_, "繋がったか～！？", 17);

			setState(State::CONNECTED);
		}
		else // 失敗
		{
			statusText_->setText("ホストへの接続に失敗しました");
		}
	}

	/**
	 * @fn
	 * IPアドレスを取得
	 * @param (ipData) IPアドレスの参照渡し用
	*/
	void NetworkClient::getIpData(IPDATA& ipData)
	{
		string text;
		ipAdressInputList_.at(3)->getText(text);
		ipData.d1 = atoi(text.c_str());

		ipAdressInputList_.at(2)->getText(text);
		ipData.d2 = atoi(text.c_str());

		ipAdressInputList_.at(1)->getText(text);
		ipData.d3 = atoi(text.c_str());

		ipAdressInputList_.at(0)->getText(text);
		ipData.d4 = atoi(text.c_str());
	}
}