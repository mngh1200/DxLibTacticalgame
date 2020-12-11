#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Utility/ResourceManager.h"
#include "Utility/FontManager.h"
#include "Define.h"

/**
 * @file Back.h
 * @brief 背景要素
 */

namespace Entity
{
	class Back : public Object
	{
	public:
		Back();
		Back(int screen);

		~Back() {};
		void render() const override;

		enum Screen
		{
			UNSELECTED,
			SELECT
		};

	private:
		const static int SELECT_LEFT_WIDTH = 800;

		int imageId_; // イメージID

		int screen_; // 表示スクリーン
	};
}