// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/event/BMAFalseAlarmEventFactory.h"
#include "game_plugin/event/BMAFalseAlarmEvent.h"
#include "game_plugin/component/BMAComponent.h"

#include <em5/component/event/EventIdComponent.h>
#include <em5/event/EventHelper.h>
#include <em5/map/EntityHelper.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/reflection/CampHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace flo11
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BMAFalseAlarmEventFactory::BMAFalseAlarmEventFactory()
	{
		// Nothing here
	}

	BMAFalseAlarmEventFactory::~BMAFalseAlarmEventFactory()
	{
		// Nothing here
	}

	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	em5::FreeplayEvent* BMAFalseAlarmEventFactory::tryTriggerEvent()
	{
		//Find target BMA
		qsf::Entity* targetEntity = findTargetEntity();
		if (targetEntity == nullptr) {
			return nullptr; //No BMA available
		}

		// Create the event instance
		BMAFalseAlarmEvent& freeplayEvent = createAndInitializeEvent<BMAFalseAlarmEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setTarget(targetEntity);

		// Done
		return &freeplayEvent;
	}

	qsf::Entity* BMAFalseAlarmEventFactory::findTargetEntity()
	{
		const qsf::ComponentCollection::ComponentList<flo11::BMAComponent>& bmaComponentsInMap = qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<flo11::BMAComponent>();
		if (bmaComponentsInMap.empty())
		{
			// No bmas there... early out
			return nullptr;
		}

		// Try to find a bma we can trigger a fire
		std::vector<flo11::BMAComponent*> openBMAComponents = bmaComponentsInMap.copyToVector();
		const size_t numberOfPersonsInMap = openBMAComponents.size();
		const size_t totalTries = 40;
		for (size_t tries = 0; tries < numberOfPersonsInMap; ++tries)
		{
			// Choose a random person component
			qsf::Entity& entity = qsf::Random::getPickOutAnyOf(openBMAComponents)->getEntity();

			// Check if the person is appropriate
			if (checkCandidate(entity))
			{
				return &entity;
			}

			if (tries > totalTries)
				return nullptr;
		}

		// None found
		return nullptr;
	}

	bool BMAFalseAlarmEventFactory::checkCandidate(qsf::Entity& entity)
	{
		flo11::BMAComponent* component = entity.getComponent<flo11::BMAComponent>();
		if (component == nullptr) {
			return false;
		}

		if (component->getActiveState() == true || component->isCanTrigger() == false) {
			return false;
		}

		const em5::EntityHelper entityHelper(*component->getTargetEntity());
		// Check if entity is a valid event target, i.e. is entity in another event
		if (!entityHelper.isEntityValidEventTarget()) {
			return false;
		}

		return true;
	}

	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
} // user
