#pragma once
#include <climits>
#include <vector>
#include <list>
#include "DxLib.h"

/**
 * @file Controller.h
 * @brief �L�[�C�x���g���Ǘ�
 */

namespace
{
	//! �L�[���͂̎�ސ�
	constexpr int	KEY_LEN = 256;
}

namespace FrameWork
{
	class Controller
	{
	public:
		/**
		 * @enum
		 * �}�E�X�C�x���g
		 */
		enum {
			MOUSE_NONE_EVENT,
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

		bool isKeyPressed(int input) const;
		bool isKeyPressedNow(int input) const;

	private:
		Controller() : key_{ {NOT_PRESSED} }, prevKey_{ {NOT_PRESSED} } {};
		~Controller() {};

		/**
		 * @enum KeyState
		 * �L�[�̓��͏��
		 */
		enum KeyState {
			NOT_PRESSED = 0,
			PRESSED = 1,
			PRESSED_NOW = 2
		};

		char getKey(int input) const;


		//! �e�L�[�̓��͏��
		char key_[KEY_LEN];

		//! �e�L�[�̓��͏�ԁi�P�t���[���O�j
		char prevKey_[KEY_LEN];
	};
}