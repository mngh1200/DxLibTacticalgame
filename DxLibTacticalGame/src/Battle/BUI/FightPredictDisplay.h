#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Entity/Unit/Unit.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Battle/Fight.h"
#include "BattleUIDefine.h"

using namespace std;

/**
 * @file FightPredictDisplay.h
 * @brief �퓬�\���\����
 */

namespace Entity
{
	class FightPredictDisplay : public Object
	{
	public:
		FightPredictDisplay();
		~FightPredictDisplay() {};

		void render() const override;
		bool animationUpdate() override;

		void emplaceFight(const Battle::Fight* fight);
		void reset();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int AREA_WIDTH = 450; //! �G���A�̕�
		constexpr static int CENTER_WIDTH = 50; //! �������̕�
		constexpr static int CENTER_PADDING = 10; //! �������̗]��
		constexpr static int DIRECTION_W = CENTER_WIDTH - CENTER_PADDING * 2; // �U�������̕�

		constexpr static int PLAYER_X = AREA_WIDTH + CENTER_WIDTH;

		constexpr static int ANIMATION_MS = 500;	//! �A�j���[�V��������
		constexpr static int ANIMATION_Y0 = WIN_H;  //! �A�j���[�V�����J�n�ʒu

		void drawFightPredict(int x, Battle::FightData& fightData, bool isEnemy) const;
		void drawAtackDirection(int y, bool isPlayer) const;

		// �A�j���[�V�������
		enum AnimationKind
		{
			DISPLAY
		};

		//! �퓬�\���f�[�^
		const Battle::Fight* fight_;

		//! �A�j���[�V�����p�ϐ�
		Animation animation_;
	};
}