// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Core/Debug.hpp>

namespace Nz
{
	/*!
	* \brief Reset the current bit cursor

	* \remark This function only reset the cursor position, it doesn't do any writing
	          if you wish to write all bits and reset bit position, call FlushBits

	  \see FlushBits
	*/
	inline void SerializationContext::ResetBitPosition()
	{
		currentBitPos = 8;
	}
}

#include <Nazara/Core/DebugOff.hpp>
