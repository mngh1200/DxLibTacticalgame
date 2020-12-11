#pragma once
#include <climits>
#include <string>
#include "Entity/UI/Button/TextButton.h"
#include "Utility/FontManager.h"
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
		CourseButton(int x, int y, char status);
		~CourseButton() {};

		void render() const override;

		bool animationUpdate() override;

		void onMouseClick() override;
		void onMouseOver() override;
		void onMouseOut() override;

		void setSelected(bool isSelected);

		// �N���A��
		enum Status : char
		{
			S = 'S',
			A = 'A',
			B = 'B',
			C = 'C',
			NO_CLEAR = '!'
		};

		static const int SIZE = 100; //! ���A����

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int RECT_ROUND = 10; //! �p�ۂ̒l
		constexpr static int HOVER_ANIMATION_MS = 100;	//! �g��A�k���̎���
		static const float HOVER_ANIMATION_SCALE;	//! �T�C�Y�{��(float��cpp�Œ�`����K�v����)

		//! �I����Ԃɂ��邩
		bool isSelected_;

		//! �A�j���[�V�����N���X
		Animation animation_;

		//! ��̑傫��
		Shape baseShape_;

		// �A�j���[�V�����̎��
		enum AnimationId 
		{
			EXPANSION,
			SHRINK,
			BORN
		};
	};

	
}