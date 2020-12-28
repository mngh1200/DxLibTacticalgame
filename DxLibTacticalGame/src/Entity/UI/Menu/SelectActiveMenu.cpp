#include "SelectActiveMenu.h"

namespace Entity {

	/**
	 * @fn
	 * 開始（表示）
	 * @param (unitX) 選択中のユニットのx座標
	 * @param (unitY) 選択中のユニットのy座標
	 */
	void SelectActiveMenu::start(int unitX, int unitY)
	{
		menuBtns_.clear();
		
		int size = 2; // テスト処理 ボタンの個数

		shape_.set(unitX + CHIP_SIZE, unitY + CHIP_SIZE, WIDTH, size * (BUTTON_HEIGHT + BUTTON_MARGIN) + PADDING * 2);

		// 右端で見切れる場合
		if (shape_.getX2() > WIN_W)
		{
			shape_.x = unitX - shape_.w;
		}

		// 最下部で見切れる場合
		if (shape_.getY2() > WIN_H)
		{
			shape_.y = unitY - shape_.h;
		}

		int buttonX = shape_.x + PADDING; // ボタンX座標
		int buttonY = shape_.y + PADDING; // ボタンY座標
		

		addMenuButton(ButtonKind::WAIT, "待機", buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN);
		addMenuButton(ButtonKind::CANCEL, "キャンセル", buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN);

		

		isDisplay_ = true;
		shape_.disabledHit = false;
	}

	/**
	 * @fn
	 * 終了（非表示化）
	 */
	void SelectActiveMenu::end()
	{
		shape_.disabledHit = true;
		isDisplay_ = false;
	}

	/**
	 * @fn
	 * 描画
	 */
	void SelectActiveMenu::render() const
	{
		if (!isDisplay_)
		{
			return;
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// メニュー枠
		DxLib::DrawRoundRectAA((float)shape_.x, (float)shape_.y, (float)shape_.getX2(), (float)shape_.getY2(), (float)RECT_ROUND, (float)RECT_ROUND, POS_NUM, rm.getColor(ColorType::MAIN_COLOR), TRUE);

		int x, y;
		if (isMouseDown_ || isMouseOver_)
		{
			DxLib::GetMousePoint(&x, &y);
		}

		// ボタン
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			int textColor = rm.getColor(ColorType::SUB_COLOR);
			int backgroundColor = rm.getColor(ColorType::MAIN_COLOR);

			if (isMouseDown_ && itr->shape.isHit(x, y))
			{
				textColor = rm.getColor(ColorType::MAIN_COLOR);
				backgroundColor = rm.getColor(ColorType::SUB_COLOR);
			}
			else if (isMouseOver_ && itr->shape.isHit(x, y))
			{
				textColor = rm.getColor(ColorType::MAIN_COLOR);
				backgroundColor = rm.getColor(ColorType::SUB_COLOR);
			}

			DxLib::DrawRoundRectAA((float)itr->shape.x, (float)itr->shape.y, (float)itr->shape.getX2(), (float)itr->shape.getY2(), (float)RECT_ROUND, (float)RECT_ROUND, POS_NUM, backgroundColor, TRUE);
			DxLib::DrawFormatStringToHandle(itr->shape.x + BUTTON_PADDING, itr->shape.y + BUTTON_PADDING, textColor, rm.getHdlFont(FontType::NORMAL_S24), itr->text.c_str());
		}
	}


	/**
	 * @fn
	 * 押下されているボタンのkey取得
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @return key
	 */
	int SelectActiveMenu::getHitButtonKey(int x, int y)
	{
		if (!isDisplay_)
		{
			return -1;
		}

		MenuButton* hitBtn = getHitMenuButton(x, y);

		if (hitBtn == nullptr)
		{
			return -1;
		}

		return hitBtn->key;
	}

	/**
	 * @fn
	 * ボタン追加
	 * @param (key) ボタンのkey
	 * @param (text) テキスト
	 * @param (buttonX) x座標
	 * @param (buttonY) y座標
	 * @param (buttonW) 幅
	 * @param (buttonH) 高さ
	 * @param (margin) 上下の余白
	 */
	void SelectActiveMenu::addMenuButton(int key, string text, int buttonX, int buttonY, int buttonW, int buttonH, int margin)
	{
		int size = menuBtns_.size();
		menuBtns_.push_back(MenuButton{ key, text, Shape(buttonX, buttonY + (buttonH + margin) * size, buttonW, buttonH)});
	}


	/**
	 * @fn
	 * 押下されているボタンを取得
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @return 押下されているボタン
	 */
	MenuButton* SelectActiveMenu::getHitMenuButton(int x, int y)
	{
		for (auto itr = menuBtns_.begin(); itr != menuBtns_.end(); ++itr)
		{
			if (itr->shape.isHit(x, y))
			{
				return &*itr;
			}
		}

		return nullptr;
	}

}