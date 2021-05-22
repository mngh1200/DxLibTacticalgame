#pragma once
#include <climits>
#include <vector>
#include <list>
#include "DxLib.h"

/**
 * @file Controller.h
 * @brief キーイベントを管理
 */

namespace
{
	//! キー入力の種類数
	constexpr int	KEY_LEN = 256;
}

namespace FrameWork
{
	class Controller
	{
	public:
		// for singleton
		Controller(const Controller&) = delete;
		Controller& operator=(const Controller&) = delete;
		Controller(Controller&&) = delete;
		Controller& operator=(Controller&&) = delete;

		static Controller& getInstance();

		int getAllEvents();

		bool isKeyPressed(int input) const;
		bool isKeyPressedNow(int input) const;

	private:
		Controller() : key_{ {NOT_PRESSED} }, prevKey_{ {NOT_PRESSED} } {};
		~Controller() {};

		/**
		 * @enum KeyState
		 * キーの入力状態
		 */
		enum KeyState {
			NOT_PRESSED = 0,
			PRESSED = 1,
			PRESSED_NOW = 2
		};

		char getKey(int input) const;


		//! 各キーの入力状態
		char key_[KEY_LEN];

		//! 各キーの入力状態（１フレーム前）
		char prevKey_[KEY_LEN];
	};
}