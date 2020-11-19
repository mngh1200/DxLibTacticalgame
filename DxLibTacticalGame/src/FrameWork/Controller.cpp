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


		// マウスイベント
		Game& game = Game::getInstance();
		return game.objectsControl.checkMouseEvent();
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