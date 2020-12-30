#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	Map::Map()
	{
		type_ = Object::Type::MAP;

		weak_ptr<Unit> tmpUnit;

		if (tmpUnit.expired()) {
			int a = 10;
		}

		// テスト処理
		w_ = 20;
		h_ = 10;

		shape_.set(0, 0, w_ * CHIP_SIZE, h_ * CHIP_SIZE);

		mass_.reserve(h_); // メモリ確保

		// マップデータ生成
		for (int y = 0; y < h_; y++)
		{
			mass_.push_back(vector<shared_ptr<Mass>>());
			mass_[y].reserve(w_); // メモリ確保
			
			for (int x = 0; x < w_; x++)
			{
				int kind = Mass::Kind::PLAIN;

				if (x == 0 || x == w_ - 1)
				{
					kind = Mass::Kind::MOUNTAIN;
				}
				else if (y == 5)
				{
					kind = Mass::Kind::RIVER;
				}
				else if (x == 5)
				{
					kind = Mass::Kind::FOREST;
				}

				mass_[y].push_back(make_shared<Mass>(kind));
			}
		}
	}

	/**
	 * @fn
	 * 描画
	 */
	void Map::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int x = 0;
		int y = 0;

		int mouseX = -1;
		int mouseY = -1;

		if (isMouseOver_) // ホバー時のみ取得
		{
			DxLib::GetMousePoint(&mouseX, &mouseY);
			mouseX = getMassX(mouseX);
			mouseY = getMassY(mouseY);
		}

		for (auto line = mass_.begin(); line != mass_.end(); ++line) {
			for (auto cell = (*line).begin(); cell != (*line).end(); ++cell) {
				int realX = getRealX(x);
				int realY = getRealY(y);
				DxLib::DrawGraph(realX, realY, (*cell)->getImageId(), FALSE);

				
				if (isMouseOver_) // ホバー時の描画
				{
					if (x == mouseX && y == mouseY)
					{
						drawHoverMass(realX, realY);
					}
				}

				if ((*cell)->state == Mass::State::NORMAL)
				{
					// 何もしない
				}
				if ((*cell)->state == Mass::State::MOVABLE) // 移動範囲
				{
					drawMoveableMass(realX, realY);
				}
				else if ((*cell)->state == Mass::State::ATK_ABLE) // 攻撃可能範囲
				{
					drawAtackMass(realX, realY);
				}
				++x;
			}
			++y;
			x = 0;
		}
	}

	/**
	 * @fn
	 * マスのポインタを返す
	 * @param (x) マスのx座標
	 * @param (y) マスのy座標
	 * @return マス
	 */
	shared_ptr<Mass> Map::getMass(int massX, int massY)
	{
		if (isRange(massX, massY))
		{
			try
			{
				return mass_.at(massY).at(massX);
			}
			catch (out_of_range&) {}
		}

		shared_ptr<Mass> mass = make_shared<Mass>();
		return mass;
	}

	/**
	 * @fn
	 * マスの状態を初期化する
	 */
	void Map::clearMassState()
	{
		for (auto line = mass_.begin(); line != mass_.end(); ++line) {
			for (auto cell = (*line).begin(); cell != (*line).end(); ++cell) {
				(*cell)->state = Mass::State::NORMAL;
				(*cell)->passingMov = -1;
			}
		}
	}

	/**
	 * @fn
	 * マップ範囲内であるか判定
	 * @param (x) マスのx座標
	 * @param (y) マスのy座標
	 * @return 範囲内である場合はtrueを返す
	 */
	bool Map::isRange(int x, int y) const
	{
		return 0 <= x && x < w_ && 0 <= y && y < h_;
	}


	/**
	 * @fn
	 * 移動可能範囲のマスを描画
	 * @param (realX) x座標
	 * @param (realY) y座標
	 */
	void Map::drawMoveableMass(int realX, int realY)
	{
		DxLib::DrawGraph(realX, realY, Utility::ResourceManager::getInstance().getImage(ImageType::IMAGE, ImageId::MASS_MOVE), TRUE);

		//DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
		//DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, rm.getColor(ColorType::PLAYER_COLOR), TRUE);
		//DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, rm.getColor(ColorType::PLAYER_COLOR), FALSE);
	}

	/**
	 * @fn
	 * 攻撃可能範囲のマスを描画
	 * @param (realX) x座標
	 * @param (realY) y座標
	 */
	void Map::drawAtackMass(int realX, int realY)
	{
		DxLib::DrawGraph(realX, realY, Utility::ResourceManager::getInstance().getImage(ImageType::IMAGE, ImageId::MASS_ATACK), TRUE);
	}

	/**
	 * @fn
	 * ホバーマスを描画
	 * @param (realX) x座標
	 * @param (realY) y座標
	 */
	void Map::drawHoverMass(int realX, int realY)
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, MASS_EFFECT_ALPHA);
		DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, Utility::ResourceManager::getInstance().getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/**
	 * @fn
	 * 選択中のマスを描画
	 * @param (realX) x座標
	 * @param (realY) y座標
	 */
	void Map::drawSelectedMass(int realX, int realY)
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, MASS_EFFECT_ALPHA);
		DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, Utility::ResourceManager::getInstance().getColor(ColorType::MAIN_COLOR_ON), TRUE);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/**
	 * @fn
	 * マスの座標から正味の座標を取得(x座標)
	 */
	int Map::getRealX(int massX)
	{
		return massX * CHIP_SIZE;
	}

	/**
	 * @fn
	 * マスの座標から正味の座標を取得(y座標)
	 */
	int Map::getRealY(int massY)
	{
		return massY * CHIP_SIZE;
	}

	/**
	 * @fn
	 * 正味の座標からマスの座標を取得(x座標)
	 */
	int Map::getMassX(int realX)
	{
		return realX / CHIP_SIZE;
	}

	/**
	 * @fn
	 * 正味の座標からマスの座標を取得(y座標)
	 */
	int Map::getMassY(int realY)
	{
		return realY / CHIP_SIZE;
	}

}