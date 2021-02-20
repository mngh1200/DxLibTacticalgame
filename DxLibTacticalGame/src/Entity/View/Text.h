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
		Text(string text, int x, int y, int fontType, int colorType, int align = LEFT);
		~Text() {};

		void setText(string text);

		void setY(int y);

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
		//! ������
		string text_;

		//! �t�H���g�̎��
		int font_;

		//! �F
		int color_;

		//! ���x���W
		int baseX_;

		//! ����
		int align_;
	};
}