#pragma once

#include "game_plugin/Export.h"

#include <qsf/component/Component.h>
#include <qsf/audio/AudioProxy.h>

namespace flo11
{

	class BMAComponent : public qsf::Component
	{

	public:
		static const uint32 COMPONENT_ID;

	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		BMAComponent(qsf::Prototype* prototype);
		~BMAComponent();

		void detectFire();
		void reset();
		bool getActiveState();
		std::string getName();
		void setName(std::string radioName);
		uint64 getAlarmLightId();
		void setAlarmLightId(uint64 id);
		uint64 getTargetId();
		qsf::Entity* getTargetEntity();
		void setTargetId(uint64 id);
		bool thereAreFiresInside();
		bool isCanTrigger();
		void setCanTrigger(bool canTrigger);
		void setAlarmSound(std::string filePath);
		std::string getAlarmSound();

	protected:
		void setAlarmLights(bool state);

	private:
		bool active;
		std::string name;
		uint64 alarmLightEntityId;
		uint64 monitoringEntityId;
		bool mCanTrigger;
		qsf::AudioProxy alarmSound;
		std::string mAlarmSoundName;

		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	};

}

QSF_CAMP_TYPE_NONCOPYABLE(flo11::BMAComponent)
