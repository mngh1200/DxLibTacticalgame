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
	class MenuScreenButton : public Object
	{
	public:
		MenuScreenButton();
		MenuScreenButton(string text, int x, int y, int w, int h);
		~MenuScreenButton() {};

		void render() const override;

		bool animationUpdate() override;

		void onMouseLeftDown() override;
		void onMouseLeftUp() override;
		void onMouseOver() override;
		void onMouseOut() override;

	protected:
		bool createAnimation(int animationId);

	private:
		
		static const int ACCENT_WIDTH = 25;			//! �A�N�Z���g�����̕�
		static const int TEXT_PADDING_LEFT = 25;	//! �e�L�X�g�̍��]��
		static const int TEXT_SIZE = 32;			//! �t�H���g�T�C�Y
		static const int ANIMATION_MS = 100;		//! �g��A�k���̎���
		static const float ANIMATION_SCALE;			//! �T�C�Y�{��(float��cpp�Œ�`����K�v����)

		//! �e�L�X�g��Y���W
		int textY_;

		//! �e�L�X�g
		string text_;

		//! �{�^���F
		int backgroundColor_;

		//! �����F
		int textColor_;

		//! �A�j���[�V�����N���X
		Animation animation_;

		//! ��̑傫��
		Shape baseShape_;

		enum AnimationId 
		{
			EXPANSION,
			SHRINK
		};
	};

	
}