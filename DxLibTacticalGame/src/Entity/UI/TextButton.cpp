#include "TextButton.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	TextButton::TextButton(int colorType, int textColorType) :
		text_(""),
		textX_(0),
		textY_(0),
		sizeAnimationScale_(1),
		sizeAnimation_{},
		nowState_(State::NORMAL)
	{
		type_ = Type::BUTTON;
		baseShape_ = shape_;

		Utility::FontManager& fontManager = Utility::FontManager::getInstance();

		for (int i = 0; i < State::LEN; ++i)
		{
			color_[State::NORMAL] = fontManager.getColor(colorType);
			textColor_[State::NORMAL] = fontManager.getColor(textColorType);
		}

		font_ = fontManager.getHdlFont(FontType::NORMAL);
	}

	/**
	 * @fn
	 * ボタン位置とサイズ指定
	 * @param (x) X座標
	 * @param (y) Y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void TextButton::setShape(int x, int y, int w, int h)
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
	void TextButton::setPos(int x, int y)
	{
		// テキストとボタンの相対座標を計算
		baseShape_.x = shape_.x = x;
		baseShape_.y = shape_.y = y;
	}

	/**
	 * @fn
	 * ボタンサイズ指定
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void TextButton::setSize(int w, int h)
	{
		baseShape_.w = shape_.w = w;
		baseShape_.h = shape_.h = h;

		// 中央揃えにするため座標取得
		int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text_.c_str());
		textX_ = (shape_.w - width) / 2;

		// 中段に表示するための相対座標取得
		int fontHeight = DxLib::GetFontSizeToHandle(font_);
		textY_ = (shape_.h - fontHeight) / 2;
	}

	/**
	 * @fn
	 * テキスト指定
	 * @param (text) テキスト
	 */
	void TextButton::setText(string text)
	{
		text_ = text;
	}

	/**
	 * @fn
	 * テキスト指定
	 * @param (text) テキスト
	 * @param (x) ボタンとテキストの相対座標
	 * @param (fontType) フォント
	 */
	void TextButton::setText(string text, int x, int fontType)
	{
		text_ = text;
		textX_ = x;
		setFont(fontType);
	}

	/**
	 * @fn
	 * テキスト指定(ボタンの中央に配置)
	 * @param (text) テキスト
	 * @param (fontType) フォント
	 */
	void TextButton::setTextCenter(string text, int fontType)
	{
		text_ = text;
		setFont(fontType);

		// 中央揃えにするため座標取得
		int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text_.c_str());
		textX_ = (shape_.w - width) / 2;
	}

	/**
	 * @fn
	 * フォント指定
	 * @param (fontType) フォント
	 */
	void TextButton::setFont(int fontType)
	{
		Utility::FontManager& fontM = Utility::FontManager::getInstance();
		font_ = fontM.getHdlFont(fontType);

		// 中段に表示するための相対座標取得
		int fontHeight = DxLib::GetFontSizeToHandle(font_);
		textY_ = (shape_.h - fontHeight) / 2;
	}

	void TextButton::setColor(int backgroundColor, int textColor, int state)
	{
		color_[state] = backgroundColor;
		textColor_[state] = textColor;
	}

	/**
	 * @fn
	 * 描画
	 */
	void TextButton::render() const
	{
		// ボタン
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), color_[nowState_], TRUE);

		// テキスト
		DxLib::DrawFormatStringToHandle(shape_.x + textX_, shape_.y + textY_, textColor_[nowState_], font_, text_.c_str());
	}

	/**
	 * @fn
	 * アニメーション作成(ObjectsControl::addAnimationObjメソッド専用で呼び出す)
	 * @return アニメーション作成可能な場合trueを返す
	 */
	bool TextButton::createAnimation(int animationId)
	{
		// 既に実行済みのアニメーションがある場合は作成不可
		if (animationId == animationId_)
		{
			return false;
		}

		if (animationId == EXPANSION) // 拡大
		{
			sizeAnimation_.reset(Animation::NORMAL);
			return true;
		}
		else if (animationId == SHRINK) // 縮小
		{
			sizeAnimation_.reset(Animation::REVERSE);
			sizeAnimation_.adjustFrame(shape_, baseShape_, sizeAnimationScale_);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * 拡大、縮小アニメーションをセット
	 * @param (scale) 倍率
	 * @param (timeMs) 時間[ms]
	 */
	void TextButton::setSizeAnimation(float scale, int timeMs)
	{
		sizeAnimationScale_ = scale;
		sizeAnimation_ = Animation(timeMs);
	}

	/**
	 * @fn
	 * アニメーション更新
	 * @return true:終了
	 */
	bool TextButton::animationUpdate()
	{
		if (animationId_ == EXPANSION || animationId_ == SHRINK)
		{
			return sizeAnimation_.update(&shape_, baseShape_, sizeAnimationScale_);
		}
		return true;
	}


	/**
	 * @fn
	 * マウスが左ボタンで押下された瞬間の処理
	 */
	void TextButton::onMouseLeftDown()
	{
		if (ableSizeAnimation())
		{
			joinAnimationList(EXPANSION); // 拡大
		}

		// サウンド
		Utility::FontManager& fontM = Utility::FontManager::getInstance();
		DxLib::PlaySoundMem(fontM.getSound(SoundKind::MOUSE_DOWN), DX_PLAYTYPE_BACK);

	}

	/**
	 * @fn
	 * マウスの左ボタンが押し上げられた瞬間の処理
	 */
	void TextButton::onMouseLeftUp()
	{
		if (ableSizeAnimation())
		{
			joinAnimationList(SHRINK); // 縮小
		}
	}

	/**
	 * @fn
	 * マウスにホバーされた瞬間の処理
	 */
	void TextButton::onMouseOver()
	{
		if (ableSizeAnimation())
		{
			joinAnimationList(EXPANSION);
		}
	}

	/**
	 * @fn
	 * マウスがホバー状態から離れたときの処理
	 */
	void TextButton::onMouseOut()
	{
		nowState_ = State::NORMAL;

		if (isMouseDown_ && ableSizeAnimation())
		{
			joinAnimationList(SHRINK); // 縮小
		}
	}

	/**
	 * @fn
	 * アニメーション可能か
	 * @return true:可能
	 */
	bool TextButton::ableSizeAnimation() const
	{
		return sizeAnimationScale_ != 0;
	}

}