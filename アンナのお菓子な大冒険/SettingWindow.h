#pragma once
#include"Common.h"
#include"SoundIcon.hpp"

std::shared_ptr<UIElement> SettingWindow(const InputGroup& upInputGroup, const InputGroup& downInputGroup, const InputGroup& leftInputGroup, const InputGroup& rightInputGroup, const std::function<void()>& onClose = nullptr);
