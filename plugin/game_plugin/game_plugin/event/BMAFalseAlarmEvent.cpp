// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/event/BMAFalseAlarmEvent.h"
#include "game_plugin/component/BMAComponent.h"

#include <em5/event/EventHelper.h>
#include <em5/component/event/EventIdComponent.h>
#include <em5/freeplay/objective/ObjectiveHelper.h>
#include <em5/logic/HintHelper.h>
#include <em5/plugin/Messages.h>
#include <em5/fire/component/ComplexFireComponent.h>
#include <em5/component/objects/ObjectDamageComponent.h>
#include <em5/component/building/BuildingComponent.h>
#include <em5/gui/EmergencyGui.h>
#include <em5/gui/IngameHud.h>
#include <em5/gui/hud/MessageWindow.h>
#include <em5/EM5Helper.h>

#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>
#include <qsf/Qsf.h>
#include <qsf/map/MapSystem.h>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace flo11
{

	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BMAFalseAlarmEvent::FREEPLAY_EVENT_ID = qsf::StringHash("flo11::BMAFalseAlarmEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BMAFalseAlarmEvent::BMAFalseAlarmEvent()
	{
		// Nothing here
	}

	BMAFalseAlarmEvent::~BMAFalseAlarmEvent()
	{
		// Nothing here
	}

	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool BMAFalseAlarmEvent::onStartup()
	{
		BMAComponent* bma = mTargetBMA->getComponent<flo11::BMAComponent>();
		//No nullptr check, EventFactory did that already
		
		std::string eventName = QSF_TRANSLATE_CONTEXTSTRING("flo11::BMAFireEvent", "ID_BMA_FIRE_EVENT_NAME");
		eventName.append(": ");
		eventName.append(bma->getName());

		this->setEventName(eventName);
		this->setNormalPointGain(500);


		bma->detectFire(); //BMA AUslösen

		mInvestigatingMessageProxy.registerAt(qsf::MessageConfiguration("flo11:InvestigationFinished", bma->getId()), boost::bind(&BMAFalseAlarmEvent::onInvestigationFinished, this, _1));
		mResetBMAMessageProxy.registerAt(qsf::MessageConfiguration("flo11:BMAResetActionFinished"), boost::bind(&BMAFalseAlarmEvent::onResetBMAFinished, this, _1));

		setRunning();
		// Done
		return true;
	}

	void BMAFalseAlarmEvent::onShutdown()
	{
		//Reset damage level of target entity
		BMAComponent* bma = mTargetBMA->getComponent<flo11::BMAComponent>();
		qsf::Entity* targetEntity = QSF_MAINMAP.getEntityById(bma->getTargetId());

		// Remove event ID component
		targetEntity->destroyComponent<em5::EventIdComponent>();
	}

	void BMAFalseAlarmEvent::onRun()
	{		
	}

	bool BMAFalseAlarmEvent::onFailure(EventResult& eventResult)
	{
		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void BMAFalseAlarmEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool BMAFalseAlarmEvent::checkEventConfiguration()
	{
		return true;
	}

	void BMAFalseAlarmEvent::setTarget(qsf::Entity* target)
	{
		this->createEventLocationEntity(*target);
		this->mTargetBMA = target;
	}

	const qsf::Entity* BMAFalseAlarmEvent::getFocusEntity()
	{
		return getEventLocationEntity();
	}

	void BMAFalseAlarmEvent::onResetBMAFinished(const qsf::MessageParameters& parameters)
	{
		//QSF_LOG_PRINTS(INFO, "ResetBMAFinished");
		uint64 entityId;
		parameters.getParameter<uint64>("EntityId", entityId);
		//QSF_LOG_PRINTF(INFO, "Entites: %d and %d", entityId, this->mTargetBMA->getId());
		if (entityId == this->mTargetBMA->getId()) {
			//BMA was reset
			em5::Objective* objective = getObjectives().getObjectiveByTypeId(qsf::StringHash("ResetBMA"));
			if (nullptr != objective) {
				objective->setAccomplished();
			}
		}
	}

	void BMAFalseAlarmEvent::onInvestigationFinished(const qsf::MessageParameters& parameters)
	{
		EM5_GUI.getIngameHud().getMessageWindow()->addTextMessage(QT_TR_NOOP("ID_FIRE_IN_BUILDING"), qsf::Color3::ORANGE);
		uint64 entityId;
		parameters.getParameter<uint64>("EntityId", entityId);

		//Only add objective once
		if (getObjectives().getObjectiveByTypeId(qsf::StringHash("ResetBMA")) == nullptr) {
			//Add new Objective for reset BMA
			em5::Objective& resetObjective = getObjectives().getOrCreateObjective(qsf::StringHash("ResetBMA"),
				em5::Objective::OBJECTIVETYPE_REQUIRED, getId());
			resetObjective.setText(QT_TR_NOOP("ID_RESET_BMA_OBJECTIVE_TEXT"));
			resetObjective.setNeededNumber(1);
		}
	}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
