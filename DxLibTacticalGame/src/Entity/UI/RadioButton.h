#pragma once
#include <climits>
#include <string>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file RadioButton.h
 * @brief ���W�I�{�^���v�f
 */

namespace Entity
{
	struct RadioButtonItem
	{
		string name = ""; //! �A�C�e����
		int relationTextX = 0; //! �e�L�X�g�\���̑��΍��W
		Shape shape = Shape(0, 0, 0, 0); //! ��`���W
	};

	class RadioButton : public Object
	{
	public:
		RadioButton();
		~RadioButton() {};

		void setPos(int x, int y);
		void setItemSize(int w, int h);

		void setItems(vector<string>& textList);

		void setColor(int backColorType, int selectedBackColor, int textColorType);
		void setFont(int fontType);

		void select(int num);

		void render() const override;

		void onMouseClick(int x, int y) override;

		void getSelectedText(string& text) const;
		int getSelectedNum() const { return selectedNum_; }; // ���Ԗڂ̍��ڂ�I�����Ă��邩�Ԃ��i0~�j

	private:
		void setTextCenter();
		void adjustShape();

		constexpr static int ITEM_MARGIN = 3; //! ���ڂ̗]��

		int relationTextY_; //! �e�L�X�g�̑���y���W

		int font_; //! �t�H���g���

		int backColor_; //! �w�i�F
		int textColor_; //! �����F
		int selectedBackColor_; //! �I�����̔w�i�F

		vector<RadioButtonItem> items_; //! �I�����ږ�

		int itemW_; //! ���ڕ�
		int itemH_; //! ���ڍ���

		int selectedNum_; //! ���Ԗڂ̍��ڂ�I�����Ă��邩
	};
}