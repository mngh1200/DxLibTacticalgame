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
		imageId_ = rm.getImage(ImageType::IMAGE, BACKGROUND_MENU, 0);
	}

	/**
	 * @fn
	 * 初期処理
	 * @param (screenKind) 画面の種類
	 */
	void Back::init(int screenKind)
	{
		screen_ = screenKind;

		if (screen_ == ScreenKind::BATTLE_SCREEN)
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
		Utility::ResourceManager& resourceManager = Utility::ResourceManager::getInstance();

		if (imageId_ != -1)
		{
			DxLib::DrawGraph(shape_.x, shape_.y, imageId_, FALSE);
		}
		else if (screen_ == ScreenKind::BATTLE_SCREEN) // 戦闘画面
		{
			DxLib::DrawBox(shape_.x, shape_.y, WIN_W, WIN_H, resourceManager.getColor(ColorType::SUB_COLOR_DARK), TRUE);
		}
	}
}