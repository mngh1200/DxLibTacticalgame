#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Utility/ResourceManager.h"
#include "Entity/Object.h"
#include "TerrainEffectDisplay.h"
#include "Entity/Unit/Unit.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "BattleUIDefine.h"

using namespace std;

/**
 * @file UnitStatusDisplay.h
 * @brief ���j�b�g�X�e�[�^�X�\����
 */

namespace Entity
{
	class UnitStatusDisplay : public Object
	{
	public:
		UnitStatusDisplay();
		~UnitStatusDisplay() {};

		void render() const override;
		bool animationUpdate() override;

		void setTargetUnit(shared_ptr<Unit> unit);
		void clear();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int WIDTH = 450; //! ��
		constexpr static int X = TerrainEffectDisplay::WIDTH + 40; //! X���W

		constexpr static int ANIMATION_MS = 500;	//! �A�j���[�V��������
		constexpr static int ANIMATION_Y0 = WIN_H;  //! �A�j���[�V�����J�n�ʒu

		// �A�j���[�V�������
		enum AnimationKind
		{
			DISPLAY
		};

		//! �Ώۃ}�X
		shared_ptr<Unit> targetUnit_;

		//! �A�j���[�V�����p�ϐ�
		Animation animation_;
	};
}