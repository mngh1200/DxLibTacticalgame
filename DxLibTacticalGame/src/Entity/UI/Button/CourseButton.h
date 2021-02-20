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
		CourseButton(int x, int y, int rank, bool isNew = false);
		~CourseButton() {};

		void render() const override;

		bool animationUpdate() override;

		void onMouseClick(int x, int y) override;
		void onMouseOver(int x, int y) override;
		void onMouseOut(int x, int y) override;

		void setSelected(bool isSelected);

		static const int SIZE = 100; //! ���A����

		// �A�j���[�V�����̎��
		enum AnimationId
		{
			EXPANSION,
			SHRINK,
			BORN
		};

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int RECT_ROUND = 10; //! �p�ۂ̒l
		constexpr static int HOVER_ANIMATION_MS = 100;	//! �g��A�k���̎���
		static const float HOVER_ANIMATION_SCALE;	//! �T�C�Y�{��(float��cpp�Œ�`����K�v����)

		constexpr static int BORN_ANIMATION_MS = 500; //! NEW�R�[�X�A�j���[�V�����̎���
		constexpr static int BORN_ANIMATION_DELAY = 100; //! NEW�R�[�X�A�j���[�V�����̒x������
		constexpr static int BORN_DISP_TEXT_SIZE = 48; //! �ǂ̑傫���̎��_�ŕ�����\�����邩

		//! �I����Ԃɂ��邩
		bool isSelected_;

		//! �A�j���[�V�����N���X
		Animation animation_;

		//! ��̑傫��
		Shape baseShape_;

		//! �R�[�X�̃N���A��
		string text_;
	};

	
}