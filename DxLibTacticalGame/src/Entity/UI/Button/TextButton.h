#pragma once
#include <climits>
#include <string>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file TextButton.h
 * @brief �e�L�X�g�t���{�^���v�f
 */

namespace Entity
{
	class TextButton : public Object
	{
	public:
		TextButton();
		TextButton(int colorType, int textColorType);
		virtual ~TextButton() {};

		void setShape(int x, int y, int w, int h);
		void setPos(int x, int y);
		void setSize(int w, int h);

		void setText(string text);
		void setText(string text, int fontType);
		void setTextX(int x);
		void setFont(int fontType);

		void setColor(int backgroundColorType, int textColorType, int state = ALL);
		void setBackgroundColor(int backgroundColorType, int state = ALL);
		void setTextColor(int textColorType, int state = ALL);

		virtual void render() const override;

		// �{�^���̏��
		enum State
		{
			NORMAL,
			MOUSE_DOWN,
			MOUSE_OVER,
			ALL
		};

	protected:
		//! �e�L�X�g�̍��W
		int textX_;
		int textY_;

		//! �e�L�X�g
		string text_;

		//! �t�H���g
		int font_;

		//! �{�^���F(�{�^���̊e��Ԃ̏ꍇ���Ǘ�)
		int color_[State::ALL];

		//! �����F(�{�^���̊e��Ԃ̏ꍇ���Ǘ�)
		int textColor_[State::ALL];

	private:
		void setTextCenter();
		void setTextMiddle();

		//! ���������ɂ��邩
		bool isCenter_;
	};


}