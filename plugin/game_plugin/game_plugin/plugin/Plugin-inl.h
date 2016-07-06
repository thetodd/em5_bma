// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace flo11
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Plugin::~Plugin()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Plugin methods                    ]
	//[-------------------------------------------------------]
	inline const char* Plugin::getName() const
	{
		return "BMA Plugin";	// Please replace with a better name
	}

	inline const char* Plugin::getVendor() const
	{
		return "Florian 11-47-1";	// Replace with your own name as author
	}

	inline const char* Plugin::getDescription() const
	{
		return "Adds BMA commands and event.";	// Insert a plugin description here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
