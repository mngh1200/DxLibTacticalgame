#pragma once
#include <climits>
#include <string>
#include "Entity/Object.h"
#include "FrameWork/Controller.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file TextBox.h
 * @brief IP�A�h���X���͗�
 */

namespace Entity
{
	class TextBox : public Object
	{
	public:
		TextBox();
		~TextBox();

		// �f�[�^�^�C�v�̎��
		enum DataType
		{
			ALL, //! �����Ȃ�
			HAN, //! ���p�̂�
			NUM, //! ���l�̂�
		};

		void setShape(int x, int y, int w, int h);
		void setPos(int x, int y);
		void setSize(int w, int h);

		void setPaddingLeft(int paddingLeft);

		void setText(string text);

		void setColor(int backColorType, int textColorType, int borderColorType, int borderColorTypeOnFocus);
		void setFont(int fontType);
		void setDataType(DataType dataType);
		void setMaxLength(int maxLength);
		void setBorderWidth(int borderWidth);

		void setNextInput(shared_ptr<TextBox> nextInput);

		void render() const override;

		void onFocus() override;
		void onBlur() override;

		void update();

		void getText(string& text) { text =  text_; };

	private:
		int relationTextX_; //! �e�L�X�g�̑���x���W
		int relationTextY_; //! �e�L�X�g�̑���y���W

		int font_; //! �t�H���g���

		int backColor_; //! �w�i�F
		int textColor_; //! �����F
		int borderColor_; //! �g���F
		int borderColorOnFocus_; //! �t�H�[�J�X���̘g���F

		int borderWidth_; //! �g����
		int maxLength_; //! �ő啶����

		int inputHandle_; //! ���͒����̃n���h��

		string text_;			//! �\���e�L�X�g

		weak_ptr<TextBox> nextInput_; //! ���Ƀt�H�[�J�X����v�f

		DataType dataType_; //! ���̓f�[�^�^�C�v
	};
}