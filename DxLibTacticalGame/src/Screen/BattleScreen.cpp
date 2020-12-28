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

		// マップ（マス）
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>();
		objectsControl.addObject(Layer::MAP, 0, map);

		// 行動選択メニュー
		shared_ptr<Entity::SelectActiveMenu> selectActiveMenu = make_shared<Entity::SelectActiveMenu>();
		objectsControl.addObject(Layer::UI, SELECT_ACTIVE_MENU, selectActiveMenu);

		// バトル管理クラス
		btlMng = Battle::BattleManager(map, selectActiveMenu);

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
		enemy->init(4, 0, true);
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
			Entity::ObjectsControl& objCont = FrameWork::Game::getInstance().objectsControl;

			if (nowScene_ == Scene::PLAYER_TURN) // プレイヤーターン
			{
				if (eventType == MOUSE_INPUT_LOG_CLICK) // クリック
				{
					if (hitObjSp->getLayerId() == Layer::MAP) // マップクリック
					{
						btlMng.onClickMass(x, y);
					}
					else if (hitObjSp->getType() == Entity::Figure::UNIT) // ユニットクリック
					{
						btlMng.onClickUnit(x, y);
					}
					else if (hitObjSp->getLayerId() == Layer::UI) // UI操作
					{
						if (hitObjSp->getObjectId() == UIid::SELECT_ACTIVE_MENU) // 行動選択
						{
							shared_ptr<Entity::SelectActiveMenu> menu = dynamic_pointer_cast<Entity::SelectActiveMenu>(hitObjSp);
							btlMng.onClickActionMenu(menu->getHitButtonKey(x, y));
						}
					}
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
		btlMng.animationCheck();

		if (isOpenOverlayEnded())
		{
			nowScene_ = Scene::PLAYER_TURN;
		}
		isCloseOverlayEnded();
	}
}