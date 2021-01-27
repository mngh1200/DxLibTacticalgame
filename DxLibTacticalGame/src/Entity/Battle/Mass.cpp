#include "Mass.h"
#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	const vector<int> Mass::COST = { 1, 2, 2, INT_MAX, 1, 1 };
	const vector<int> Mass::AGL_UP = { 0, 20, -20, 0, 0, 0 };
	const vector<int> Mass::DEF_UP = { 0,  2,  -2, 0, 3, 3 };
	const vector<string> Mass::TEXT1 = {"草原", "森", "川", "山", "砦（自軍）", "砦（敵軍）"};

	/**
	 * @fn
	 * コンストラクタ
	 * @param (kindId) マスの種類
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
	 * @return 移動コスト
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
	 * @return 回避UP
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
	 * @return 守備UP
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
	 * @return マスの種類
	 */
	int Mass::getKind() const
	{
		return kindId_;
	}

	/**
	 * @fn
	 * 画像のIDを返す
	 * @return 画像ID
	 */
	int Mass::getImageId() const
	{
		return imageId_;
	}

	/**
	 * @fn
	 * 地形効果説明文を返す
	 * @param (line) 何行目の説明文を取得するか
	 * @return 説明文
	 */
	string Mass::getText(int line) const
	{
		if (kindId_ == Kind::OUT_OF_MAP)
		{
			return "";
		}

		if (line == 0)
		{
			return TEXT1.at(kindId_);
		}
		else if (line == 1)
		{
			int agl = getAgl();
			int def = getDef();

			if (getCost() == INT_MAX)
			{
				return "侵入不可";
			} 
			else if (agl == 0 && def == 0)
			{
				return "特になし";
			}
			string aglSign = agl >= 0 ? "+" : "";
			string defSign = def >= 0 ? "+" : "";

			return "回避 " + aglSign + to_string(agl) + " 防御 " + defSign + to_string(def);
		}

		return "";
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
		else if (kindId == Kind::FORT_PLAYER) // 砦（自軍）
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 4);
		}
		else if (kindId == Kind::FORT_ENEMY) // 砦（敵軍）
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 5);
		}
		else // 平原
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 1);
		}
	}
}