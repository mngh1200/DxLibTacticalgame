#pragma once
#include <climits>
#include <string>
#include "Entity/UI/Button/TextButton.h"
#include "Utility/FontManager.h"
#include "Animation/Animation.h"

using namespace std;

/**
 * @file Button.h
 * @brief �{�^���v�f
 */

namespace Entity
{
	class MenuScreenButton : public TextButton
	{
	public:
		MenuScreenButton();
		MenuScreenButton(string text, int x, int y, int w, int h);
		~MenuScreenButton() {};

		bool animationUpdate() override;

		void onMouseLeftDown() override;
		void onMouseLeftUp() override;
		void onMouseOut() override;

	protected:
		bool createAnimation(int animationId);

	private:
		static const int TEXT_PADDING_LEFT = 25;	//! �e�L�X�g�̍��]��
		static const int ANIMATION_MS = 100;		//! �g��A�k���̎���
		static const float ANIMATION_SCALE;			//! �T�C�Y�{��(float��cpp�Œ�`����K�v����)

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