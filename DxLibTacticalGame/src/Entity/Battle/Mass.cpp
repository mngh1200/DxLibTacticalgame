#include "Mass.h"
#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	const static vector<int> COST = { 1, 2, 2, INT_MAX }; //! 移動コスト
	const static vector<int> AGL_UP = { 0, 20, -20, 0 };  //! 回避UP
	const static vector<int> DEF_UP = { 0,  3,  -3, 0 };  //! 防御UP

	/**
	 * @fn
	 * コンストラクタ
	 */
	Mass::Mass(int kindId) : kindId_(kindId), state(State::NORMAL), passingMov(-1)
	{
		if (0 > kindId_ || kindId_ >= Kind::LEN)
		{
			kindId_ = Kind::PLAIN;
		}

		setImageIdFromKind(kindId);
	}

	/**
	 * @fn
	 * 移動コスト取得
	 */
	int Mass::getCost() const
	{
		try
		{
			return COST.at(kindId_);
		}
		catch (out_of_range&) {}
		
		return INT_MAX;
	}

	/**
	 * @fn
	 * 回避UP取得
	 */
	int Mass::getAgl() const
	{
		try
		{
			return AGL_UP.at(kindId_);
		}
		catch (out_of_range&) {}

		return 0;
	}

	/**
	 * @fn
	 * 守備UP取得
	 */
	int Mass::getDef() const
	{
		try
		{
			return DEF_UP.at(kindId_);
		}
		catch (out_of_range&) {}

		return 0;

	}

	/**
	 * @fn
	 * マスの種類を返す
	 */
	int Mass::getKind() const
	{
		return kindId_;
	}

	/**
	 * @fn
	 * 画像のIDを返す
	 */
	int Mass::getImageId() const
	{
		return imageId_;
	}

	/**
	 * @fn
	 * 画像の種類を元に画像IDをセット
	 * @param (kindId) 画像の種類
	 */
	void Mass::setImageIdFromKind(int kindId)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// テスト処理
		if (kindId == Kind::MOUNTAIN) // 山
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 0);
		}
		else if (kindId == Kind::FOREST) // 森
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 2);
		}
		else if (kindId == Kind::RIVER) // 川
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 3);
		}
		else // 平原
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 1);
		}
	}
}