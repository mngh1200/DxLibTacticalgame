#include "Back.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
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
	 * �R���X�g���N�^
	 */
	Back::Back(int screen) : Back()
	{
		screen_ = screen;

		if (screen_ == Screen::SELECT || screen_ == Screen::BATTLE || screen_ == Screen::NETWORK)
		{
			imageId_ = -1;
		}
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Back::render() const
	{
		Utility::ResourceManager& resourceManager = Utility::ResourceManager::getInstance();

		if (imageId_ != -1)
		{
			DxLib::DrawGraph(shape_.x, shape_.y, imageId_, FALSE);
		}
		else if (screen_ == Screen::SELECT) // �I�����
		{
			DxLib::DrawBox(shape_.x, shape_.y, SELECT_LEFT_WIDTH, shape_.getY2(), resourceManager.getColor(ColorType::SUB_COLOR), TRUE);
			DxLib::DrawBox(shape_.x + SELECT_LEFT_WIDTH, shape_.y, shape_.getX2(), shape_.getY2(), resourceManager.getColor(ColorType::MAIN_COLOR), TRUE);
		}
		else if (screen_ == Screen::BATTLE) // �퓬���
		{
			DxLib::DrawBox(shape_.x, shape_.y, WIN_W, WIN_H, resourceManager.getColor(ColorType::SUB_COLOR_DARK), TRUE);
		}
		else if (screen_ == Screen::NETWORK) // �ʐM�ΐ폀�����
		{
			DxLib::DrawBox(shape_.x, shape_.y, WIN_W, WIN_H, resourceManager.getColor(ColorType::SUB_COLOR_LITE), TRUE);
		}
	}
}