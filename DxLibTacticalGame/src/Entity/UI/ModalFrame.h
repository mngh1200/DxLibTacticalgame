#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"

using namespace std;

/**
 * @file ModalFrame.h
 * @brief モーダルのフレーム要素クラス
 */

namespace Entity
{
	class ModalFrame : public Object
	{
	public:
		ModalFrame();
		~ModalFrame() {};

		constexpr static int HEAD_H = 46; //! ヘッダーの高さ

		void setShape(int x, int y, int w, int h);
		void setPos(int x, int y);
		void setSize(int w, int h);

		void setTitle(string title);

		void setValidOverlay(bool isValid);

		void setColor(int colorType);

		void render() const override;

		const Shape& getFrameShape() const { return frameBodyShape_; }; // 矩形座標取得

	private:
		constexpr static int OVERLAY_ALPHA = 100; //! オーバレイの不透明度

		constexpr static int TITLE_FONT_TYPE = FontType::NORMAL_S24;
		constexpr static int HEAD_PADDING = 10; //! ヘッダー余白

		Shape frameBodyShape_; //! フレーム本体の矩形座標（オーバーレイ考慮）

		int color_; //! 色

		bool isOverlay_; //! オーバーレイの有無

		string title_; //! ヘッダータイトル
	};
}