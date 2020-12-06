#pragma once
#include "Define.h"
#include "DxLib.h"
#include <map>
#include <vector>
using namespace std;

/**
 * @file Resource Manager.h
 * @brief リソースを管理するクラス
 */
namespace Utility
{
	class ResourceManager
	{
	public:
		// リソースタイプ
		enum ResourceType {
			PLAYER,
			ENEMY,
			MAP,
			IMAGE
		};
		// PLAYER、ENEMYのりソースの格納場所
		enum UnitKey {
			LANCER,
			CAVALRY,
			GUNNER
		};
		// PLAYER、ENEMYのアニメーション開始位置(仮)
		enum UnitStartPos {
			TOP,
			LEFT=4,
			RIGHT=8,
			DOWN=12
		};

		// ImageのID
		enum ImageId {
			BACKGROUND_MENU,
			IMAGE_ID_LEN
		};

		//for singleton
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		static ResourceManager& getInstance();

		int load();

		int getResource(ResourceType type, int kind, int pos) const;
	private:
		ResourceManager()
			:resource_{}
		{};
		~ResourceManager() {};

		void loadMapchip(const char* resourcePath, int* handle);
		void loadCharacterchip(const char* resourcePath, int* handle);
		void loadImage(const char* resourcePath, int* handle);

		map<ResourceType, std::vector<int*>> resource_;
		
		int hdlMapchip_[8];
	};
}