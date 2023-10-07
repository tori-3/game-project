#pragma once
class TalkWindow {

	double accumlater = 0;

	size_t lastLength = 0;

	class TalkInfo {
	public:
		String name;
		String text;
	};

public:
	Array<TalkInfo>talks;
	size_t index = 0;
	TalkWindow() {
		AudioAsset::Register(U"TalkAudio", GMInstrument::BassAndLead, PianoKey::C6, 0.05s, 0.3);
		FontAsset::Register(U"WindowFont", 30);
	}

	//会話を追加：String 名前,String 会話文
	void addTalk(const String& name, const String& text) {
		talks << TalkInfo{ name,text };
	}

	//更新：bool 次に進むか,bool 音量
	void update(bool nextFlg, double volume = 1.0) {

		//確認
		if (not isContinue())return;

		accumlater += Scene::DeltaTime() / 0.1;

		const size_t length = static_cast<size_t>(accumlater);
		const String& text = talks[index].text;

		if (length != lastLength && not nextFlg && not(text.size() < length || text[length] == U'\n'))AudioAsset{ U"TalkAudio" }.playOneShot(volume);

		lastLength = length;

		if (nextFlg) {
			if (text.size() < length) {
				if (isContinue()) {
					next();
					lastLength = 0;
				}
			}
			else {
				accumlater = (double)text.size();
			}
		}
	}

	//描画：RectF 表示する位置を指定,SizeF 名前ウィンドウのサイズ
	void draw(const RectF& rect, const SizeF& nameSize = { 250,50 })const {

		constexpr double thickness = 5;

		const RoundRect window{ rect,20 };
		const RoundRect nameWindow{ rect.pos - Vec2::Down(nameSize.y - thickness),nameSize ,20 };
		window.draw({ Palette::Black, 0.8 }).drawFrame(thickness, 0);
		nameWindow.draw({ Palette::Black ,0.8 }).drawFrame(thickness, 0);

		//確認
		if (not isContinue())return;

		const TalkInfo& talk = talks[index];

		const size_t length = static_cast<size_t>(accumlater);
		FontAsset(U"WindowFont")(talk.text.substr(0, length)).draw(window.rect.stretched(-20));
		FontAsset(U"WindowFont")(talk.name).drawAt(nameWindow.rect.center());

		if (talk.text.size() < length) {
			Triangle{ window.rect.br().movedBy(-40, -40), 20, 180_deg }.draw(ColorF{ 1, Periodic::Sine0_1(2.0s) });
		}

	}

	//次に進む
	void next() {
		accumlater = 0;
		index += 1;
	}

	//表示されている名前
	const String& getName()const {
		return talks[index].name;
	}

	//会話中のテキスト
	const String& getText()const {
		return talks[index].text;
	}

	//継続中かどうか(全ての会話が終了したらfalseを返す)
	bool isContinue()const {
		return index < talks.size();
	}
};
