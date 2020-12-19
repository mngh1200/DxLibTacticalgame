#include "Mass.h"
#include "Map.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	Mass::Mass(int kindId) : kindId_(kindId)
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
		return COST[kindId_];
	}

	/**
	 * @fn
	 * 回避UP取得
	 */
	int Mass::getAgl() const
	{
		return AGL_UP[kindId_];
	}

	/**
	 * @fn
	 * 守備UP取得
	 */
	int Mass::getDef() const
	{
		return DEF_UP[kindId_];
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