// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/plugin/Plugin.h"
#include "game_plugin/action/CheerAction.h"
#include "game_plugin/action/ResetBMAAction.h"
#include "game_plugin/command/ResetBMACommand.h"
#include "game_plugin/component/BMAComponent.h"
#include "game_plugin/event/BMAFireEvent.h"
#include "game_plugin/event/BMAFireEventFactory.h"
#include "game_plugin/event/BMAFalseAlarmEvent.h"
#include "game_plugin/event/BMAFalseAlarmEventFactory.h"
#include "game_plugin/command/TriggerCommand.h"
#include "game_plugin/command/InvestigateCommand.h"
#include "game_plugin/action/InvestigateBMAAction.h"

#include <em5/plugin/version/PluginVersion.h>
#include <em5/reflection/CampDefines.h>

#include <qsf_game/command/CommandManager.h>		// This is needed for commands; without this include, the build will work, but creation of commands fail

#include <qsf/map/Entity.h>							// Needed to avoid "error C2664: 'user::IndicatorComponent::IndicatorComponent(const user::IndicatorComponent &)' : cannot convert argument 1 from 'qsf::Entity *' to 'qsf::Prototype *' (game_plugin\plugin\Plugin.cpp)"
#include <qsf/localization/LocalizationSystem.h>	// Defines the "QT_TR_NOOP()"-macro
#include <qsf/prototype/helper/SlotHelper.h>
#include <qsf/plugin/QsfAssetTypes.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace flo11
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	Plugin::Plugin() :
		qsf::Plugin(new em5::PluginVersion())
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Plugin methods                 ]
	//[-------------------------------------------------------]
	bool Plugin::onInstall()
	{
		try
		{
			// Declare CAMP reflection system classes
			// -> Use Qt's "QT_TR_NOOP()"-macro in order to enable Qt's "lupdate"-program to find the internationalization texts

			// Declare CAMP reflection system classes
			QSF_START_CAMP_CLASS_EXPORT(flo11::BMAComponent, "BMAComponent", "ID_USER_COMPONENT_INDICATOR_DESCRIPTION")
				QSF_CAMP_IS_COMPONENT
				QSF_ADD_CAMP_PROPERTY(Name, BMAComponent::getName, BMAComponent::setName, "The BMA name", "")
				QSF_ADD_CAMP_ARRAY_PROPERTY(AlarmLights, flo11::BMAComponent::mAlarmLights, "The AlarmLight Slot").tag("Slot", qsf::SlotHelper::SOFT_LINK_TYPE)
				QSF_ADD_CAMP_PROPERTY(AlarmSound, BMAComponent::getAlarmSound, BMAComponent::setAlarmSound, "The AlarmSound Filename", "").tag("AssetType", qsf::QsfAssetTypes::AUDIO.getName())
				QSF_ADD_CAMP_PROPERTY(TargetEntity, BMAComponent::getTargetId, BMAComponent::setTargetId, "The Target to monitor", 0).tag("Slot", qsf::SlotHelper::SOFT_LINK_TYPE)
				QSF_ADD_CAMP_PROPERTY(CanTrigger, BMAComponent::isCanTrigger, BMAComponent::setCanTrigger, "The BMA can trigger an event", true)
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(flo11::ResetBMACommand, "ResetBMACommand", "Reset an active BMA")
				QSF_GAME_CAMP_IS_COMMAND
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(flo11::TriggerCommand, "TriggerCommand", "Triggers a BMA event for debugging")
				QSF_GAME_CAMP_IS_COMMAND
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(flo11::InvestigateCommand, "InvestigateCommand", "Investigate a bma target entity")
				QSF_GAME_CAMP_IS_COMMAND
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(flo11::ResetBMAAction, "ResetBMAAction", "Reset BMA action")
				QSF_CAMP_IS_ACTION
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(flo11::InvestigateBMAAction, "InvestigateBMAAction", "Investigate BMA action")
				QSF_CAMP_IS_ACTION
			QSF_END_CAMP_CLASS_EXPORT

			//[-------------------------------------------------------]
			// Event                                                  ]
			//[-------------------------------------------------------]
			QSF_START_CAMP_CLASS_EXPORT(flo11::BMAFireEventFactory, "BMAFireEventFactory", "Factory for BMAFire Events")
				EM5_CAMP_IS_FREEPLAY_EVENT_FACTORY
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(flo11::BMAFireEvent, "BMAFireEvent", "A BMA fire event")
				EM5_CAMP_IS_FREEPLAY_EVENT
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(flo11::BMAFalseAlarmEventFactory, "BMAFalseAlarmEventFactory", "Factory for BMAFalseAlarm Events")
				EM5_CAMP_IS_FREEPLAY_EVENT_FACTORY
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(flo11::BMAFalseAlarmEvent, "BMAFalseAlarmEvent", "A BMA false alarm event")
				EM5_CAMP_IS_FREEPLAY_EVENT
			QSF_END_CAMP_CLASS_EXPORT

			// Done
			return true;
		}
		catch (const std::exception& e)
		{
			// Error!
			QSF_ERROR("Failed to install the plugin '" << getName() << "'. Exception caught: " << e.what(), QSF_REACT_NONE);
			return false;
		}
	}

	bool Plugin::onStartup()
	{
		// Use this as entry point for all modding activities
		// Feel free to add more system relevant managers or systems here
		// It is also possible to add input handler to create a unique input behavior

		//mGameListener = new GameListener();
		//mGameListener->init();

		// Done
		return true;
	}

	void Plugin::onShutdown()
	{
		// Use this as to shutdown all modding relevant classes
		//QSF_SAFE_DELETE(mGameListener);
	}

	void Plugin::onUninstall()
	{
		// Removing classes is not possible within the CAMP reflection system

		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
