#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/command/InvestigateCommand.h"
#include "game_plugin/component/BMAComponent.h"
#include "game_plugin/action/InvestigateBMAAction.h"

#include <em5/action/ActionPriority.h>
#include <em5/EM5Helper.h>
#include <em5/game/Game.h>
#include <em5/gui/EmergencyGui.h>
#include <em5/gui/IngameHud.h>
#include <em5/gui/hud/MessageWindow.h>
#include <em5/action/move/MoveAction.h>
#include <em5/action/move/TurnToAction.h>
#include <em5/game/targetpoint/OnTouchTargetPointProvider.h>

#include <qsf_game/command/CommandContext.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf/map/Entity.h>

namespace flo11
{
	const uint32 InvestigateCommand::PLUGINABLE_ID = qsf::StringHash("flo11::InvestigateCommand");
	const uint32 InvestigateCommand::ACTION_PRIORITY = 200;

	InvestigateCommand::InvestigateCommand(qsf::game::CommandManager* commandManager) :
		em5::Command(commandManager, PLUGINABLE_ID)
	{
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mStartCommandMode = false;

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "decontaminate_person";
	}

	bool InvestigateCommand::checkAvailable()
	{
		return true;
		// Command is always available

	}

	bool InvestigateCommand::checkCaller(qsf::Entity& caller)
	{
		return true;
	}

	bool InvestigateCommand::checkContext(const qsf::game::CommandContext& context)
	{
		BMAComponent* bma = context.mTargetEntity->getComponent<flo11::BMAComponent>();
		if (bma == nullptr) {
			return false;
		}

		if (bma->getActiveState() == false) {
			//can't check inactive BMA
			return false;
		}

		if (bma->isInvestigated()) {
			//BMA was already investigated
			return false;
		}

		return true;
	}

	void InvestigateCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan(); //alle anderen aktionen abbrechen.
		actionComponent.pushAction<em5::MoveAction>(ACTION_PRIORITY, qsf::action::APPEND_TO_BACK).
			init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, em5::OnTouchTargetPointProvider::TARGET_POINT_ID));

		actionComponent.pushAction<flo11::InvestigateBMAAction>(ACTION_PRIORITY, qsf::action::APPEND_TO_BACK).
			init(*context.mTargetEntity);
	}

}
