#pragma once
#include <climits>
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class ViewObject
	{
	public:
		ViewObject() : objectType(ViewObject::ObjectType::VIEW), shape_(Shape(0,0,0,0)) {};
		ViewObject(Shape shape) : objectType(ViewObject::ObjectType::VIEW), shape_(shape) {}
		virtual ~ViewObject() {};

		virtual void render() const;

		/**
		 * @enum ObjectType
		 * オブジェクトの種類
		 */
		enum ObjectType {
			VIEW,
			TEXT
		};

		//! ViewObjectの種類
		ObjectType objectType;

	protected:
		//! オブジェクトの形状(位置やサイズ)
		Shape shape_;
	};
}