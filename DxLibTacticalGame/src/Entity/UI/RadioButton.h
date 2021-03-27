#pragma once
#include <climits>
#include <string>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file RadioButton.h
 * @brief ラジオボタン要素
 */

namespace Entity
{
	struct RadioButtonItem
	{
		string name = ""; //! アイテム名
		int relationTextX = 0; //! テキスト表示の相対座標
		Shape shape = Shape(0, 0, 0, 0); //! 矩形座標
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
		int getSelectedNum() const { return selectedNum_; }; // 何番目の項目を選択しているか返す（0~）

	private:
		void setTextCenter();
		void adjustShape();

		constexpr static int ITEM_MARGIN = 3; //! 項目の余白

		int relationTextY_; //! テキストの相対y座標

		int font_; //! フォント種類

		int backColor_; //! 背景色
		int textColor_; //! 文字色
		int selectedBackColor_; //! 選択時の背景色

		vector<RadioButtonItem> items_; //! 選択項目名

		int itemW_; //! 項目幅
		int itemH_; //! 項目高さ

		int selectedNum_; //! 何番目の項目を選択しているか
	};
}