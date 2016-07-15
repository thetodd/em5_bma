#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/command/ResetBMACommand.h"
#include "game_plugin/component/BMAComponent.h"
#include "game_plugin/action/ResetBMAAction.h"

#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include <em5/game/targetpoint/OnTouchTargetPointProvider.h>

#include <qsf_game/command/CommandContext.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf/map/Entity.h>

namespace flo11
{
	const uint32 ResetBMACommand::PLUGINABLE_ID = qsf::StringHash("flo11::ResetBMACommand");
	const uint32 ResetBMACommand::ACTION_PRIORITY = 200;

	ResetBMACommand::ResetBMACommand(qsf::game::CommandManager* commandManager) :
		em5::Command(commandManager, PLUGINABLE_ID)
	{
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mStartCommandMode = false;

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "repair_vehicle";
	}

	bool ResetBMACommand::checkAvailable()
	{
		return true;
		// Command is always available

	}

	bool ResetBMACommand::checkCaller(qsf::Entity& caller)
	{
		return true;
	}

	bool ResetBMACommand::checkContext(const qsf::game::CommandContext& context)
	{
		BMAComponent* bma = context.mTargetEntity->getComponent<flo11::BMAComponent>();
		if (bma == nullptr) {
			return false;
		}

		if (bma->getActiveState() == false) {
			//can't reset inactive bma
			return false;
		}
		else if (bma->thereAreFiresInside()) {
			return false;
		}

		return true;
	}

	void ResetBMACommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan(); //alle anderen aktionen abbrechen.
		actionComponent.pushAction<em5::MoveAction>(ACTION_PRIORITY, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, em5::OnTouchTargetPointProvider::TARGET_POINT_ID));
		QSF_LOG_PRINTS(INFO, "execute flo11:ResetBMACommand");
		actionComponent.pushAction<flo11::ResetBMAAction>(ACTION_PRIORITY, qsf::action::APPEND_TO_BACK).init(*context.mTargetEntity);

	}

}
