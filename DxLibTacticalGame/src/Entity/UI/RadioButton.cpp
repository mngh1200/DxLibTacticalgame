#include "RadioButton.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	RadioButton::RadioButton() :
		relationTextY_(0),
		selectedNum_(0),
		items_ {},
		itemW_ (0),
		itemH_ (0)
	{
		// デフォルト指定
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		font_ = rm.getHdlFont(FontType::NORMAL_S24);

		backColor_ = rm.getColor(ColorType::SUB_COLOR_LITE);
		textColor_ = rm.getColor(ColorType::SUB_COLOR);
		selectedBackColor_ = rm.getColor(ColorType::MAIN_COLOR_ON);
	}

	/**
	 * @fn
	 * ボタン位置指定
	 * @param (x) X座標
	 * @param (y) Y座標
	 */
	void RadioButton::setPos(int x, int y)
	{
		// テキストとボタンの相対座標を計算
		shape_.x = x;
		shape_.y = y;

		adjustShape();
	}

	/**
	 * @fn
	 * 項目ボタンのサイズ指定
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void RadioButton::setItemSize(int w, int h)
	{
		itemW_ = w;
		shape_.h = itemH_ = h;
		
		relationTextY_ = (h - DxLib::GetFontSizeToHandle(font_)) / 2;

		adjustShape();
	}

	/**
	 * @fn
	 * 項目のテキスト指定
	 * @param (textList) 項目のテキストリスト
	 */
	void RadioButton::setItems(vector<string>& textList)
	{
		for (auto itr = textList.begin(); itr != textList.end(); ++itr)
		{
			items_.push_back(RadioButtonItem{ *itr });
		}

		adjustShape();
	}

	/**
	 * @fn
	 * 色指定
	 * @param (backColorType)  背景色
	 * @param (selectedBackColor)  選択時の背景色
	 * @param (textColorType)  テキスト色
	 */
	void RadioButton::setColor(int backColorType, int selectedBackColorType, int textColorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		backColor_ = rm.getColor(backColorType);
		selectedBackColor_ = rm.getColor(selectedBackColorType);
		textColor_ = rm.getColor(textColorType);
	}

	/**
	 * @fn
	 * フォント指定
	 * @param (fontType) フォントタイプ
	 */
	void RadioButton::setFont(int fontType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		font_ = rm.getHdlFont(fontType);

		// テキストの相対位置調整
		relationTextY_ = (shape_.h - DxLib::GetFontSizeToHandle(font_)) / 2;

		setTextCenter();
	}

	/**
	 * @fn
	 * 特定の項目選択
	 * @param (num) 選択する項目番号
	 */
	void RadioButton::select(int num)
	{
		selectedNum_ = num;
	}

	/**
	 * @fn
	 * 描画
	 */
	void RadioButton::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// 各項目
		int count = 0;
		const int textY = shape_.y + relationTextY_;
		for (auto itr = items_.begin(); itr != items_.end(); ++itr)
		{
			RadioButtonItem item = *itr;

			int backColor = backColor_;

			if (count == selectedNum_)
			{
				backColor = selectedBackColor_;
			}

			// 背景
			DxLib::DrawBox(item.shape.x, item.shape.y, item.shape.getX2(), item.shape.getY2(), backColor, TRUE);

			// テキスト
			DxLib::DrawFormatStringToHandle(item.shape.x + item.relationTextX, textY, textColor_, font_, item.name.c_str());
		
			++count;
		}
	}

	/**
	 * @fn
	 * クリックイベント
	 * @param (x) x座標
	 * @param (y) y座標
	 */
	void RadioButton::onMouseClick(int x, int y)
	{
		int count = 0;
		for (auto itr = items_.begin(); itr != items_.end(); ++itr)
		{
			if ((*itr).shape.isHit(x, y)) // 項目選択時
			{
				selectedNum_ = count;
				Utility::ResourceManager::playSound(SoundKind::CHECK);
				break;
			}
			++count;
		}
	}

	/**
	 * @fn
	 * 選択テキスト
	 * @param (text) 選択しているテキスト返却用の参照
	 */
	void RadioButton::getSelectedText(string& text) const
	{
		text = items_.at(selectedNum_).name;
	}

	/**
	 * @fn
	 * テキストが中央揃え
	 */
	void RadioButton::setTextCenter()
	{
		for (auto itr = items_.begin(); itr != items_.end(); ++itr)
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, (*itr).name.c_str());
			(*itr).relationTextX = (itemW_ - width) / 2;
		}
	}

	/**
	 * @fn
	 * 全体の幅調整
	 */
	void RadioButton::adjustShape()
	{
		shape_.w = itemW_ * items_.size();

		int i = 0;
		for (auto itr = items_.begin(); itr != items_.end(); ++itr)
		{
			(*itr).shape = Shape(shape_.x + i * (itemW_ + ITEM_MARGIN), shape_.y, itemW_, itemH_);
			++i;
		}

		setTextCenter();
	}

}