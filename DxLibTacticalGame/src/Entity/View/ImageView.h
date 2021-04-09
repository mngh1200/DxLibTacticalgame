#pragma once
#include <climits>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "DxLib.h"

using namespace std;

/**
 * @file ImageView.h
 * @brief 画面内画像要素
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
		//! イメージハンドル
		int imageHandle_;
	};
}