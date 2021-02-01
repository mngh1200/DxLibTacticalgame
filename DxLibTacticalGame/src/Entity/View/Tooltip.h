#pragma once
#include <climits>
#include <string>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;

/**
 * @file Tooltip.h
 * @brief �c�[���`�b�v�v�f
 */

namespace Entity
{
	class Tooltip : public Figure
	{
	public:
		Tooltip() : text_(""), alpha_(0) {};
		~Tooltip() {};

		void show(int x, int y, string text);
		void hide();

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int PADDING = 10;
		const static int FONT_TYPE = FontType::NORMAL_S18;

		constexpr static int ANIME_MS = 300; //! �A�j���[�V��������

		int alpha_; //! �s�����x

		string text_; //! �e�L�X�g

		Animation animation_; //! �A�j���[�V����

		// �A�j���[�V�����̎��
		enum AnimationKind
		{
			SHOW,
			HIDE
		};
	};
}