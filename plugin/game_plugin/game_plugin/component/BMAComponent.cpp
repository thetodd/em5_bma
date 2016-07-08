#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/component/BMAComponent.h"

#include <em5/command/component/CommandableComponent.h>
#include <em5/fire/component/ComplexFireComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/Qsf.h>
#include <qsf/map/Map.h>
#include <qsf/map/MapSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/EntityHelper.h>
#include <qsf/renderer/light/LightAnimationComponent.h>
#include <qsf/audio/AudioManager.h>

namespace flo11
{
	const uint32 BMAComponent::COMPONENT_ID = qsf::StringHash("flo11::BMAComponent");

	BMAComponent::BMAComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		active(false),
		name(""),
		mCanTrigger(true)
	{
		
	}

	BMAComponent::~BMAComponent()
	{
		// Nothing to do in here
	}

	bool BMAComponent::getActiveState()
	{
		return active;
	}

	void BMAComponent::detectFire()
	{
		this->active = true;
		//activate AlarmLight
		this->setAlarmLights(true);

		//play sound
		if (this->getAlarmSound() != "") {
			qsf::EntityHelper entityHelper(*this->getTargetEntity());
			alarmSound.sourceFile(this->getAlarmSound()).spatial().position(entityHelper.getPosition())
				//.referenceDistance(20.0f).maximumDistance(150.0f)
				.channel(qsf::AudioManager::MIXERCHANNEL_SFX).loop().play();
		}
	}

	void BMAComponent::reset()
	{
		this->active = false;
		//deactivate AlarmLight
		this->setAlarmLights(false);
		alarmSound.stop();
	}

	void BMAComponent::setName(std::string name)
	{
		this->name = name;
	}

	std::string BMAComponent::getName()
	{
		return this->name;
	}

	uint64 BMAComponent::getAlarmLightId()
	{
		return this->alarmLightEntityId;
	}

	void BMAComponent::setAlarmLightId(uint64 id)
	{
		this->alarmLightEntityId = id;
		this->setAlarmLights(false);
	}

	uint64 BMAComponent::getTargetId()
	{
		return this->monitoringEntityId;
	}

	void BMAComponent::setTargetId(uint64 id)
	{
		this->monitoringEntityId = id;
	}

	void BMAComponent::setAlarmLights(bool state)
	{
		qsf::Entity* alarmLight = QSF_MAINMAP.getEntityById(this->alarmLightEntityId);
		if (alarmLight != nullptr) {
			qsf::LightAnimationComponent* light = alarmLight->getComponent<qsf::LightAnimationComponent>();
			if (light != nullptr) {
				light->setActive(state);
			}
		}
	}

	bool BMAComponent::thereAreFiresInside()
	{
		qsf::Entity* targetEntity = QSF_MAINMAP.getEntityById(this->getTargetId());
		if (targetEntity != nullptr) {
			em5::ComplexFireComponent* fireComp = targetEntity->getComponent<em5::ComplexFireComponent>();
			if (fireComp != nullptr) {
				return fireComp->isBurning();
			}
		}

		return false;
	}

	bool BMAComponent::isCanTrigger()
	{
		return this->mCanTrigger;
	}

	void BMAComponent::setCanTrigger(bool canTrigger)
	{
		this->mCanTrigger = canTrigger;
	}

	qsf::Entity* BMAComponent::getTargetEntity()
	{
		qsf::Entity* targetEntity = QSF_MAINMAP.getEntityById(this->getTargetId());
		return targetEntity;
	}

	void BMAComponent::setAlarmSound(std::string filePath)
	{
		this->mAlarmSoundName = filePath;
	}

	std::string BMAComponent::getAlarmSound()
	{
		return this->mAlarmSoundName;
	}

}
