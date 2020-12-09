#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "Utility/FontManager.h"
#include "Animation/Animation.h"

using namespace std;

/**
 * @file Button.h
 * @brief �{�^���v�f
 */

namespace Entity
{
	class TextButton : public Object
	{
	public:
		TextButton() = delete;
		TextButton(int colorType, int textColorType);
		~TextButton() {};

		void setShape(int x, int y, int w, int h);
		void setPos(int x, int y);
		void setSize(int w, int h);

		void setText(string text);
		void setText(string text, int x, int fontType);
		void setTextCenter(string text, int fontType);
		void setFont(int fontType);

		void setColor(int backgroundColor, int textColor, int state = NORMAL);
		void render() const override;

		void setSizeAnimation(float scale, int timeMs);
		bool animationUpdate() override;

		void onMouseLeftDown() override;
		void onMouseLeftUp() override;
		void onMouseOver() override;
		void onMouseOut() override;

		// �{�^���̏��
		enum State
		{
			NORMAL,
			MOUSE_DOWN,
			MOUSE_OVER,
			LEN
		};

	protected:
		bool createAnimation(int animationId);

	private:
		/*
		static const int ACCENT_WIDTH = 25;			//! �A�N�Z���g�����̕�
		static const int TEXT_PADDING_LEFT = 25;	//! �e�L�X�g�̍��]��
		static const int TEXT_SIZE = 32;			//! �t�H���g�T�C�Y
		static const int ANIMATION_MS = 100;		//! �g��A�k���̎���
		static const float ANIMATION_SCALE;			//! �T�C�Y�{��(float��cpp�Œ�`����K�v����)
		*/

		bool ableSizeAnimation() const;

		//! �e�L�X�g�̍��W
		int textX_;
		int textY_;

		//! �e�L�X�g
		string text_;

		//! �t�H���g
		int font_;

		//! ���݂̃{�^���̏��
		int nowState_;

		//! �{�^���F(�{�^���̊e��Ԃ̏ꍇ���Ǘ�)
		int color_[State::LEN];

		//! �����F(�{�^���̊e��Ԃ̏ꍇ���Ǘ�)
		int textColor_[State::LEN];

		//! ��̑傫��
		Shape baseShape_;

		//! �T�C�Y�A�j���[�V�����̃T�C�Y�ω���(1�̏ꍇ�̓A�j���[�V�����Ȃ�)
		float sizeAnimationScale_;

		//! �T�C�Y�A�j���[�V��������

		//! �A�j���[�V�����N���X
		Animation sizeAnimation_;

		//! �A�j���[�V����ID
		enum SizeAnimationId
		{
			EXPANSION,
			SHRINK
		};
	};


}