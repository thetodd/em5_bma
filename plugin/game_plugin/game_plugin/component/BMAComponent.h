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
		std::vector<uint64> mAlarmLights;

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
		uint64 getTargetId();
		qsf::Entity* getTargetEntity();
		void setTargetId(uint64 id);
		bool thereAreFiresInside();
		bool isCanTrigger();
		void setCanTrigger(bool canTrigger);
		void setAlarmSound(std::string filePath);
		std::string getAlarmSound();
		bool isInvestigated();
		void setInvestigatedState(bool state);

	protected:
		void setAlarmLights(bool state);

	private:
		bool active;
		bool mIsInvestigated;
		std::string name;
		uint64 monitoringEntityId;
		bool mCanTrigger;
		qsf::AudioProxy alarmSound;
		qsf::AssetProxy mAlarmSound;

		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	};

}

QSF_CAMP_TYPE_NONCOPYABLE(flo11::BMAComponent)
