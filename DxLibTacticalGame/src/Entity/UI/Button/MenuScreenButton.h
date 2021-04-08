#pragma once
#include <climits>
#include <string>
#include "Entity/UI/Button/TextButton.h"
#include "Utility/ResourceManager.h"
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
		~MenuScreenButton() {};

		void setY(int y);

		bool animationUpdate() override;

		void onMouseLeftDown(int x, int y) override;
		void onMouseLeftUp(int x, int y) override;
		void onMouseOut(int x, int y) override;

		constexpr static int H = 60; //! �{�^����

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int W = 400; //! �{�^����
		constexpr static int X = (WIN_W - W) / 2; //! �{�^��X���W

		constexpr static int ANIMATION_MS = 100;		//! �g��A�k���̎���
		const static float ANIMATION_SCALE;			//! �T�C�Y�{��(float��cpp�Œ�`����K�v����)

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