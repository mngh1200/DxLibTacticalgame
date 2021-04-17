#include "Back.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Back::Back() : imageId_(-1)
	{
		type_ = BACKGROUND;
		shape_ = Shape(0, 0, WIN_W, WIN_H);

		init(BackKind::NORMAL_IMAGE);
	}

	/**
	 * @fn
	 * ��������
	 * @param (kind) �w�i�̎��
	 */
	void Back::init(int kind)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (kind == BackKind::NORMAL_IMAGE)
		{
			imageId_ = rm.getImage(ImageType::IMAGE, ImageId::BACKGROUND_IMAGE_ID);
		}
		else if (kind == BackKind::DARK_IMAGE)
		{
			imageId_ = rm.getImage(ImageType::IMAGE, ImageId::BACKGROUND_DARK_IMAGE_ID);
		}
		else // MONO_COLOR (�P�F)
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
		else // �P�F
		{
			DxLib::DrawBox(shape_.x, shape_.y, WIN_W, WIN_H, resourceManager.getColor(ColorType::SUB_COLOR_DARK), TRUE);
		}
	}
}