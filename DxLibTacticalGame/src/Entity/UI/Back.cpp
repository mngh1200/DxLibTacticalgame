#include "Back.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	Back::Back() : imageId_(0), screen_(UNSELECTED)
	{
		type_ = BACKGROUND;
		shape_ = Shape(0, 0, WIN_W, WIN_H);


		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		typedef Utility::ResourceManager RM;
		imageId_ = rm.getResource(RM::ResourceType::IMAGE, RM::BACKGROUND_MENU, 0);
	}

	/**
	 * @fn
	 * コンストラクタ
	 */
	Back::Back(int screen) : Back()
	{
		screen_ = screen;

		if (screen_ == Screen::SELECT)
		{
			imageId_ = -1;
		}
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void Back::render() const
	{
		if (imageId_ != -1)
		{
			DxLib::DrawGraph(shape_.x, shape_.y, imageId_, FALSE);
		}
		else if (screen_ == Screen::SELECT)
		{
			Utility::FontManager& fontManager = Utility::FontManager::getInstance();
			DxLib::DrawBox(shape_.x, shape_.y, SELECT_LEFT_WIDTH, shape_.getY2(), fontManager.getColor(ColorType::SUB_COLOR), TRUE);
			DxLib::DrawBox(shape_.x + SELECT_LEFT_WIDTH, shape_.y, shape_.getX2(), shape_.getY2(), fontManager.getColor(ColorType::MAIN_COLOR), TRUE);
		}
	}
}