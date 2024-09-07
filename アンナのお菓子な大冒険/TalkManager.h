#pragma once
#include"TalkWindow.h"

class TalkManager {
public:
	TalkWindow talkWindow;




public:
	 TalkManager(const  TalkManager&) = delete; 
	 TalkManager& operator=(const  TalkManager&) = delete; 
	 TalkManager( TalkManager&&) = delete; 
	 TalkManager& operator=( TalkManager&&) = delete;

	 static TalkManager& get() {
		 static TalkManager instance;
		 return instance;
	 }

private:
	 TalkManager() = default; 
	~ TalkManager() = default; 
};
