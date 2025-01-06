
#include "..\kmbox\kmbox.hpp"
#include "..\settings.h"

namespace aim {

	Vector3 predictLocation(Vector3 target, Vector3 target_velocity, float projectile_speed, float projectile_gravity_scale, float distance)
	{
		float horizontalTime = distance / projectile_speed;
		float verticalTime = distance / projectile_speed;

		target.x += target_velocity.x * horizontalTime;
		target.y += target_velocity.y * horizontalTime;
		target.z += target_velocity.z * verticalTime + abs(-980 * projectile_gravity_scale) * 0.5f * (verticalTime * verticalTime);

		return target;
	}

	bool isHit(Vector3 loc, double margin = 20) {
		if (mainCamera.LocationUnderReticle.x >= loc.x - margin && mainCamera.LocationUnderReticle.x <= loc.x + margin && mainCamera.LocationUnderReticle.y >= loc.y - margin && mainCamera.LocationUnderReticle.y <= loc.y + margin)
			return true;
		else
			return false;
	}

	bool isHit2D(Vector3 loc, double margin = 20) {
		if (settings::window::Width / 2 >= loc.x - margin && settings::window::Width / 2 <= loc.x + margin && settings::window::Height / 2 >= loc.y - margin && settings::window::Height / 2 <= loc.y + margin)
			return true;
		else
			return false;
	}

	double isClose(Vector3 loc2D) {

		const double maxDistance = std::sqrt(std::pow(settings::window::Width, 2) + std::pow(settings::window::Height, 2)) / 2.0;
		
		double distance = std::sqrt(std::pow(loc2D.x - settings::window::Width / 2, 2) + std::pow(loc2D.y - settings::window::Height / 2, 2));
		
		double closeness = 1.0f - (distance / maxDistance);
		
		closeness = std::clamp(closeness, 0.0, 1.0);
		
		return closeness;
	}

	constexpr float RadiansToDegrees(float radians) {
		return radians * (180.0f / M_PI);
	}

	Rotation targetRotation(const Vector3& currentPosition, const Vector3& targetPosition) {
		float directionX = targetPosition.x - currentPosition.x;
		float directionY = targetPosition.y - currentPosition.y;
		float directionZ = targetPosition.z - currentPosition.z;

		float yawRadians = std::atan2(directionY, directionX);
		float yawDegrees = RadiansToDegrees(yawRadians);

		float distanceXY = std::sqrt(directionX * directionX + directionY * directionY); // Horizontal distance
		float pitchRadians = std::atan2(directionZ, distanceXY);
		float pitchDegrees = RadiansToDegrees(pitchRadians);

		return { yawDegrees, pitchDegrees };
	}

	PlayerCache target{};
	bool Targetting = false;

	void updateAimbot()
	{
		if (!settings::runtime::hotKeys)
			return;
		if (!settings::kmbox::SerialKmbox && !settings::kmbox::NetKmbox && !settings::config::MoonlightAim)
			return;
		if (!settings::config::Aimbot) 
			return;

		// not in game / alive
		if (!point::Player || !point::PlayerState)
			return;

		if (mem.IsKeyDown(settings::config::AimKey)) {
			std::unordered_map<uintptr_t, PlayerCache> PlayerList = secondPlayerList;

			bool ValidTarget = true;

			if (target.PlayerState) {

				// check if player still exist
				auto it = secondPlayerList.find(target.PlayerState);
				if (it != secondPlayerList.end()) {
					// get lil bro
					target = it->second;

					// check if he's good
					double dist = std::sqrt(std::pow(target.Head2D.x - settings::window::Width / 2, 2) + std::pow(target.Head2D.y - settings::window::Height / 2, 2));

					bool IsVis = point::Seconds - target.last_render <= 0.06f;

					if (target.isDying || !IsVis || dist > settings::config::AimFov)
						ValidTarget = false;

				}
				else {
					ValidTarget = false;
				}

				Targetting = true;
			}

			if (Targetting && !ValidTarget)
				return;

			double closest = HUGE;
			PlayerCache closestPlayer{};
			if (!Targetting) {
				bool closePlayerFound = false;
				for (auto it : PlayerList) {
					PlayerCache player = it.second;

					if (!player.Pawn || !player.Mesh || !player.BoneArray) continue;

					bool IsVis = point::Seconds - player.last_render <= 0.06f;

					if (player.isDying || !IsVis) continue;

					if (player.PlayerState == point::PlayerState) continue;

					if (player.TeamId == local_player::localTeam) continue;

					double distance2D = std::sqrt(std::pow(player.Head2D.x - settings::window::Width / 2, 2) + std::pow(player.Head2D.y - settings::window::Height / 2, 2));

					if (distance2D < settings::config::AimFov) {
						if (distance2D < closest) {
							closest = distance2D;
							closestPlayer = player;
							closePlayerFound = true;
						}
					}
				}

				if (!closePlayerFound)
					return;
			}
			else {
				closestPlayer = target;
			}

			target = closestPlayer;

			Vector3 originalTarget3D = closestPlayer.Head3D;
			Vector3 originalTarget2D = closestPlayer.Head2D;

			Vector3 target3D = originalTarget3D;
			Vector3 target2D = originalTarget2D;

			if (settings::config::Prediction) {
				if (point::ProjectileSpeed > 0) {
					target3D = predictLocation(target3D, closestPlayer.Velocity, point::ProjectileSpeed, point::ProjectileGravity, (float)mainCamera.Location.Distance(target3D));
					target2D = w2s(target3D);
				}
			}

			Rotation target = targetRotation(mainCamera.Location, target3D);

			float currentY = mainCamera.Rotation.y;
			float currentX = mainCamera.Rotation.x;

			float targety = target.yaw - currentY;
			float targetx = target.pitch - currentX;

			while (targety > 180.0f) targety -= 360.0f;
			while (targety < -180.0f) targety += 360.0f;

			std::clamp(targetx, -89.9f, 89.9f);

			float xPercent = point::MouseSensX / 0.25f;
			float xStep = 1.8f * (1.f / xPercent);
			
			float yPercent = point::MouseSensY / 0.25f;
			float yStep = -1.8f * (1.f / yPercent);

			float fovScale = 80.f / mainCamera.FieldOfView;

			xStep *= fovScale;
			yStep *= fovScale;

			float AngleX = targety * (xStep / settings::config::AimSmoothing); // settings::config::StepPerDegreeX
			float AngleY = targetx * (yStep / settings::config::AimSmoothing); // settings::config::StepPerDegreeY

			if (settings::config::MoonlightAim) {
				mouse_event(MOUSEEVENTF_MOVE, AngleX, AngleY, 0, 0);
			}
			else if (settings::kmbox::NetKmbox) {
				kmNet_mouse_move(AngleX, AngleY);
			}
			else {
				km_move(AngleX, AngleY);
			}
		}
		else {
			target.PlayerState = 0;
			Targetting = false;
		}
	}

	void updateTriggerBot()
	{
		if (!settings::runtime::hotKeys)
			return;

		if (!settings::kmbox::SerialKmbox && !settings::kmbox::NetKmbox && !settings::config::MoonlightAim)
			return;

		if (!settings::config::TriggerBot)
			return;

		static bool clicked = false;

		if (clicked) {
			if (settings::config::MoonlightAim)
				mouse_event(MOUSEEVENTF_LEFTUP, mainCamera.Rotation.x, mainCamera.Rotation.y, 0, 0);
			else
				kmNet_mouse_left(false);
			clicked = false;
		}

		static chrono::steady_clock::time_point lastClick = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastClick).count() < settings::config::TriggerDelay)
			return;

		if (mem.IsKeyDown(settings::config::TriggerKey)) {
			std::unordered_map<uintptr_t, PlayerCache> PlayerList = secondPlayerList;

			for (auto it : PlayerList) {
				PlayerCache player = it.second;

				if (!player.Pawn || !player.Mesh || !player.BoneArray) continue;

				bool IsVis = point::Seconds - player.last_render <= 0.06f;

				if (player.isDying || !IsVis) continue;

				if (player.PlayerState == point::PlayerState) continue;

				if (player.TeamId == local_player::localTeam) continue;

				if (isHit(player.Head3D)) {
					lastClick = std::chrono::steady_clock::now();

					if (settings::config::MoonlightAim) {
						mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
						clicked = true;
					}
					else if (settings::kmbox::NetKmbox) {
						kmNet_mouse_left(true);
						clicked = true;
					}
					else {
						km_click();
					}
				}

			}
		}

	}
}