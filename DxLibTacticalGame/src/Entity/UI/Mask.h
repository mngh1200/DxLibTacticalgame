#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace
{
	const int COLOR = DxLib::GetColor(0, 0, 0);
	constexpr int ADD_ALPHA = 25;
}

namespace Entity
{
	class Mask : public Object
	{
	public:
		Mask();
		Mask(bool isOpen);
		~Mask() {};

		bool animationUpdate() override;

		void render() const override;

	private:
		//! 画面終了時か表示時か
		bool isOpen_;

		//! 透過度
		int alpha_;

	};
}