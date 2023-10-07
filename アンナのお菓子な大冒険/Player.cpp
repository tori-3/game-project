#include"Entity.h"

void Player::update() {

	DataManager::get().stage.hit(&box);

	if (jump && box.touch(Direction::down))jump = false;

	double speed_rate = 1.0;
	if (KeySpace.down()) { if (box.touch(Direction::down)) { v.y = -700.0; jump = true; } }//ジャンプ
	if (KeyJ.pressed()) { speed_rate = 2.0; };//ダッシュ
	if (KeyA.pressed()) {
		if (not box.touch(Direction::left))v.x = -400 * speed_rate;
		left = true;
	}//左
	else if (KeyD.pressed()) {
		if (not box.touch(Direction::right))v.x = 400 * speed_rate;
		left = false;
	}//右

	if (KeyG.pressed()) {
		rushTimer.restart();
	}

	if (rushTimer.isRunning() && 0s < rushTimer) {
		if (left) {
			if (not box.touch(Direction::left))v.x = -1000;
		}
		else {
			if (not box.touch(Direction::right))v.x = 1000;
		}
	}


	box.physicsUpdate();
	box.update();

	if (box.touch(Direction::down) && box.touch(Direction::up) && box.touch(Direction::left) && box.touch(Direction::right)) {
		hp = 0;
	}

	figure = box.Get_Box();
}

void Player::draw()const {

	if (jump)box.draw(Palette::Blue);
	else box.draw(Palette::Red);

	figure.draw();

}
