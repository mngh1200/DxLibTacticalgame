#pragma once
#include <climits>
#include "Unit.h"

using namespace std;

/**
 * @file Gunner.h
 * @brief 銃兵ユニットのクラス
 */

namespace Entity
{

	class Gunner : public Unit
	{
	public:
		Gunner() : 
			isLoaded_(true), 
			isAtacked_(false),
			isBulletExist_(false),
			bulletX_(0),
			bulletY_(0),
			bulletAnime_{}
		{};
		~Gunner() {};

		constexpr static int ATTENUATION = 15; //! 命中率の距離減衰

		bool isAtackable() const override; // 攻撃可能であるか

	protected:
		void createAtackAnimation() override;
		bool updateAtackAnimation() override;

		void renderExtra() const override;
		void turnEndExtra(bool isOwnEnd) override;
		void getExtraStatusListExtra(vector<pair<string, string>>& list) const override;

	private:
		constexpr static int UNLOADED_MARGIN_BOTTOM = 3; //! 未装填描画の余白
		constexpr static int UNLOADED_MARGIN_RIGHT = CHIP_SIZE / 4; //! 未装填描画の余白

		bool isAtacked_; //! このターン攻撃したか
		bool isLoaded_; //! 装填済であるか
		bool isBulletExist_; //! 弾丸移動中であるか

		Animation bulletAnime_; //! 弾丸用のアニメーションクラス

		int bulletX_, bulletY_; //! 弾丸の座標
	};


}