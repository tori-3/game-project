#pragma once

inline double linerMove(double pos, double target, double speed, double dt = Scene::DeltaTime()) {

	double d = speed * dt;

	if (pos < target) {

		if (target < pos + d) {
			return target;
		}
		else {
			return pos + d;
		}
	}
	else {

		if (pos - d < target) {
			return target;
		}
		else {
			return pos - d;
		}
	}
}
