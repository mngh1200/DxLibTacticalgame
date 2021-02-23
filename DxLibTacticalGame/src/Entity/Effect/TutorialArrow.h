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
 * @file TutorialArrow.h
 * @brief �_���[�W�G�t�F�N�g�i�_���[�W�ʂ܂���MISS�̕\���j
 */

namespace Entity
{
	class TutorialArrow : public Figure
	{
	public:
		TutorialArrow() : animation_{}, baseY_(0) {};
		~TutorialArrow() {};

		constexpr static int W = 32; //! ��
		constexpr static int H = 50; //! ����

		static shared_ptr<TutorialArrow> makeTutorialArrow(int x, int y);

		void start(int x, int y);

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int ANIME_MS = 400; //! �A�j���[�V����1�񕪂̎���
		constexpr static int DISPLAY_TIME = 8000; //! �\������
		constexpr static int ANIMATION_COUNT = DISPLAY_TIME / ANIME_MS; //! �A�j���[�V�������J��Ԃ���

		constexpr static int MOVE_Y = 15; //! �A�j���[�V�����œ��삷���

		int baseY_; //! ���Y���W

		Animation animation_; //! �A�j���[�V����

		// �A�j���[�V�����̎��
		enum AnimationKind
		{
			UP_DOWN
		};
	};
}