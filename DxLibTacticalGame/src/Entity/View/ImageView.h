#pragma once
#include <climits>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "DxLib.h"

using namespace std;

/**
 * @file ImageView.h
 * @brief ��ʓ��摜�v�f
 */

namespace Entity
{
	class ImageView : public Figure
	{
	public:
		ImageView() : imageHandle_(-1) {};
		~ImageView() {};

		void setPos(int x, int y);
		void setImageId(int imageHandle);

		void render() const;

	private:
		//! �C���[�W�n���h��
		int imageHandle_;
	};
}