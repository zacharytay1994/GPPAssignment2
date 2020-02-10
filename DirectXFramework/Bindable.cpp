#include "Bindable.h"


DxgiInfoManager& Bindable::GetInfoManager( Graphics& gfx ) noexcept(IS_DEBUG)
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error( "YouFuckedUp! (tried to access gfx.infoManager in Release config)" );
#endif
}
