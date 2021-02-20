#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Entity/Unit/Unit.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;

/**
 * @file DamageEffect.h
 * @brief �_���[�W�G�t�F�N�g�i�_���[�W�ʂ܂���MISS�̕\���j
 */

namespace Entity
{
	class DamageEffect : public Figure
	{
	public:
		DamageEffect() : isDisplay_(false), imageIds_{} {};
		~DamageEffect() {};

		constexpr static int MISS = -1; //! �~�X�̏ꍇ�̒l

		static void makeDamageEffect(int unitX, int unitY, int damage);

		void init(int unitX, int unitY, int damage);

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:

		constexpr static int H = 32; //! �~�X�\���̕�
		constexpr static int MISS_W = 115; //! �~�X�\���̕�
		constexpr static int DAMAGE_W = 26; //! �_���[�W���l�̕��i�P�����j

		vector<int> imageIds_; //! �\���摜

		bool isDisplay_; //! �`���

		Animation animation_; //! �A�j���[�V����

		// �A�j���[�V�����̎��
		enum AnimationKind
		{
			DISPLAY
		};
	};
}