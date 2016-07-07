#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/command/TriggerBMAEventCommand.h"

#include <em5/action/ActionPriority.h>
#include <em5/EM5Helper.h>
#include <em5/freeplay/FreeplaySystem.h>
#include <em5/map/EntityHelper.h>

#include <qsf_game/command/CommandContext.h>

namespace flo11
{
	const uint32 TriggerBMAEventCommand::PLUGINABLE_ID = qsf::StringHash("flo11::TriggerBMAEventCommand");
	const uint32 TriggerBMAEventCommand::ACTION_PRIORITY = em5::action::COMMAND_STD;

	TriggerBMAEventCommand::TriggerBMAEventCommand(qsf::game::CommandManager* commandManager) :
		em5::Command(commandManager, PLUGINABLE_ID)
	{
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mStartCommandMode = true; //No trigger by right-click
		mIconSettings.mButtonIconPath = "police_megaphone";

		mButtonPriority = 5;


	}

	bool TriggerBMAEventCommand::checkAvailable()
	{
		return true;
		// Command is always available

	}

	bool TriggerBMAEventCommand::checkCaller(qsf::Entity& caller)
	{
		em5::EntityHelper callerhelper(caller);
		//		callerhelper.
		if (!callerhelper.isSquadRoadVehicle() || !callerhelper.isSquadVehicleInValidState())
		{
			//Command only works with (squad and road) vehicles
			return false;
		}
		return true;
	}

	bool TriggerBMAEventCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (context.mAllowSelfExecution)
		{
			return false;
		}

		return false;
	}

	void TriggerBMAEventCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		QSF_LOG_PRINTS(INFO, "execute flo11:TriggerBMAEventCommand");
		//Trigger Event
		EM5_FREEPLAY.triggerEventByPath("BMAEvent_Definitions/BMAFireEvent");
	}

}
