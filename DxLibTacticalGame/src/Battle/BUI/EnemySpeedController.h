#pragma once
#include <climits>
#include <cmath>
#include "Entity/UI/Button/TurnEndButton.h"
#include "Utility/ResourceManager.h"
#include "Battle/BUI/BattleUIDefine.h"

using namespace std;

/**
 * @file EnemySpeedController.h
 * @brief �G�^�[���i�s���x�����{�^��
 */

namespace Entity
{
	class EnemySpeedController : public Object
	{

	public:
		EnemySpeedController();
		~EnemySpeedController() {};

		void render() const override;
		void onMouseClick(int x, int y) override;

		void show();
		void hide();

	private:
		constexpr static int PADDING_X = 22; //! �{�^���̗]�� ��
		constexpr static int PADDING_Y = 16; //! �{�^���̗]�� �c
		constexpr static int BUTTON_H = BATTLE_UI_AREA_H - (TurnEndButton::MARGIN + PADDING_Y) * 2; //! �{�^���̃T�C�Y
		constexpr static int BUTTON_W = BUTTON_H + 10;
		constexpr static int BUTTON_RELATIVE_XN = (TurnEndButton::WIDTH - PADDING_X * 2 - BUTTON_W) / 2; //! N�Ԗڂ̃{�^���̑���X���W

		const static float PLAYMARK_W; //! �Đ��}�[�N�̕�

		//! �i�s���x
		enum Speed
		{
			SLOW,
			NORMAL,
			FAST,
			LEN
		};

		//! �i�s���x
		int speed_;

		void changeSpeed(int speed);

		Shape buttonsShape_[Speed::LEN];
	};

	
}