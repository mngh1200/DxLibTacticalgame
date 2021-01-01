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

		// 背景
		shared_ptr<Entity::Back> back = make_shared<Entity::Back>(Entity::Back::Screen::BATTLE);
		objectsControl.addObject(Layer::BACK, 0, back);

		// マップ（マス）
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>();
		objectsControl.addObject(Layer::MAP, 0, map);

		// バトル管理クラス
		btlMng = Battle::BattleManager(map, Layer::UI);

		// ユニット設置(テスト)
		shared_ptr<Entity::Unit> playerUnit = make_shared<Entity::Unit>();
		playerUnit->init(4, 9, false);
		if (btlMng.setUnit(playerUnit))
		{
			objectsControl.addObject(Layer::PLAYER_UNIT, 0, playerUnit);
		}

		shared_ptr<Entity::Unit> playerUnit2 = make_shared<Entity::Unit>();
		playerUnit2->init(5, 9, false);
		if (btlMng.setUnit(playerUnit2))
		{
			objectsControl.addObject(Layer::PLAYER_UNIT, 1, playerUnit2);
		}

		shared_ptr<Entity::Unit> enemy = make_shared<Entity::Unit>();
		enemy->init(4, 7, true);
		if (btlMng.setUnit(enemy))
		{
			objectsControl.addObject(Layer::ENEMY_UNIT, 0, enemy);
		}
		
		

		// オーバーレイセット
		createOverlay(true);
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	*/
	void BattleScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{

			if (nowScene_ == Scene::PLAYER_TURN) // プレイヤーターン
			{
				btlMng.updateByEvents(hitObjSp, x, y, button, eventType);
			}
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void BattleScreen::updateByAnimation()
	{
		btlMng.animationCheck();

		if (isOpenOverlayEnded())
		{
			nowScene_ = Scene::PLAYER_TURN;
		}
		isCloseOverlayEnded();
	}
}