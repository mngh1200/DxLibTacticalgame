#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Entity/Battle/Mass.h"
#include "Animation/Animation.h"
#include "DxLib.h"

using namespace std;

/**
 * @file TerrainEffectDisplay.h
 * @brief �n�`���ʕ\����
 */

namespace Entity
{
	class TerrainEffectDisplay : public Figure
	{
	public:
		TerrainEffectDisplay();
		~TerrainEffectDisplay() {};

		constexpr static int WIDTH = 210; //! ��

		void render() const override;
		bool animationUpdate() override;

		void setTargetMass(shared_ptr<Mass> mass, int realX);
		void clear();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int LINE_MARGIN = 3;  //! ��s�̗]��
		constexpr static int LINE_PADDING = 6; //! ��s�̗]��
		constexpr static int LINE_HEIGHT = 18 + (LINE_MARGIN + LINE_PADDING) * 2; //! ��s������̍���
		constexpr static int LINE_COUNT = 2;   //! ���s���邩

		constexpr static int ANIMATION_Y0 = WIN_H;  //! �A�j���[�V�����J�n�ʒu

		// �A�j���[�V�������
		enum AnimationKind
		{
			DISPLAY
		};

		//! �Ώۃ}�X
		shared_ptr<Mass> targetMass_;

		//! �A�j���[�V�����p�ϐ�
		Animation animation_;
	};
}