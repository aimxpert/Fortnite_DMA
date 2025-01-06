#pragma once

namespace esp {
	// idk :(
	bool shouldDisplayPlayer(PlayerCache player) {

		auto it = secondPlayerList.find(player.PlayerState);
		if (it != secondPlayerList.end()) {

			double dist = std::sqrt(std::pow(player.Head2D.x - settings::window::Width / 2, 2) + std::pow(player.Head2D.y - settings::window::Height / 2, 2));

			if (player.isDying || dist > settings::config::AimFov)
				return false;

		}
		else {
			return false;
		}

		return true;
	}

	void renderPlayers()
	{

		// stats
		int playersRendered = 0;
		int playersLooped = 0;
		int validPlayersLooped = 0;
		int invalidPlayersLooped = 0;
		int teammatesSkipped = 0;
		int bots = 0;

		//// repetitive
		//double closest = HUGE;
		//PlayerCache closestPlayer{};

		for (auto it : secondPlayerList) {
			PlayerCache player = it.second;

			if (player.ignore)
				continue;

			playersLooped++;

			// valid?
			if (!isPlayerValid(player)) {
				invalidPlayersLooped++;
				continue;
			}

			validPlayersLooped++;

			// should be in the updates
			bool IsVis = point::Seconds - player.last_render <= 0.06f;

			// player.isDBNO

			if (player.isBot) {
				bots++;
			}

			// check if its me ༼ つ ◕_◕ ༽つ
			if (player.PlayerState == point::PlayerState) {
				//ImGui::GetBackgroundDrawList()->AddText((ImFont*)fonts::notosans_font, 18.5f, ImVec2(player.Neck2D.x, player.Neck2D.y), ImColor(255, 255, 255, 255), "me");
				continue;
			}

			// chcek if not in lobby and check if its a teammate (omg friends) (◕‿◕✿)
			if (point::Player && player.TeamId == local_player::localTeam) {
				//ImGui::GetBackgroundDrawList()->AddText((ImFont*)fonts::notosans_font, 18.5f ,ImVec2(player.Neck2D.x, player.Neck2D.y), ImColor(255,255,255,255), "teammate");
				teammatesSkipped++;
				continue;
			}

			// check if dying (dead) ◉_◉
			if (player.bisDying) {
				//ImGui::GetBackgroundDrawList()->AddText((ImFont*)fonts::notosans_font, 18.5f, ImVec2(player.Neck2D.x, player.Neck2D.y), ImColor(255, 255, 255, 255), "dying");
				continue;
			}

			// info we might want to show

			int distanceMeters = mainCamera.Location.Distance(player.Head3D) / 100;

			// ok maybe we can draw now

			playersRendered++;

			// skeleton
			if (settings::config::Skeleton)
			{
				// color
				ImColor colSK = ImColor(255,0,0,255);
				if (IsVis)
					colSK = ImColor(0, 255, 0, 255);

				//thickness
				float tk = 0.8f;

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.Neck2D.x, player.Neck2D.y), ImVec2(player.Head2D.x, player.Head2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.Hip2D.x, player.Hip2D.y), ImVec2(player.Neck2D.x, player.Neck2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.UpperArmLeft2D.x, player.UpperArmLeft2D.y), ImVec2(player.Neck2D.x, player.Neck2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.UpperArmRight2D.x, player.UpperArmRight2D.y), ImVec2(player.Neck2D.x, player.Neck2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.LeftHand2D.x, player.LeftHand2D.y), ImVec2(player.UpperArmLeft2D.x, player.UpperArmLeft2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.RightHand2D.x, player.RightHand2D.y), ImVec2(player.UpperArmRight2D.x, player.UpperArmRight2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.LeftHand2D.x, player.LeftHand2D.y), ImVec2(player.LeftHandT2D.x, player.LeftHandT2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.RightHand2D.x, player.RightHand2D.y), ImVec2(player.RightHandT2D.x, player.RightHandT2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.LeftThigh2D.x, player.LeftThigh2D.y), ImVec2(player.Hip2D.x, player.Hip2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.RightThigh2D.x, player.RightThigh2D.y), ImVec2(player.Hip2D.x, player.Hip2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.LeftCalf2D.x, player.LeftCalf2D.y), ImVec2(player.LeftThigh2D.x, player.LeftThigh2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.RightCalf2D.x, player.RightCalf2D.y), ImVec2(player.RightThigh2D.x, player.RightThigh2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.LeftFoot2D.x, player.LeftFoot2D.y), ImVec2(player.LeftCalf2D.x, player.LeftCalf2D.y), colSK, tk);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(player.RightFoot2D.x, player.RightFoot2D.y), ImVec2(player.RightCalf2D.x, player.RightCalf2D.y), colSK, tk);
			}

			// box
			if (settings::config::Box)
			{
				ImColor colBox = ImColor(255, 0, 0, 255);
				if (IsVis)
					colBox = ImColor(0, 255, 0, 255);

				float tk = 2.f;
				float rd = 10.f;

				float box_height = (abs(player.Top2D.y - player.Bottom2D.y));
				float box_width = 0.5f * box_height;

				ImVec2 topLeft = ImVec2(player.Bottom2D.x - box_width / 2, player.Top2D.y);
				ImVec2 bottomRight = ImVec2(player.Bottom2D.x + box_width / 2, player.Bottom2D.y);

				//const char* nameStr = player.Name;
				//ImVec2 nameSize = ImGui::CalcTextSize(nameStr);
				//ImVec2 namePos = ImVec2(
				//	topLeft.x + (width / 2.0f) - (nameSize.x / 2.0f),
				//	topLeft.y - nameSize.y - 5.0f
				//);

				ImGui::GetBackgroundDrawList()->AddRect(topLeft, bottomRight, colBox, rd, NULL, tk);
			}

			//distance
			if (settings::config::Distance)
			{
				ImColor colTxt = ImColor(255, 255, 255, 255);

				float box_height = (abs(player.Top2D.y - player.Bottom2D.y));
				float box_width = 0.5f * box_height;

				ImVec2 topLeft = ImVec2(player.Bottom2D.x - box_width / 2, player.Top2D.y);
				ImVec2 bottomRight = ImVec2(player.Bottom2D.x + box_width / 2, player.Bottom2D.y);

				string distanceStr = std::format("({:d}m)", distanceMeters);
				ImVec2 distanceSize = ImGui::CalcTextSize(distanceStr.c_str());
				ImVec2 distancePos = ImVec2(
					topLeft.x + (box_width / 2.0f) - (distanceSize.x / 2.0f),
					bottomRight.y + 5.0f
				);

				ImGui::GetBackgroundDrawList()->AddText(distancePos, colTxt, distanceStr.c_str());
			}

			//double distance2D = std::sqrt(std::pow(player.Head2D.x - settings::window::Width / 2, 2) + std::pow(player.Head2D.y - settings::window::Height / 2, 2));

			//if (distance2D < settings::config::AimFov) {
			//	if (distance2D < closest) {
			//		closestPlayer = player;
			//		closest = distance2D;
			//	}
			//}
		}

		if (settings::config::Aimbot && settings::config::Prediction && aim::Targetting) {
			if (point::ProjectileSpeed > 0 && shouldDisplayPlayer(aim::target)) {
				Vector3 pred = w2s(aim::predictLocation(aim::target.Head3D, aim::target.Velocity, point::ProjectileSpeed, point::ProjectileGravity, (float)mainCamera.Location.Distance(aim::target.Head3D)));
				ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(pred.x, pred.y), 5, ImColor(255, 255, 255, 255), 20);
			}
		}

		// stats
		info::render::playersRendered = playersRendered;
		info::render::playersLooped = playersLooped;
		info::render::validPlayersLooped = validPlayersLooped;
		info::render::invalidPlayersLooped = invalidPlayersLooped;
		info::render::teammatesSkipped = teammatesSkipped;
		info::render::validBots = bots;
	}

	void Debug() 
	{

		const auto& MainValues = stats::mainThreadData.getValues();
		float mainAvgMs = std::accumulate(MainValues.begin(), MainValues.end(), 0.0f) / MainValues.size();
		float mainFPS = (mainAvgMs > 0) ? (1000.0f / mainAvgMs) : 0.0f;
		string mainStr = std::format("FPS: {:.0f}", mainFPS);

		ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImColor(255, 255, 255, 255), mainStr.c_str());

	}
}