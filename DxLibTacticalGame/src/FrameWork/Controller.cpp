#include "Controller.h"


namespace FrameWork
{
	/**
	 * @fn
	 * シングルトンにするためのインスタンス関数
	 */
	Controller& Controller::getInstance()
	{
		static Controller instance;
		return instance;
	}

	/**
	 * @fn
	 * 全てのキーの状態を取得
	 * @return デフォルト: 0 , ゲーム終了判定: -1
	 */
	int Controller::getAllEvents()
	{
		// 全てのキーの状態取得
		if (DxLib::GetHitKeyStateAll(key_) != 0) {
			return -1;
		}

		// キーの状態調整
		for (int i = 0; i < KEY_LEN; i++)
		{
			if (prevKey_[i] == NOT_PRESSED && key_[i] == PRESSED)
			{
				key_[i] = PRESSED_NOW;  // if change state, set pressed state
			}
			prevKey_[i] = key_[i];
		}

		// マウスイベント取得
		int x, y, button, eventType;
		
		if (DxLib::GetMouseInputLog2(&button, &x, &y, &eventType, true) == -1)
		{
			// マウスイベントログがなかった場合、マウスの位置だけ取得
			eventType = -1;
			button = -1;
			if (DxLib::GetMousePoint(&x, &y) == -1)
			{
				return -1;
			}
		}

		Game& game = Game::getInstance();

		//! 接触オブジェクトを見つけたか
		bool isFoundHitObj = false;
		
		// マウスイベント
		for (auto&& layer : game.layerObjList)
		{
			for (auto&& objMap : layer)
			{
				shared_ptr<Entity::Object> obj = objMap.second;
				if (obj->checkMouseEvent(x, y, button, &eventType, isFoundHitObj)) {
					// マウス接触
					isFoundHitObj = true;

					// テスト（ボタン押下時終了）
					if (obj->objectType == Entity::Object::ObjectType::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
					{
						return -1;
					}
				}
			}
		}


		return 0;
	}

	/**
	 * @fn
	 * 対象キーが押下中か判定
	 * @return true:押下中
	 */
	bool Controller::isKeyPressed(int input) const
	{
		return getKey(input) != KeyState::NOT_PRESSED;
	}

	/**
	 * @fn
	 * 対象キーが押下した瞬間か判定
	 * @return true:押下した瞬間
	 */
	bool Controller::isKeyPressedNow(int input) const
	{
		return getKey(input) == KeyState::PRESSED_NOW;
	}

	/**
	 * @fn
	 * 対象キーの状態取得
	 * @return 対象キーの状態
	 */
	char Controller::getKey(int input) const
	{
		return key_[input];
	}
}