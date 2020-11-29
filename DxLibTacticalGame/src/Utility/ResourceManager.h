#pragma once
#include "Define.h"
#include "DxLib.h"
#include <map>
#include <vector>
using namespace std;
namespace Utility
{
	class ResourceManager
	{
	public:
		enum ResourceType {
			PLAYER,
			ENEMY,
			MAP,
			IMAGE
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