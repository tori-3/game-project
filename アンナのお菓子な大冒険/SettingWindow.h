#pragma once

struct GameData;

std::shared_ptr<UIElement> SettingWindow(const InputGroup& upInputGroup, const InputGroup& downInputGroup, const InputGroup& leftInputGroup, const InputGroup& rightInputGroup, const std::function<void()>& onClose, GameData& gameData, UIManager& manager, size_t index = 0);
