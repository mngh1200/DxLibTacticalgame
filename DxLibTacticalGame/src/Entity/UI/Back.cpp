#include "Back.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Back::Back() : imageId_(0)
	{
		type_ = BACKGROUND;
		shape_ = Shape(0, 0, WIN_W, WIN_H);


		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		typedef Utility::ResourceManager RM;
		imageId_ = rm.getResource(RM::ResourceType::IMAGE, RM::BACKGROUND_MENU, 0);
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Back::render() const
	{
		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, FALSE);
	}
}