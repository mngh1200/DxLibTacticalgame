#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Back : public Object
	{
	public:
		Back();
		~Back() {};

		void init(int layerId, int objectId, Shape shape) override;
		void render() const override;
	};
}