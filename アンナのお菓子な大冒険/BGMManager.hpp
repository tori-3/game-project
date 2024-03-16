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

	static BGMManager& get() {
		static BGMManager instance;
		return instance;
	}

public:

	void play(AssetNameView name) {

		if (m_name != name) {
			if (m_name) {
				AudioAsset{ m_name }.stop(0.1s);
			}
			if (name) {
				AudioAsset{ name }.play(0.2s);
			}
			m_name = name;
		}	
	}

	void setVolume(double volume) {
		if (m_name) {
			AudioAsset{ m_name }.setVolume(volume);
		}
	}

private:
	AssetName m_name;
};
