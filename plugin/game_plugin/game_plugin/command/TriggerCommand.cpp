// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/command/TriggerCommand.h"

#include <em5/action/ActionPriority.h>
#include <em5/map/EntityHelper.h>

#include <qsf_game/command/CommandContext.h>
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include <em5/freeplay/FreeplaySystem.h>
#include <em5/EM5Helper.h>
//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace flo11
{
	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TriggerCommand::PLUGINABLE_ID = qsf::StringHash("flo11::TriggerCommand");

	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 TriggerCommand::ACTION_PRIORITY = em5::action::COMMAND_STD;

	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TriggerCommand::TriggerCommand(qsf::game::CommandManager* commandManager) :
		em5::Command(commandManager, PLUGINABLE_ID)
	{
		mIconSettings.mTooltip = QT_TR_NOOP("ID_CMD_TRIGGER_BMA_EVENT");
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_megaphone";
	}

	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool TriggerCommand::checkAvailable()
	{
		// Command is always available
		return true;
	}

	bool TriggerCommand::checkCaller(qsf::Entity& caller)
	{
		return true;
	}

	bool TriggerCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution) {
			return false;
		}

		//Don't execute on caller with right click
		if (context.mTargetEntity == context.mCaller) {
			return false;
		}

		return true;
	}

	void TriggerCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		//qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		EM5_FREEPLAY.triggerEventByPath("BMAEvent_Definitions/BMAFireEvent");
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
