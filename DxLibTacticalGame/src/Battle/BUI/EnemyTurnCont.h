#pragma once
#include <climits>
#include <cmath>
#include "Entity/Unit/Unit.h"
#include "Entity/UI/Button/BuiConfirmButton.h"
#include "Utility/ResourceManager.h"
#include "Battle/BUI/BattleUIDefine.h"
#include "Animation/Animation.h"

using namespace std;

namespace Battle
{
	class EnemyBattleController;
}

/**
 * @file EnemyTurnCont.h
 * @brief �G�^�[�����Ƀo�g��UI�̉E���ɐݒu����I�u�W�F�N�g�i�i�s���x������󋵃e�L�X�g�j
 */

namespace Entity
{
	class EnemyTurnCont : public Object
	{

	public:
		EnemyTurnCont();
		~EnemyTurnCont() {};

		void render() const override;
		void onMouseClick(int x, int y) override;

		void show();
		void hide();

		void setMode(int mode);
		
		bool animationUpdate() override;

		// ���[�h�̎��
		enum Mode
		{
			SPEED_CONT, //! �X�s�[�h����
			ENEMY_TURN_MESSAGE, //! �G�^�[�����ł���\�L
			ENEMY_SET_MESSAGE	//! �G�̎��R�z�u���ł���\�L
		};

		//! �G�^�[�����̃A�j���[�V�������Ԕ{��
		static float animationRate;

		static int getAnimationMs(int ms);

	protected:
		bool createAnimation(int animationId);
	private:
		constexpr static int PADDING_X = 22; //! �{�^���̗]�� ��
		constexpr static int PADDING_Y = 16; //! �{�^���̗]�� �c
		constexpr static int BUTTON_H = BATTLE_UI_AREA_H - (BuiConfirmButton::MARGIN + PADDING_Y) * 2; //! �{�^���̃T�C�Y
		constexpr static int BUTTON_W = BUTTON_H + 10;
		constexpr static int BUTTON_RELATIVE_XN = (BuiConfirmButton::WIDTH - PADDING_X * 2 - BUTTON_W) / 2; //! N�Ԗڂ̃{�^���̑���X���W

		const static float PLAYMARK_W; //! �Đ��}�[�N�̕�

		constexpr static int FONT_TYPE = FontType::NORMAL_S18; // �t�H���g�^�C�v
		constexpr static int TEXT_COLOR_TYPE = ColorType::MAIN_COLOR; // �e�L�X�g�J���[�^�C�v

		constexpr static int TEXT_MARGIN_LEFT = 10; //! �e�L�X�g�̗]��

		const static string ENEMY_TURN_TEXT;	//! �G�^�[�����ł���\�L�e�L�X�g
		const static string ENEMY_SET_TEXT;		//! �G�̎��R�z�u���ł���\�L�e�L�X�g

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

		//! �G�^�[���I�����ɐi�s���x��ێ����Ă���
		int cacheSpeed_;

		void changeSpeed(int speed);

		//! �e�{�^���̍��W�ƃT�C�Y
		Shape buttonsShape_[Speed::LEN];

		//! ���[�h
		int mode_;

		//! �e�L�X�g�̑���Y���W
		int textRelationY_;

		//! �e�L�X�g�̕s�����x
		int textAlpha_;

		//! �A�j���[�V����
		Animation animation_;
	};

	
}