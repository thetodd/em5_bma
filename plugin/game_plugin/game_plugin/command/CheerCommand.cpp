// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/command/CheerCommand.h"

#include <em5/action/ActionPriority.h>
#include <em5/map/EntityHelper.h>

#include <qsf_game/command/CommandContext.h>
#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/localization/LocalizationSystem.h>
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include <qsf_game\component\base\BoneLinkComponent.h>
#include <qsf_game\component\base\BoneToBoneLinkComponent.h>
#include <qsf/link/link/prototype/BoneToBoneLink.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/reflection/type/CampGlmQuat.h>
#include <qsf/component/link/LinkComponent.h>
#include <em5/action/base/PlayAnimationAction.h>

#include <qsf/component/link/LinkComponent.h>
#include <em5/component/vehicle/parts/WaterCannonComponent.h>
#include "em5/component/person/PersonComponent.h"
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
	const uint32 CheerCommand::PLUGINABLE_ID = qsf::StringHash("flo11::CheerCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CheerCommand::ACTION_PRIORITY = em5::action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CheerCommand::CheerCommand(qsf::game::CommandManager* commandManager) :
		em5::Command(commandManager, PLUGINABLE_ID)
	{
		mIconSettings.mTooltip = QT_TR_NOOP("ID_CHEERS");
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_megaphone";
	}

	bool CheerCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		
		// Check if unit state is valid
		em5::EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadVehicleInValidState())
			return false;
			
		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool CheerCommand::checkAvailable()
	{
		// Command is always available
		return true;
	}

	bool CheerCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority ?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;
		
		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool CheerCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
			return false;
		return true;
	}

	void CheerCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		EM5_FREEPLAY.triggerEventByPath("BMAEvent_Definitions/BMAFireEvent");
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
