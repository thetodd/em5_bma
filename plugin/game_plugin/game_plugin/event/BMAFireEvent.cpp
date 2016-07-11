// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/event/BMAFireEvent.h"
#include "game_plugin/component/BMAComponent.h"

#include <em5/event/EventHelper.h>
#include <em5/component/event/EventIdComponent.h>
#include <em5/freeplay/objective/ObjectiveHelper.h>
#include <em5/logic/HintHelper.h>
#include <em5/plugin/Messages.h>
#include <em5/fire/component/ComplexFireComponent.h>
#include <em5/component/objects/ObjectDamageComponent.h>
#include <em5/component/building/BuildingComponent.h>

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
	const uint32 BMAFireEvent::FREEPLAY_EVENT_ID = qsf::StringHash("flo11::BMAFireEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BMAFireEvent::BMAFireEvent()
	{
		// Nothing here
	}

	BMAFireEvent::~BMAFireEvent()
	{
		// Nothing here
	}

	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool BMAFireEvent::onStartup()
	{
		BMAComponent* bma = mTargetBMA->getComponent<flo11::BMAComponent>();
		//No nullptr check, EventFactory did that already
		
		std::string eventName = QSF_TRANSLATE_CONTEXTSTRING("flo11::BMAFireEvent", "ID_BMA_FIRE_EVENT_NAME");
		eventName.append(": ");
		eventName.append(bma->getName());

		this->setEventName(eventName);
		this->setNormalPointGain(500);


		bma->detectFire(); //BMA AUslösen

		//set Building on fire
		qsf::Entity* targetEntity = QSF_MAINMAP.getEntityById(bma->getTargetId());
		if (targetEntity != nullptr) {
			// Mark the entity so it wont get effected by other events
			em5::EventIdComponent& eventIdComponent = targetEntity->getOrCreateComponentSafe<em5::EventIdComponent>();
			eventIdComponent.setEvent(*this, em5::eventspreadreason::NO_REASON);

			em5::ComplexFireComponent* fireComp = targetEntity->getComponent<em5::ComplexFireComponent>();
			em5::FireComponent* sFireComp = targetEntity->getComponent<em5::FireComponent>();
			if (fireComp != nullptr) {
				fireComp->startFire(this);
				fireComp->setActive(true);
			}
			else if (sFireComp != nullptr) {
				//has simple EM5Fire component
				QSF_LOG_PRINTS(INFO, "Start simple fire");
				sFireComp->startFire(this);
				sFireComp->setActive(true);
			}
		}

		em5::ObjectiveHelper objectiveHelper(*this);
		em5::Objective* objective = objectiveHelper.getObjectiveByTypeId(em5::ObjectiveHelper::OBJECTIVE_NEED_EXTINGUISHFIRES);
		//getObjectives().insertObjective()
		objective->increaseNeededNumber(targetEntity);


		mMessageProxy.registerAt(qsf::MessageConfiguration("flo11::BMAResetActionFinished"), boost::bind(&BMAFireEvent::onResetBMAFinished, this, _1));
		mTargetBurningMessageProxy.registerAt(qsf::MessageConfiguration(em5::Messages::EM5_OBJECT_STOP_BURNING, bma->getTargetId()), boost::bind(&BMAFireEvent::onTargetStopBurning, this, _1));

		setRunning();
		// Done
		return true;
	}

	void BMAFireEvent::onShutdown()
	{
		//Reset damage level of target entity
		BMAComponent* bma = mTargetBMA->getComponent<flo11::BMAComponent>();
		qsf::Entity* targetEntity = QSF_MAINMAP.getEntityById(bma->getTargetId());
		em5::ObjectDamageComponent* damage = targetEntity->getComponent<em5::ObjectDamageComponent>();
		em5::BuildingComponent* building = targetEntity->getComponent<em5::BuildingComponent>();

		if (building != nullptr) {
			building->setDamageState(em5::DamageComponent::DamageState::DAMAGESTATE_INTACT);
		}

		if (damage != nullptr) {
			damage->setDamageState(em5::DamageComponent::DamageState::DAMAGESTATE_INTACT);
		}

		// Remove event ID component
		targetEntity->destroyComponent<em5::EventIdComponent>();
	}

	void BMAFireEvent::onRun()
	{		
	}

	bool BMAFireEvent::onFailure(EventResult& eventResult)
	{
		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void BMAFireEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool BMAFireEvent::checkEventConfiguration()
	{
		return true;
	}

	void BMAFireEvent::setTarget(qsf::Entity* target)
	{
		this->createEventLocationEntity(*target);
		this->mTargetBMA = target;
	}

	const qsf::Entity* BMAFireEvent::getFocusEntity()
	{
		return getEventLocationEntity();
	}

	void BMAFireEvent::onResetBMAFinished(const qsf::MessageParameters& parameters)
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

	void BMAFireEvent::onTargetStopBurning(const qsf::MessageParameters& parameters)
	{
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
