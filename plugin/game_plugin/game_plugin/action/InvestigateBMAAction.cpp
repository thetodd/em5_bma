// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/action/InvestigateBMAAction.h"
#include "game_plugin/component/BMAComponent.h"

#include <em5/action/move/TurnToAction.h>
#include <em5/action/ActionPriority.h>
#include <em5/command/CommandContext.h>
#include <em5/game/animation/AnimationHelper.h>
#include <em5/map/EntityHelper.h>

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/renderer/animation/MeshAnimationChannel.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace flo11
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier InvestigateBMAAction::ACTION_ID = "flo11::InvestigateBMAAction";


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]

	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	InvestigateBMAAction::InvestigateBMAAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mAnimationChannel(nullptr)
	{
		// Nothing here
	}

	InvestigateBMAAction::~InvestigateBMAAction()
	{
		// Nothing here
	}

	void InvestigateBMAAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 InvestigateBMAAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void InvestigateBMAAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool InvestigateBMAAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		mTargetEntity = getMap().getEntityById(mTargetEntityId);
		if (!mTargetEntity.valid()) {
			return false;
		}
		
		BMAComponent* bma = mTargetEntity->getComponent<flo11::BMAComponent>();
		if (bma == nullptr) {
			return false;
		}

		return true;
	}

	void InvestigateBMAAction::onShutdown()
	{
		if (!mTargetEntity.valid()) {
			return; // Nothing to do here...
		}
	}

	qsf::action::Result InvestigateBMAAction::updateAction(const qsf::Clock&)
	{
		// Get the caller entity
		qsf::Entity& callerEntity = getEntity();

		if (!mTargetEntity.valid()) {
			return qsf::action::RESULT_DONE;
		}

		//Get BMAComponent
		BMAComponent* bma = mTargetEntity->getComponent<flo11::BMAComponent>();
		if (bma == nullptr || bma->getActiveState() == false) {
			return qsf::action::RESULT_DONE; //Nothing to do...
		}
		
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				getComponent().pushAction<em5::TurnToAction>(em5::action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mTargetEntityId);
				//Play working animation
				em5::AnimationHelper animationHelper(getEntity());
				mAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationEngineerRepairStandingStart());
				mCurrentState = STATE_START;

				return qsf::action::RESULT_CONTINUE;
			}
			case STATE_START:
			{
				if (nullptr != mAnimationChannel && mAnimationChannel->isFinished())
				{
					// Wait for finish the animation
					return qsf::action::RESULT_CONTINUE;
				}

				em5::AnimationHelper animationHelper(getEntity());
				mAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationEngineerRepairStandingEnd());

				mCurrentState = STATE_FINISH;

				return qsf::action::RESULT_CONTINUE;
			}
			case STATE_FINISH:
			{
				if (nullptr != mAnimationChannel && mAnimationChannel->isFinished())
				{
					// Wait for finish the animation
					return qsf::action::RESULT_CONTINUE;
				}

				//Fire Message
				qsf::MessageParameters parameters;
				parameters.setParameter("EntityId", getTargetEntityId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration("flo11::BMAInvestigationFinished", getTargetEntityId()), parameters);
				//EM5_GUI.getIngameHud().getMessageWindow()->addTextMessage("Hello World");

				return qsf::action::RESULT_DONE;
			}
		}
		return qsf::action::RESULT_DONE;
	}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
