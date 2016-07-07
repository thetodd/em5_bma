#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/command/TriggerBMAEventCommand.h"

#include <em5/action/ActionPriority.h>
#include <em5/EM5Helper.h>
#include <em5/freeplay/FreeplaySystem.h>

#include <qsf_game/command/CommandContext.h>

namespace flo11
{
	const uint32 TriggerBMAEventCommand::PLUGINABLE_ID = qsf::StringHash("flo11::TriggerBMAEventCommand");
	const uint32 TriggerBMAEventCommand::ACTION_PRIORITY = 200;

	TriggerBMAEventCommand::TriggerBMAEventCommand(qsf::game::CommandManager* commandManager) :
		em5::Command(commandManager, PLUGINABLE_ID)
	{
		mIconSettings.mShowAsGuiButton = true;

		// Set name of the mouse cursor
		mIconSettings.mButtonIconPath = "enable";
	}

	bool TriggerBMAEventCommand::checkAvailable()
	{
		return true;
		// Command is always available

	}

	bool TriggerBMAEventCommand::checkCaller(qsf::Entity& caller)
	{
		return true;
	}

	bool TriggerBMAEventCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return true;
	}

	void TriggerBMAEventCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		QSF_LOG_PRINTS(INFO, "execute flo11:TriggerBMAEventCommand");
		//Trigger Event
		EM5_FREEPLAY.triggerEventByPath("BMAEvent_Definitions/BMAFireEvent");
	}

}
