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
	 * 全てのキーの状態を取得
	 */
	int Controller::getAllKeyPressed()
	{
		return DxLib::GetHitKeyStateAll(key_);
	}

	/**
	 * @fn
	 * キーの押下状態を調整（押下した瞬間のキーの値を変更）
	 */
	void Controller::adjustKeyState()
	{
		for (int i = 0; i < KEY_LEN; i++)
		{
			// compare previous state with current state
			if (prevKey_[i] == NOT_PRESSED && key_[i] == PRESSED)
			{
				key_[i] = PRESSED_NOW;  // if change state, set pressed state
			}
			prevKey_[i] = key_[i];
		}
	}

	/**
	 * @fn
	 * 対象キーの状態取得
	 */
	char Controller::getKey(int input) const
	{
		return key_[input];
	}
}