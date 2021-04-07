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
		ImageView() : x_(0), y_(0), imageHandle_(-1) {};
		~ImageView() {};

		void setPos(int x, int y);
		void setImageId(int imageHandle);

		void render() const;

	private:
		//! 座標
		int x_, y_;

		//! イメージハンドル
		int imageHandle_;
	};
}