#pragma once
#include <climits>
#include <string>
#include "Utility/ResourceManager.h"
#include "Entity/Shape.h"
#include "Entity/Figure.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;

/**
 * @file Text.h
 * @brief ��ʓ��e�L�X�g�v�f
 */

namespace Entity
{
	class Text : public Figure
	{
	public:
		Text();
		Text(const char* text, int x, int y, int fontType, int colorType, int align = LEFT);
		~Text() {};

		void setText(const char* text);

		void setPadding(int paddingVertical, int paddingSide);

		void setBackgroundColor(int backgroundColorType);

		void setY(int y);

		void setW(int w);

		void render() const;

		/**
		 * @enum Enum
		 * �z�u
		 */
		enum Align
		{
			LEFT,
			CENTER,
			RIGHT
		};

	private:
		void adjustAlign();

		//! ������
		string text_;

		//! �t�H���g�̎��
		int font_;

		//! �F
		int color_;

		//! ���x���W
		int baseX_;

		//! ��̕�
		int baseW_;

		//! ����
		int align_;

		//! �]��
		int paddingVertical_, paddingSide_;

		//! �w�i�F
		int backgroundColor_;
	};
}