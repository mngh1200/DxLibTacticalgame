#pragma once
#include <climits>
#include "DxLib.h"

namespace
{
	//! キー入力の種類数
	const int	KEY_LEN = 256;
}

namespace FrameWork
{
	class Controller
	{
	public:
		/**
		 * @enum
		 * マウスイベント
		 */
		enum {
			MOUSE_LEFT_DOWN,
			MOUSE_RIGHT_DOWN,
			MOUSE_WHEEL_DOWN,
			MOUSE_LEFT_UP,
			MOUSE_RIGHT_UP,
			MOUSE_WHEEL_UP
		};

		// for singleton
		Controller(const Controller&) = delete;
		Controller& operator=(const Controller&) = delete;
		Controller(Controller&&) = delete;
		Controller& operator=(Controller&&) = delete;

		static Controller& getInstance();

		int getAllEvents();

	private:
		Controller() {};
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

		bool isKeyPressed(int input) const;
		bool isKeyPressedNow(int input) const;
		char getKey(int input) const;


		//! 各キーの入力状態
		char key_[KEY_LEN];

		//! 各キーの入力状態（１フレーム前）
		char prevKey_[KEY_LEN];
	};
}