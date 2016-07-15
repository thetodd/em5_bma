// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <em5/freeplay/factory/FreeplayEventFactory.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace flo11
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 freeplay event factory for "medical emergency"
	*/
	class BMAFalseAlarmEventFactory : public em5::FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BMAFalseAlarmEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BMAFalseAlarmEventFactory();

	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual em5::FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity* findTargetEntity();
		bool checkCandidate(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user

//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(flo11::BMAFalseAlarmEventFactory)
