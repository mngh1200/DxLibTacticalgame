#pragma once
#include <climits>
#include "FrameWork/Game.h"
#include "ModalFrame.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/View/Text.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file Dialog.h
 * @brief �_�C�A���O�N���X
 */

namespace Entity
{
	class Dialog
	{
	public:
		Dialog() : isShow_(false) {};
		~Dialog() {};

		void show(const char* message, int frameLayerId, int contentLayerId);
		void end();

		bool isEqualsOkBtn(shared_ptr<Object> objSp) const;

	private:
		constexpr static int PADDING = 30; //! �]��
		constexpr static int PADDING_BOTTOM = 20; //! ���]��
		
		constexpr static int FONT_TYPE = FontType::NORMAL_S24; //! �t�H���g�̎��

		constexpr static int MESSAGE_MARGIN_BOTTOM = 30; //! ���b�Z�[�W�̉��]��
		constexpr static int BUTTON_H = 50; //! �{�^���̍���

		constexpr static int TEXT_COLOR_TYPE = ColorType::SUB_COLOR; //! �e�L�X�g�F
		constexpr static int TEXT_HOVER_COLOR_TYPE = ColorType::MAIN_COLOR; //! �z�o�[���̃e�L�X�g�F

		constexpr static int BUTTON_COLOR_TYPE = ColorType::SUB_COLOR_LITE; //! �{�^���F
		constexpr static int BUTTON_HOVER_COLOR_TYPE = ColorType::SUB_COLOR; //! �z�o�[���̃{�^���F

		bool isShow_;	//! �\�����ł��邩

		shared_ptr<ModalFrame> frame_; //! ���[�_���̘g�I�u�W�F�N�g

		shared_ptr<TextButton> okBtn_; //! OK�{�^���̋�`���W

		shared_ptr<Text> messageBox_;	//! ���b�Z�[�W
	};
}