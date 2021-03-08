#pragma once
#include <climits>
#include <string>
#include "Entity/Object.h"
#include "FrameWork/Controller.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file TextBox.h
 * @brief IPアドレス入力欄
 */

namespace Entity
{
	class TextBox : public Object
	{
	public:
		TextBox();
		~TextBox();

		// データタイプの種類
		enum DataType
		{
			ALL, //! 制限なし
			HAN, //! 半角のみ
			NUM, //! 数値のみ
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
		int relationTextX_; //! テキストの相対x座標
		int relationTextY_; //! テキストの相対y座標

		int font_; //! フォント種類

		int backColor_; //! 背景色
		int textColor_; //! 文字色
		int borderColor_; //! 枠線色
		int borderColorOnFocus_; //! フォーカス時の枠線色

		int borderWidth_; //! 枠線幅
		int maxLength_; //! 最大文字列長

		int inputHandle_; //! 入力中情報のハンドル

		string text_;			//! 表示テキスト

		weak_ptr<TextBox> nextInput_; //! 次にフォーカスする要素

		DataType dataType_; //! 入力データタイプ
	};
}