#include "Map.h"
#include "Entity/Unit/Gunner.h"
#include "Screen/BattleScreen.h"

namespace Entity {
	/**
     * @fn
     * コンストラクタ
     */
	Map::Map()
	{
		type_ = Object::Type::MAP;

		shape_.set(0, MAP_Y, MAP_MASS_W * CHIP_SIZE, MAP_MASS_H * CHIP_SIZE);
	}

	/**
	 * @fn
	 * マップデータ読み込み
	 */
	void Map::loadStageData(std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H >& mapData)
	{
		mass_.reserve(MAP_MASS_H); // メモリ確保

		int y = 0;
		int x = 0;
		for (auto yItr = mapData.begin(); yItr != mapData.end(); ++yItr)
		{
			auto line = *yItr;
			mass_.push_back(vector<shared_ptr<Mass>>());
			mass_[y].reserve(MAP_MASS_W);
			for (auto xItr = line.begin(); xItr != line.end(); ++xItr)
			{
					int kind = *xItr;
					if (kind >= Mass::Kind::LEN) {
						kind = Mass::Kind::OUT_OF_MAP;
					}
					else if (kind == Mass::Kind::FORT_PLAYER) // 味方砦指定
					{
						playerFortMass_ = make_pair(x, y);
					}
					else if (kind == Mass::Kind::FORT_ENEMY) // 敵砦指定
					{
						enemyFortMass_ = make_pair(x, y);
					}
					mass_[y].push_back(make_shared<Mass>(kind));
					++x;
			}
			++y;
			x = 0;
		}
	}

	/**
	 * @fn
	 * ユニットデータ群の読み込み（大元はステージデータ）
	 * @param (units) ユニットデータ（x座標, y座標, 種類、味方(0)か敵か(1)）
	 */
	void Map::loadUnits(vector<vector<int>>& units)
	{
		for (auto itr = units.begin(); itr != units.end(); ++itr)
		{
			auto unit = *itr;

			if (unit.size() > 3)
			{
				setUnit(unit[0], unit[1], unit[2], unit[3] == 1);
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
	 * 特定マスにユニット設置
	 * @param (massX) マス座標x
	 * @param (massY) マス座標y
	 * @param (kind) ユニット種類
	 * @param (isEnemy) false: プレイヤーユニット, true: 敵ユニット
	 */
	void Map::setUnit(int massX, int massY, int kind, bool isEnemy)
	{

		if (isRange(massX, massY))
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			Entity::ObjectsControl& objectsControl = game.objectsControl;

			shared_ptr<Entity::Unit> unit;
			
			if (kind == UnitKey::GUNNER)
			{
				unit = make_shared<Entity::Gunner>();
			}
			else
			{
				unit = make_shared<Entity::Unit>();
			}

			unit->init(massX, massY, kind, isEnemy);

			units_.emplace(make_pair(massX, massY), unit); // 新規追加のみ

			if (!isEnemy)
			{
				objectsControl.addObject(Screen::BattleScreen::Layer::PLAYER_UNIT, unit);
			}
			else
			{
				objectsControl.addObject(Screen::BattleScreen::Layer::ENEMY_UNIT, unit);
			}
		}
	}

	/**
	 * @fn
	 * 特定マスのユニット取得
	 * @param (massX) マス座標X
	 * @param (massY) マス座標Y
	 */
	shared_ptr<Unit> Map::getUnit(int massX, int massY)
	{
		pair<int, int> pos = make_pair(massX, massY);
		if (units_.count(pos) > 0)
		{
			return units_.at(pos);
		}

		return nullptr;
	}

	/**
	 * @fn
	 * 対象ユニットを座標上から削除
	 * @param (unit) 対象ユニット
	 */
	void Map::eraseUnit(shared_ptr<Unit> unit)
	{
		units_.erase(make_pair(unit->getMassX(), unit->getMassY()));
	}


	/**
	 * @fn
	 * マスのイテレータの始点を返す
	 * @return マスのイテレータの始点
	*/
	vector<vector<shared_ptr<Mass>>>::iterator Map::massBegin()
	{
		return mass_.begin();
	}

	/**
	 * @fn
	 * マスのイテレータの終点を返す
	 * @return マスのイテレータの終点
	*/
	vector<vector<shared_ptr<Mass>>>::iterator Map::massEnd()
	{
		return mass_.end();
	}

	/**
	 * @fn
	 * ユニットリストのイテレータの始点を返す
	 * @return ユニットリストのイテレータの始点
	*/
	map<pair<int, int>, shared_ptr<Unit>>::iterator Map::unitsBegin()
	{
		return units_.begin();
	}

	/**
	 * @fn
	 * ユニットリストのイテレータの終点を返す
	 * @return ユニットリストのイテレータの終点
	*/
	map<pair<int, int>, shared_ptr<Unit>>::iterator Map::unitsEnd()
	{
		return units_.end();
	}

	/**
	 * @fn
	 * ユニットのマス移動（移動確定時）
	 * @param (unit) 対象ユニット
	*/
	void Map::confirmMove(shared_ptr<Unit> unit)
	{
		int baseX = unit->getBaseX();
		int baseY = unit->getBaseY();
		int massX = unit->getMassX();
		int massY = unit->getMassY();

		if (baseX != massX || baseY != massY) // 移動しているときのみ
		{
			unit->setPos(massX, massY);
			units_.emplace(make_pair(massX, massY), unit);
			units_.erase(make_pair(baseX, baseY));
		}
	}

	/**
	 * @fn
	 * 移動可能範囲表示
	 * @param (unit) 対象ユニット
	*/
	void Map::displayMovableRange(shared_ptr<Unit> unit)
	{
		if (unit)
		{
			bool isOwnUnit = !unit->isEnemy(); // 自軍ユニットであるか

			int move = unit->getInfo().mov;
			int x = unit->getMassX();
			int y = unit->getMassY();

			if (isOwnUnit)
			{
				// 初期位置からの攻撃範囲表示
				displayAtackAbleRange(unit, x, y, true);
			}

			vector<pair<int, int>> posList;
			getMovableMasses(unit, x, y, move, posList, true);

			if (isOwnUnit) // 自軍ユニット
			{
				for (auto itr = posList.begin(); itr != posList.end(); ++itr)
				{
					getMass(itr->first, itr->second)->state = Mass::State::MOVABLE; // 移動可能エリア表示
				}
			}
			else // 敵軍ユニットの場合
			{
				for (auto itr = posList.begin(); itr != posList.end(); ++itr)
				{
					displayAtackAbleRange(unit, itr->first, itr->second); // 攻撃可能エリア表示
				}
			}
			
		}
	}

	/**
	 * @fn
	 * 移動可能範囲の探索
	 * @param (unit) 対象ユニット
	 * @param (x) マス座標X
	 * @param (y) マス座標Y
	 * @param (move) 残移動コスト
	 * @param (isInit) 最初の探索であるか
	 * @param (isAtackAbleArea) 攻撃可能範囲の探索であるか
	*/
	void Map::getMovableMasses(shared_ptr<Unit> unit, int x, int y, int move, vector<pair<int, int>> &posList,  bool isInit)
	{
		shared_ptr<Mass> nowMass = getMass(x, y);

		// マップ外
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}

		shared_ptr<Unit> massUnit = getUnit(x, y);
		bool isOwnUnitOnMass = false; // 味方ユニットがマス上に存在するか

		
		if (!isInit) // 初回以外の処理
		{
			// マス上にユニットがいる場合(自身は無視)
			if (massUnit)
			{
				if (massUnit->isEnemy() != unit->isEnemy()) // 敵軍ユニットがいる場合は無視
				{
					return;
				}
				isOwnUnitOnMass = true;  // 味方ユニットがいる
			}

			// movコスト消費(初回はコスト消費しない)
			move = move - nowMass->getCost();
		}

		if (move > nowMass->passingMov)
		{
			// 自軍の別ユニットが既にいる場合は、移動対象マスにしない
			if (!isOwnUnitOnMass)
			{
				posList.push_back(make_pair(x, y));
			}

			// マス通過時のmovコストを保持
			nowMass->passingMov = move;

			getMovableMasses(unit, x - 1, y, move, posList, false);
			getMovableMasses(unit, x + 1, y, move, posList, false);
			getMovableMasses(unit, x, y - 1, move, posList, false);
			getMovableMasses(unit, x, y + 1, move, posList, false);
		}
	}

	/**
	 * @fn
	 * 現在地の攻撃可能範囲表示
	 * @param (unit) 対象ユニット
	 * @param (x) 攻撃起点マス座標X
	 * @param (y) 攻撃起点マス座標Y
	 * @param (isExistEnemyOnly) trueの場合、攻撃対象ユニットがいる場合のみ表示
	*/
	void Map::displayAtackAbleRange(shared_ptr<Unit> unit, int x, int y, bool isExistEnemyOnly)
	{
		// 攻撃可能のマスのリストを取得
		vector<pair<int, int>> posList;
		getAtackAbleMasses(unit, x, y, posList, isExistEnemyOnly);

		for (auto itr = posList.begin(); itr != posList.end(); ++itr)
		{
			getMass(itr->first, itr->second)->state = Mass::State::ATK_ABLE; // 攻撃可能エリア表示
		}
	}

	/**
	 * @fn
	 * 攻撃対象マスを取得
	 * @param (unit) 対象ユニット
	 * @param (x) 攻撃起点マス座標X
	 * @param (y) 攻撃起点マス座標Y
	 * @param (posList) 対象マスのリストを返すための参照
	 * @param (isExistEnemyOnly) trueの場合、攻撃対象ユニットがいる場合のみ表示
	*/
	void Map::getAtackAbleMasses(shared_ptr<Unit> unit, int x, int y, vector<pair<int, int>>& posList, bool isExistEnemyOnly)
	{
		if (unit)
		{
			if (!unit->isAtackable()) // ユニットが攻撃不可の状態
			{
				return;
			}

			int range = unit->getInfo().range;

			for (int i = 1; i <= range; i++)
			{
				getAtackAbleMass(unit, x - i, y, posList, isExistEnemyOnly);
				getAtackAbleMass(unit, x + i, y, posList, isExistEnemyOnly);
				getAtackAbleMass(unit, x, y - i, posList, isExistEnemyOnly);
				getAtackAbleMass(unit, x, y + i, posList, isExistEnemyOnly);
			}
		}
	}

	/**
	 * @fn
	 * 攻撃可能マスであるか判定
	 * @param (unit) 対象ユニット
	 * @param (x) 攻撃起点マス座標X
	 * @param (y) 攻撃起点マス座標Y
	 * @param (posList) 対象マスのリストを返すための参照
	 * @param (isExistEnemyOnly) trueの場合、攻撃対象ユニットがいる場合のみ表示
	*/
	void Map::getAtackAbleMass(shared_ptr<Unit> unit, int x, int y, vector<pair<int, int>>& posList, bool isExistEnemyOnly)
	{
		shared_ptr<Mass> nowMass = getMass(x, y);

		// マップ外
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}

		if (!isExistEnemyOnly)
		{
			posList.push_back(make_pair(x, y));
		}
		else
		{
			// 敵ユニットが存在する場合のみ攻撃可能にする
			shared_ptr<Unit> targetUnit = getUnit(x, y);
			if (targetUnit && targetUnit->isEnemy() != unit->isEnemy())
			{
				posList.push_back(make_pair(x, y));
			}
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
		return 0 <= x && x < MAP_MASS_W && 0 <= y && y < MAP_MASS_H;
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
	 * @param (massX) マス座標X
	 */
	int Map::getRealX(int massX)
	{
		return massX * CHIP_SIZE;
	}

	/**
	 * @fn
	 * マスの座標から正味の座標を取得(y座標)
	 * @param (massY) マス座標Y
	 */
	int Map::getRealY(int massY)
	{
		return massY * CHIP_SIZE + MAP_Y;
	}

	/**
	 * @fn
	 * 正味の座標からマスの座標を取得(x座標)
	 * @param (realX) 正味のX座標
	 */
	int Map::getMassX(int realX)
	{
		return realX / CHIP_SIZE;
	}

	/**
	 * @fn
	 * 正味の座標からマスの座標を取得(y座標)
	 * @param (realY) 正味のY座標
	 */
	int Map::getMassY(int realY)
	{
		return (realY - MAP_Y) / CHIP_SIZE;
	}

	/**
	 * @fn
	 * マス座標を基準にマス間の距離を取得
	 * @param x0 基準地点のX座標
	 * @param y0 基準地点のY座標
	 * @param x1 対象地点のX座標
	 * @param y1 対象地点のY座標
	 * @return マス座標基準の距離
	 */
	int Map::getMassDistance(int x0, int y0, int x1, int y1)
	{
		return abs(x0 - x1) + abs(y0 - y1);
	}



}