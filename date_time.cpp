#include "date_time.h"

namespace fluid
{


long Time::timeZone = 0L;
long Time::adjustTime = 0L;

bool Time::setTimeZone()
{
	if(0L == timeZone)
	{
		tzset();
		timeZone = timezone;
		return true;
	}
	return false;
}

void Time::setAdjustTime(const int offset)
{
	adjustTime -= offset;
}

}
