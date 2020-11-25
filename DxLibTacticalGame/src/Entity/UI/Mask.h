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
		//! ‰æ–ÊI—¹‚©•\¦‚©
		bool isOpen_;

		//! “§‰ß“x
		int alpha_;

	};
}