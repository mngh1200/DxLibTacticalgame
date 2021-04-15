#pragma once
#include <climits>
#include <string>
#include "Entity/UI/Button/TextButton.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"

using namespace std;

/**
 * @file CourseButton.h
 * @brief �Z���N�g��ʂ̃R�[�X�Z���N�g�{�^��
 */

namespace Entity
{
	class CourseButton : public TextButton
	{
	public:
		CourseButton();
		CourseButton(int x, int y, int w, int h, const char* text, int state);
		~CourseButton() {};

		void render() const override;

		void setSelected(bool isSelected, bool isSound = false);

		// �{�^���̏��
		enum State
		{
			NORMAL, //! �W��
			NEW,	//! �V�X�e�[�W
			CLEAR	//! �N���A�X�e�[�W
		};

	private:
		constexpr static int RECT_ROUND = 10; //! �p�ۂ̒l
		constexpr static int HOVER_ANIMATION_MS = 100;	//! �g��A�k���̎���
		static const float HOVER_ANIMATION_SCALE;	//! �T�C�Y�{��(float��cpp�Œ�`����K�v����)

		constexpr static int BORN_ANIMATION_MS = 500; //! NEW�R�[�X�A�j���[�V�����̎���
		constexpr static int BORN_ANIMATION_DELAY = 100; //! NEW�R�[�X�A�j���[�V�����̒x������
		constexpr static int BORN_DISP_TEXT_SIZE = 48; //! �ǂ̑傫���̎��_�ŕ�����\�����邩

		//! �I����Ԃɂ��邩
		bool isSelected_;

		//! �{�^���̏��
		int state_;
	};

	
}