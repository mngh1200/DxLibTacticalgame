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
 * @file ExtraEffect.h
 * @brief ������ʃG�t�F�N�g�i�X�L����A�g���̕\���j
 */

namespace Entity
{
	class ExtraEffect : public Figure
	{
	public:
		ExtraEffect() : text_{}, isPlayer_(true), num_(0), isLeft_(true), animation_{} {};
		~ExtraEffect() {};

		static void makeExtraEffect(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, const char* text, int num);

		void initExtraEffect(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, const char* text, int num);

		void hide();

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int BASE_Y = 100; //! ���Y���W
		constexpr static int W = 180; //! ��
		constexpr static int H = 30; //! ����

		constexpr static int MARGIN = 10; //! �]��
		constexpr static int PADDING = 5; //! �]���i�����j

		constexpr static int ANIMATION_TIME = 200; //! �A�j���[�V��������
		constexpr static int NUM_DELAY = 100; //! �A�Ԃɂ��x������

		//! �e�L�X�g
		string text_;

		//! �v���C���[����
		bool isPlayer_;

		//! �\���ʒu�������ł��邩
		bool isLeft_;

		//! ���Ԗڂɕ\�����邩
		int num_;

		//! �A�j���[�V�����N���X
		Animation animation_;

		// �A�j���[�V�����̎��
		enum AnimationKind
		{
			SHOW,
			HIDE
		};
	};
}