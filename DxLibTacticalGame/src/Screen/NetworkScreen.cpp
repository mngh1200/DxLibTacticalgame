#include "NetworkScreen.h"

namespace Screen
{
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
	 * 初期処理
	*/
	void NetworkScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		objectsControl.setLayer(Layer::LEN);

		// 背景
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());

		// 部屋を作るボタン
		shared_ptr<Entity::TextButton> createRoomButton = make_shared<Entity::TextButton>();
		createRoomButton->setText("部屋を作る", FontType::BLACK_S48);
		createRoomButton->setShape(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		createRoomButton->setColor(ColorType::MAIN_COLOR, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		createRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		createRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);

		objectsControl.addObject(Layer::UI, UIid::CREATE_ROOM_BUTTON, createRoomButton);

		// 部屋を探すボタン
		shared_ptr<Entity::TextButton> searchRoomButton = make_shared<Entity::TextButton>();
		searchRoomButton->setText("部屋を探す", FontType::BLACK_S48);
		searchRoomButton->setShape(BUTTON_X, BUTTON_Y + BUTTON_H + BUTTON_MARGIN, BUTTON_W, BUTTON_H);
		searchRoomButton->setColor(ColorType::MAIN_COLOR, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		searchRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		searchRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);

		objectsControl.addObject(Layer::UI, UIid::SEARCH_ROOM_BUTTON, searchRoomButton);

		// IPアドレス入力エリア
		shared_ptr<TextBox> nextInput; // 次のIPアドレス入力欄
		for (int i = 3; i >= 0; --i)
		{
			shared_ptr<Entity::TextBox> ipInput = make_shared<Entity::TextBox>();
			ipInput->setShape(BUTTON_X + i * (100 + 10), BUTTON_Y + (BUTTON_H + BUTTON_MARGIN) * 2, 100, BUTTON_H);
			ipInput->setMaxLength(3);
			ipInput->setDataType(TextBox::DataType::NUM);
			objectsControl.addObject(Layer::UI, UIid::IP_INPUT_1 + i, ipInput);

			if (nextInput)
			{
				ipInput->setNextInput(nextInput);
			}
			nextInput = ipInput;
			ipAdressInputList_.push_back(ipInput);
		}


		// 戻るボタン
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(ColorType::MAIN_COLOR, ColorType::SUB_COLOR);
		backBtn->setShape(WIN_W - BACK_SIZE, 0, BACK_SIZE, BACK_SIZE);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setText("×", FontType::NORMAL_S32);
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, backBtn);

		// IPアドレス取得
		DxLib::printfDx(string("自分のIPアドレス" + getIpAdress()).c_str());

		// オーバーレイセット
		createOverlay(true);
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	 * @param (hitObjWp) 接触オブジェクトの弱参照
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType) イベントの種類（マウスダウン or マウスアップ or マウスクリック）
	*/
	void NetworkScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		// IPアドレスリストの更新処理
		for (auto itr = ipAdressInputList_.begin(); itr != ipAdressInputList_.end(); ++itr)
		{
			(*itr)->update();
		}

		if (hitObjSp)
		{
			// ボタンのクリックイベント
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				if (objId == UIid::CREATE_ROOM_BUTTON) // 「部屋を作る」ボタン
				{
					createRoom();
				}
				else if (objId == UIid::SEARCH_ROOM_BUTTON) // 「部屋を探す」ボタン
				{
					searchRoom();
				}
				else if (objId == UIid::QUIT_BUTTON) // 終了ボタン
				{
					// 画面遷移
					nextScreen_ = new MenuScreen();
					createOverlay(false);
				}
			}
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void NetworkScreen::updateByAnimation()
	{
		isOpenOverlayEnded();

		if (isCloseOverlayEnded())
		{
			FrameWork::Game::getInstance().setScreen(nextScreen_); // 画面遷移
		}
		
	}

	/**
	 * @fn
	 * 部屋を作る
	*/
	void NetworkScreen::createRoom()
	{
		char StrBuf[256];        // データバッファ
		int NetHandle, LostHandle;    // ネットワークハンドル
		int DataLength;            // 受信データ量保存用変数
		IPDATA Ip;            // 接続先ＩＰアドレスデータ

		// 接続してくるのを待つ状態にする
		DxLib::PreparationListenNetWork(9850);

		// 接続してくるかＥＳＣキーが押されるまでループ
		NetHandle = -1;
		while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
		{
			// 新しい接続があったらそのネットワークハンドルを得る
			NetHandle = GetNewAcceptNetWork();
			if (NetHandle != -1) break;
		}

		// 接続されていたら次に進む
		if (NetHandle != -1)
		{
			// 接続の受付を終了する
			StopListenNetWork();

			// 接続してきたマシンのＩＰアドレスを得る
			GetNetWorkIP(NetHandle, &Ip);

			// データが送られて来るまで待つ
			while (!ProcessMessage())
			{
				// 取得していない受信データ量が０以外のときはループから抜ける
				if (GetNetWorkDataLength(NetHandle) != 0) break;
			}

			// データ受信
			DataLength = GetNetWorkDataLength(NetHandle);    // データの量を取得
			NetWorkRecv(NetHandle, StrBuf, DataLength);    // データをバッファに取得

			// 受信したデータを描画
			DxLib::printfDx(StrBuf);

			// 受信成功のデータを送信
			NetWorkSend(NetHandle, "繋がったぞ～！！", 17);

			// 相手が通信を切断するまで待つ
			while (!ProcessMessage())
			{
				// 新たに切断されたネットワークハンドルを得る
				LostHandle = GetLostNetWork();

				// 切断された接続が今まで通信してた相手だった場合ループを抜ける
				if (LostHandle == NetHandle) break;
			}
		}

		DxLib::printfDx("切断しました");
	}

	/**
	 * @fn
	 * 部屋探す
	*/
	void NetworkScreen::searchRoom()
	{
		char StrBuf[256];    // データバッファ
		IPDATA Ip;        // 接続用ＩＰアドレスデータ
		int NetHandle;        // ネットワークハンドル
		int DataLength;        // 受信データ量保存用変数

		// ＩＰアドレスを設定( ここにある４つのＩＰ値は仮です )
		Ip.d1 = 192;
		Ip.d2 = 168;
		Ip.d3 = 207;
		Ip.d4 = 203;

		// 通信を確立
		NetHandle = ConnectNetWork(Ip, 9850);

		// 確立が成功した場合のみ中の処理をする
		if (NetHandle != -1)
		{
			// データ送信
			NetWorkSend(NetHandle, "繋がったか～！？", 17);

			// データがくるのを待つ
			while (!ProcessMessage())
			{
				// 取得していない受信データ量を得る
				DataLength = GetNetWorkDataLength(NetHandle);

				// 取得してない受信データ量が０じゃない場合はループを抜ける
				if (DataLength != 0) break;
			}

			// データ受信
			NetWorkRecv(NetHandle, StrBuf, DataLength);    // データをバッファに取得

			// 受信したデータを描画
			DxLib::printfDx(StrBuf);

			// キー入力待ち
			WaitKey();

			// 接続を断つ
			CloseNetWork(NetHandle);
		}
	}
}