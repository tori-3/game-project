#pragma once

class BGMManager {
private:
	BGMManager() = default;
	~BGMManager() = default;

public:
	BGMManager(const BGMManager&) = delete;
	BGMManager& operator=(const BGMManager&) = delete;
	BGMManager(BGMManager&&) = delete;
	BGMManager& operator=(BGMManager&&) = delete;

	static BGMManager& get()
	{
		static BGMManager instance;
		return instance;
	}

public:

	void play(AssetNameView name,Duration fadeinTime=0.2s, Duration fadeoutTime = 0.1s)
	{

		if (m_name != name) {
			if (m_name) {
				AudioAsset{ m_name }.stop(fadeoutTime);
			}
			if (name) {
				AudioAsset{ name }.play(fadeinTime,MixBus1);
			}
			m_name = name;
		}	
	}

	void stop(Duration fadeoutTime = 0.5s)
	{
		AudioAsset{ m_name }.stop(fadeoutTime);
		m_name = {};
	}

private:
	AssetName m_name;
};
