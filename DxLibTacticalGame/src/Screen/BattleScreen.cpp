#include "BattleScreen.h"

namespace Screen
{
	/**
	 * @fn
	 * 初期処理
	*/
	void BattleScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;
		objectsControl.setLayer(Layer::LEN);

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// 背景
		shared_ptr<Entity::Back> back = make_shared<Entity::Back>();
		back->init(Entity::Back::ScreenKind::BATTLE_SCREEN);
		objectsControl.addObject(Layer::BACK, 0, back);

		// システムメニュ-
		systemMenu_ = make_shared<Entity::ContextMenu>();
		objectsControl.addObject(Layer::CONTEXT_MENU, 0, systemMenu_);

		systemMenu_->addMenuButton(SystemMenuKey::TURN_END, "ターン終了");
		systemMenu_->addMenuButton(SystemMenuKey::CHECK_WIN_TEXT, "勝敗条件");

		if (isNetMatch_) // 通信対戦
		{
			systemMenu_->addMenuButton(SystemMenuKey::CLOSE_NETWORK, "切断");
		}
		else // 標準
		{
			systemMenu_->addMenuButton(SystemMenuKey::HINT, "ヒント");
			systemMenu_->addMenuButton(SystemMenuKey::BACK_SELECT_SCREEN, "セレクト画面に戻る");
			systemMenu_->addMenuButton(SystemMenuKey::BACK_MENU_SCREEN, "メニュー画面に戻る");
		}

		systemMenu_->setDisabledMenuButton(SystemMenuKey::TURN_END, true);



		// マップ（マス）
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>();
		objectsControl.addObject(Layer::MAP, 0, map);
		
		// バトル管理用クラスの初期処理
		if (isNetMatch_) // 通信対戦
		{
			btlMng_.init(map, stageId_, setUnitNum_, isServer_, sender_);
			playerBtlCont_.init(map);
			enemyBtlCont_.init(map, 0);
		}
		else // 標準
		{
			int aiKind;
			btlMng_.init(map, stageId_, &setUnitNum_, &aiKind);
			playerBtlCont_.init(map);
			enemyBtlCont_.init(map, aiKind);
		}

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
	void BattleScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{
			// システムメニュー関連イベント
			if (nowScene_ == Scene::PLAYER_TURN || nowScene_ == Scene::ENEMY_TURN || nowScene_ == Scene::SET_UNITS || nowScene_ == Scene::WAIT_ENEMY_SET)
			{
				int systemMenuKey = systemMenu_->checkRunButton(x, y, eventType);
				execSystemMenuProcess(systemMenuKey);

				if (systemMenuKey == -1 && (eventType == MOUSE_INPUT_LOG_UP || (eventType == MOUSE_INPUT_LOG_CLICK && hitObjSp != systemMenu_)))
				{
					systemMenu_->hide(); // コンテキストメニューを閉じる

					if (button == MOUSE_INPUT_RIGHT) // 右マウスダウン
					{
						systemMenu_->show(x, y); // コンテキストメニューを開く
					}
				}
			}

			if (nowScene_ == Scene::PLAYER_TURN) // プレイヤーターン
			{
				playerBtlCont_.updateByEvents(&btlMng_, hitObjSp, x, y, button, &eventType);				
				
				if (eventType == MOUSE_INPUT_LOG_CLICK)
				{
					// ターン終了ボタン
					if (hitObjSp->getLayerId() == Layer::UI && hitObjSp->getObjectId() == Battle::BattleUI::BattleUIid::TURN_END_BUTTON)
					{
						turnEnd();
					}
				}
			}
			else if (nowScene_ == Scene::SET_UNITS && eventType == MOUSE_INPUT_LOG_CLICK) // ユニット配置シーン
			{
				// 準備完了ボタン
				if (hitObjSp->getLayerId() == Layer::UI && hitObjSp->getObjectId() == Battle::BattleUI::BattleUIid::CONFIRM_UNIT_SET)
				{
					confirmSetUnits();
				}
				else if (hitObjSp->getLayerId() == Layer::UI && hitObjSp->getObjectId() == Battle::BattleUI::BattleUIid::SELECT_UNIT_AREA)
				{
					// ユニット選択エリア
					btlMng_.tutorial.onEvent(Battle::TutorialManager::TutorialId::FREE_SET_SET, &btlMng_);
				}
				else
				{
					// ユニット配置イベント
					SetUnits::onClick(x, y, btlMng_.map, &btlMng_.battleUI, &btlMng_.tutorial, &btlMng_);
				}
			}
			else if (nowScene_ == Scene::RESULT && eventType == MOUSE_INPUT_LOG_CLICK) // 勝敗結果画面
			{
				// 画面クリックで画面遷移
				if (isNetMatch_) // 通信対戦時
				{
					nextScreen_ = new NetworkScreen();
				}
				else // 標準時
				{
					nextScreen_ = new SelectScreen();
				}
				
				createOverlay(false);
			}
			else if (nowScene_ == Scene::NETWORK_CLOSE) // 相手のネットワーク切断時
			{
				if (eventType == MOUSE_INPUT_LOG_CLICK && dialog_.isEqualsOkBtn(hitObjSp)) // ダイアログのOKボタンクリック時
				{
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
	void BattleScreen::updateByAnimation()
	{
		if (isNetMatch_) // 通信対戦関連処理
		{
			updateNetwork();
		}

		btlMng_.animationCheck(); // バトル管理系の処理

		if (!isNetMatch_ && nowScene_ == Scene::ENEMY_TURN) // 敵ターン
		{
			if (enemyBtlCont_.update(&btlMng_))
			{
				turnEnd();
			}
		}

		// 勝敗判定
		int winner = btlMng_.checkEnd();
		if (winner !=  Battle::CheckWin::Winner::UNDECIDED &&
			(nowScene_ == Scene::ENEMY_TURN || nowScene_ == Scene::PLAYER_TURN))
		{
			nowScene_ = Scene::RESULT_ANIME;

			bool isPlayerWin = winner == Battle::CheckWin::Winner::PLAYER;

			ResultScene::makeResultScene(isPlayerWin);
			
			if (isPlayerWin) // 新コース解禁判定
			{
				Utility::SaveManager& saveManager = Utility::SaveManager::getInstance();

				saveManager.updateRank(stageId_, StageRank::CLEAR);

				int newStageId = stageId_ + 1;
				if (saveManager.getRank(newStageId) == StageRank::LOCK)
				{
					saveManager.updateRank(newStageId, StageRank::NEW);
				}

				saveManager.save();
			}

		} 
		else if (nowScene_ == Scene::RESULT_ANIME) // 結果画面のアニメーション終了判定
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			Entity::ObjectsControl& objectsControl = game.objectsControl;

			shared_ptr<Entity::Object> retScene = objectsControl.getObjectWp(Layer::TOP_UI, TopUiId::RESULT_SCENE).lock();

			if (retScene && !retScene->isAnimation())
			{
				nowScene_ = Scene::RESULT;
			}
		}
		else if (isOpenOverlayEnded()) // オーバーレイ開く
		{
			if (setUnitNum_ > 0) // ユニット配置
			{
				nowScene_ = Scene::SET_UNITS;
			}
			else
			{
				startBattle();
			}
		}	
		else if (isCloseOverlayEnded()) // オーバレイ閉じる
		{
			if (nextScreen_ != nullptr)
			{
				// セレクト画面に戻る
				FrameWork::Game::getInstance().setScreen(nextScreen_);
			}
		}
	}

	/**
	 * @fn
	 * 選択ステージをセット
	*/
	void BattleScreen::setStage(int id)
	{
		stageId_ = id;
	}

	/**
	 * @fn
	 * 通信対戦できる状態を準備
	 * @param (isServer) サーバー側であるか
	 * @param (mapId) マップID
	 * @param (unitNum) ユニット数
	*/
	void BattleScreen::prepareNetMatch(int netHandle, bool isServer, int mapId, int unitNum, bool isFirst)
	{
		sender_ = make_shared<Network::SendManager>(); // 送信管理クラスの初期化

		isNetMatch_ = true;
		stageId_ = mapId;
		setUnitNum_ = unitNum;
		isServer_ = isServer;
		isFirst_ = isFirst;

		sender_->setNetHandle(netHandle);
		receiver_.setNetHandle(netHandle);
	}

	/**
	 * @fn
	 * 通信対戦関連の更新処理
	*/
	void BattleScreen::updateNetwork()
	{
		if (nowScene_ == Scene::NETWORK_CLOSE)
		{
			return;
		}

		// 切断された場合
		if (DxLib::GetLostNetWork() == receiver_.getNetHandle())
		{
			DxLib::CloseNetWork(receiver_.getNetHandle());
			nowScene_ = Scene::NETWORK_CLOSE;

			// ダイアログ表示
			FrameWork::Game& game = FrameWork::Game::getInstance();
			Entity::ObjectsControl& objectsControl = game.objectsControl;

			dialog_.show("相手が通信を切断しました\nメインメニューに戻ります", Layer::DIALOG_FRAME, Layer::TOP_UI);
		}

		receiver_.receive(); // データ受信

		if (nowScene_ == Scene::WAIT_ENEMY_SET) // 敵プレイヤーのユニット配置待ち
		{
			if (receiver_.checkReceiveSignal(SignalKind::SET_END)) // 敵プレイヤー配置完了
			{
				SetUnits::receiveSetUnitsData(&receiver_, btlMng_.map); // 敵ユニットの配置
				btlMng_.battleUI.endWaitEnemySet();
				startBattle();
			}
		}
		else if (nowScene_ == Scene::ENEMY_TURN) // 敵ターン
		{
			if (receiver_.execEnemyAction(&btlMng_, btlMng_.map, Layer::ENEMY_UNIT))
			{
				turnEnd(); // 敵ターン終了
			}
		}
	}

	/**
	 * @fn
	 * バトル開始
	*/
	void BattleScreen::startBattle()
	{
		if (isFirst_)
		{
			systemMenu_->setDisabledMenuButton(SystemMenuKey::TURN_END, false);
			nowScene_ = Scene::PLAYER_TURN;
			btlMng_.onStartTurn(true);
		}
		else
		{
			nowScene_ = Scene::ENEMY_TURN;
			btlMng_.onStartTurn(false);
		}

	}

	/**
	 * @fn
	 * システムメニュー関連の処理を実行
	 * @param (systemMenuKey) システムメニューキー
	*/
	void BattleScreen::execSystemMenuProcess(int systemMenuKey)
	{
		if (systemMenuKey == SystemMenuKey::TURN_END) // ターンエンド
		{
			// ターンエンド処理
			turnEnd();
			systemMenu_->hide();
		}
		else if (systemMenuKey == SystemMenuKey::HINT) // ヒント
		{
			showHint();
			systemMenu_->hide();
		}
		else if (systemMenuKey == SystemMenuKey::CHECK_WIN_TEXT) // 勝敗条件
		{
			showCheckWinText();
			systemMenu_->hide();
		}
		else if (systemMenuKey == SystemMenuKey::BACK_SELECT_SCREEN) // セレクト画面
		{
			nextScreen_ = new SelectScreen();
			createOverlay(false);
		}
		else if (systemMenuKey == SystemMenuKey::BACK_MENU_SCREEN) // メニュー画面
		{
			nextScreen_ = new MenuScreen();
			createOverlay(false);
		}
		else if (systemMenuKey == SystemMenuKey::CLOSE_NETWORK) // 切断
		{
			DxLib::CloseNetWork(receiver_.getNetHandle());
			nextScreen_ = new NetworkScreen();
			createOverlay(false);
		}
	}

	/**
	 * @fn
	 * ターン終了時（別ターンに切替時）
	*/
	void BattleScreen::turnEnd()
	{
		Utility::ResourceManager::playSound(SoundKind::CLICK);

		if (nowScene_ == Scene::PLAYER_TURN) // プレイヤーターン終了
		{
			if (isNetMatch_)
			{
				sender_->sendSignal(Network::SignalKind::TURN_END); // ターン終了信号を送信
			}

			nowScene_ = Scene::ENEMY_TURN;
			btlMng_.onStartTurn(false);

			systemMenu_->setDisabledMenuButton(SystemMenuKey::TURN_END, true);
		}
		else if (nowScene_ == Scene::ENEMY_TURN) // 敵ターン終了
		{
			nowScene_ = Scene::PLAYER_TURN;
			btlMng_.onStartTurn(true);

			systemMenu_->setDisabledMenuButton(SystemMenuKey::TURN_END, false);
		}
	}

	/**
	 * @fn
	 * ユニット設置の準備完了ボタン押下時の処理
	*/
	void BattleScreen::confirmSetUnits()
	{
		btlMng_.battleUI.endSelectUnitMode();
		btlMng_.map->clearMassUnitSet();

		if (isNetMatch_)
		{
			nowScene_ = Scene::WAIT_ENEMY_SET;
			btlMng_.battleUI.startWaitEnemySet();
			SetUnits::sendSetUnitsData(sender_, btlMng_.map); // 配置情報送信
		}
		else
		{
			startBattle();
		}
	}

	/**
	 * @fn
	 * ヒント表示
	*/
	void BattleScreen::showHint()
	{
		string title;
		string hint;
		vector<int> checkWinData;
		Utility::ResourceManager::loadStageData("stage", stageId_, &title, &hint, &checkWinData);

		btlMng_.message->show(hint, false);
	}

	/**
	 * @fn
	 * 勝敗条件表示
	*/
	void BattleScreen::showCheckWinText()
	{
		string text = "勝利条件\n";

		string value;
		int lineCount;

		// 勝利条件内容
		btlMng_.getCheckWin().getWinConditionsText(&value, &lineCount);
		text += value;

		// 敗北条件ラベル
		text += "\n\n敗北条件\n";

		// 敗北条件内容
		btlMng_.getCheckWin().getLoseConditionsText(&value, &lineCount);
		text += value;

		btlMng_.message->show(text, false);
	}
}