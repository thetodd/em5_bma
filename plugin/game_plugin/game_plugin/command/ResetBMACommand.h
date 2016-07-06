#pragma once

#include "em5/command/Command.h"
#include <qsf/map/Entity.h>	

namespace flo11
{
	/**
	*  @brief Let a vehicle move only on specific status ids.
	*  @author Florian 11-47-1
	*/
	class ResetBMACommand : public em5::Command
	{

	public:
		static const uint32 PLUGINABLE_ID;

	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit ResetBMACommand(qsf::game::CommandManager* commandManager);

	public:
		virtual bool checkAvailable() override;
		bool checkCaller(qsf::Entity& caller) override;
		virtual bool checkContext(const qsf::game::CommandContext& context) override;;
		virtual void execute(const qsf::game::CommandContext& context) override;

	protected:

	private:
		static const uint32 ACTION_PRIORITY;

		QSF_CAMP_RTTI()


	};

}

QSF_CAMP_TYPE_NONCOPYABLE(flo11::ResetBMACommand)
