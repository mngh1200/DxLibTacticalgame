#include "TextBox.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	TextBox::TextBox() :
		relationTextX_(10),
		relationTextY_(0),
		inputHandle_(-1),
		text_(""),
		borderWidth_(2),
		maxLength_(INT_MAX),
		dataType_(DataType::ALL)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		font_ = rm.getHdlFont(FontType::NORMAL_S32);

		backColor_ = rm.getColor(ColorType::MAIN_COLOR);
		textColor_ = rm.getColor(ColorType::SUB_COLOR);
		borderColor_ = rm.getColor(ColorType::SUB_COLOR_LITE);
		borderColorOnFocus_ = rm.getColor(ColorType::SUB_COLOR);

		type_ = Type::TEXT_BOX;
	}

	/**
	 * @fn
	 * デストラクタ
	 */
	TextBox::~TextBox()
	{
		DxLib::DeleteKeyInput(inputHandle_);
	}

	/**
	 * @fn
	 * ボタン位置とサイズ指定
	 * @param (x) X座標
	 * @param (y) Y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void TextBox::setShape(int x, int y, int w, int h)
	{
		setPos(x, y);
		setSize(w, h);
	}

	/**
	 * @fn
	 * ボタン位置指定
	 * @param (x) X座標
	 * @param (y) Y座標
	 */
	void TextBox::setPos(int x, int y)
	{
		// テキストとボタンの相対座標を計算
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * ボタンサイズ指定
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void TextBox::setSize(int w, int h)
	{
		shape_.w = w;
		shape_.h = h;
		
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		relationTextY_ = (shape_.h - DxLib::GetFontSizeToHandle(font_)) / 2;
	}

	/**
	 * @fn
	 * 左側の余白指定
	 * @param (paddingLeft) 左側の余白
	 */
	void TextBox::setPaddingLeft(int paddingLeft)
	{
		relationTextX_ = paddingLeft;
	}

	/**
	 * @fn
	 * テキストセット
	 * @param (text) テキスト
	 */
	void TextBox::setText(const char* text)
	{
		text_ = text;
	}

	/**
	 * @fn
	 * 色指定
	 * @param (backColorType)  背景色
	 * @param (textColorType)  テキスト色
	 * @param (borderColorType) 枠線色
	 * @param (borderColorTypeOnFocus) フォーカス時の枠線色 
	 */
	void TextBox::setColor(int backColorType, int textColorType, int borderColorType, int borderColorTypeOnFocus)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		backColor_ = rm.getColor(backColorType);
		textColor_ = rm.getColor(textColorType);
		borderColor_ = rm.getColor(borderColorType);
		borderColorOnFocus_ = rm.getColor(borderColorTypeOnFocus);
	}

	/**
	 * @fn
	 * フォント指定
	 * @param (fontType) フォントタイプ
	 */
	void TextBox::setFont(int fontType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		font_ = rm.getHdlFont(fontType);

		relationTextY_ = (shape_.h - DxLib::GetFontSizeToHandle(font_)) / 2;
	}

	/**
	 * @fn
	 * 入力データタイプ指定
	 * @param (dataType) データタイプ
	 */
	void TextBox::setDataType(DataType dataType)
	{
		dataType_ = dataType;
	}

	/**
	 * @fn
	 * 最大文字列長指定
	 * @param (maxLength) 最大文字列長
	 */
	void TextBox::setMaxLength(int maxLength)
	{
		maxLength_ = maxLength;
	}

	/**
	 * @fn
	 * 枠線の幅指定
	 * @param (borderWidth) 枠線の幅
	 */
	void TextBox::setBorderWidth(int borderWidth)
	{
		borderWidth_ = borderWidth;
	}

	/**
	 * @fn
	 * 次にフォーカスするIPアドレス入力欄
	 */
	void TextBox::setNextInput(shared_ptr<TextBox> nextInput)
	{
		nextInput_ = nextInput;
	}

	/**
	 * @fn
	 * 描画
	 */
	void TextBox::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// 枠線と背景色判定
		int borderColor = borderColor_;

		if (isFocus_)
		{
			borderColor = borderColorOnFocus_;
		}


		// 枠線
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), borderColor, TRUE);

		// 背景
		DxLib::DrawBox(shape_.x + borderWidth_, shape_.y + borderWidth_,
			shape_.getX2() - borderWidth_, shape_.getY2() - borderWidth_, backColor_, TRUE);


		// テキスト
		if (DxLib::CheckKeyInput(inputHandle_) != -1)
		{
			DxLib::DrawKeyInputString(shape_.x + relationTextX_, shape_.y + relationTextY_, inputHandle_);
		}
		else
		{
			DxLib::DrawFormatStringToHandle(shape_.x + relationTextX_, shape_.y + relationTextY_, textColor_, font_, text_.c_str());
		}
	}

	/**
	 * @fn
	 * フォーカスイベント
	 */
	void TextBox::onFocus()
	{
		int state = DxLib::CheckKeyInput(inputHandle_); // キー入力ハンドルの状態取得
		
		if (state == -1) // キー入力ハンドルが存在しない場合、作成
		{
			// 入力データタイプを判定
			if (dataType_ == DataType::ALL)
			{
				inputHandle_ = DxLib::MakeKeyInput(maxLength_, FALSE, FALSE, FALSE);
			}
			else if (dataType_ == DataType::HAN)
			{
				inputHandle_ = DxLib::MakeKeyInput(maxLength_, FALSE, TRUE, FALSE);
			}
			else if (dataType_ == DataType::NUM)
			{
				inputHandle_ = DxLib::MakeKeyInput(maxLength_, FALSE, FALSE, TRUE);
			}

			// 入力中文字の色やフォントを指定
			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
			DxLib::SetKeyInputStringColor(textColor_, textColor_,textColor_, textColor_, textColor_,
				textColor_, textColor_, textColor_);
			DxLib::SetKeyInputStringFont(font_);

			DxLib::SetKeyInputString(text_.c_str(), inputHandle_);
		}

		DxLib::SetActiveKeyInput(inputHandle_);
	}

	/**
	 * @fn
	 * フォーカスアウトイベント
	 */
	void TextBox::onBlur()
	{
		DxLib::DeleteKeyInput(inputHandle_);
	}

	/**
	 * @fn
	 * 変更があったかの確認と更新処理（キーイベントと入力チェック）
	 */
	bool TextBox::checkChangeAndUpdate()
	{
		if (!isFocus_)
		{
			return false; // フォーカス以外は処理不要
		}

		FrameWork::Controller& cont = FrameWork::Controller::getInstance();

		// 入力中の内容取得
		char text[1024];
		DxLib::GetKeyInputString(text, inputHandle_);

		bool isChange = text_ != text; // 変更があったか判定

		text_ = text;

		// エンターまたはタブを押した時 または、最大長まで入力した時　次の要素へ移動
		if (cont.isKeyPressedNow(KEY_INPUT_TAB) || cont.isKeyPressedNow(KEY_INPUT_RETURN) || ((int)(text_.length()) >= maxLength_ && isChange))
		{
			shared_ptr<TextBox> nextInput = nextInput_.lock();
			if (nextInput)
			{
				nextInput->onForceFocus(); // 次の要素フォーカス
			}
			onBlurBase(); // フォーカス解除
		}

		return isChange;
	}

}