#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES

#include "olcPixelGameEngine.h"
#include "atltime.h"
#include <cmath>


template <typename T_ty>
constexpr T_ty degToRad(T_ty angle)
{
	return angle * (M_PI / T_ty(180));
}


struct hhmmss {
	int hour;
	int minute;
	int second;
};


inline bool operator==(const hhmmss& lhs, const hhmmss& rhs) {
	return lhs.hour == rhs.hour &&
		lhs.minute == rhs.minute &&
		lhs.second == rhs.second;
}


using std::min;
using std::to_string;

class SimpleClock : public olc::PixelGameEngine
{
private:
	hhmmss lastTick = { -1, 0, 0 };
	const olc::vi2d center = { ScreenWidth() / 2, ScreenHeight() / 2 };

	const double outerRadius = min(center.x, center.y) - 1.0;
	const double midfieldRadius = outerRadius * 0.96;
	const double innerRadius = outerRadius * 0.9;
	const double hourHandLength = outerRadius * 0.6;

private:
	olc::vi2d GetClockCoords(const double angleInDegrees,
		const double multiplier) const
	{
		const auto angleInRadians = degToRad(angleInDegrees);

		return {
		static_cast<int32_t>(sin(angleInRadians) * multiplier) + center.x,
		static_cast<int32_t>(-cos(angleInRadians) * multiplier) + center.y
		};
	}

private:
	void DrawClockFace()
	{
		for (short i = 0; i < 360; i += 6) {
			const bool isFiveMinuteMark = i % 30 == 0;

			const double minuteMarkInnerRadius =
				isFiveMinuteMark ? innerRadius : midfieldRadius;

			const auto outer = 
				GetClockCoords(i, outerRadius - 1.0);

			const auto inner = 
				GetClockCoords(i, minuteMarkInnerRadius);

			DrawLine(outer, 
				inner, 
				isFiveMinuteMark ? olc::DARK_GREY : olc::VERY_DARK_GREY);
		}

		for (int i = 0; i < 12; ++i) {
			const auto inner =
				GetClockCoords((i + 1) * (360/12), midfieldRadius * 0.8);

			DrawString(inner, to_string(i+1));
		}

		DrawCircle(center, 
			static_cast<int32_t>(outerRadius), 
			olc::WHITE);
	}


private:
	void DrawHands()
	{
		DrawHourHand();
		DrawMinuteHand();
		DrawSecondsHand();
	}


private:
	inline void DrawHourHand() {
		const auto hoursAnalog = lastTick.hour % 12;

		// Hour hand position depends on hours and minutes (also seconds, but
		// we ignore that for now).
		const double hourAngle = 
			(static_cast<double>(hoursAnalog) * (360.0 / 12.0))
			+ (((360.0 / 12.0) * static_cast<double>(lastTick.minute)) / 60.0);

		const auto hourPosition = 
			GetClockCoords(hourAngle, hourHandLength);

		// Draw with appropriate thickness.
		DrawLine({ center.x + 1, center.y + 1 },
			{ hourPosition.x + 1, hourPosition.y + 1 },
			olc::DARK_RED);
		DrawLine(center,
			hourPosition,
			olc::RED);
		DrawLine({ center.x - 1, center.y - 1 },
			{ hourPosition.x - 1, hourPosition.y - 1 },
			olc::RED);
	}


private:
	inline void DrawMinuteHand() {
		const double minuteAngle = 
			static_cast<double>(lastTick.minute) * 6.0;

		const auto minutePosition = 
			GetClockCoords(minuteAngle, innerRadius - 1);

		DrawLine(center,
			minutePosition,
			olc::GREY);
	}


private:
	inline void DrawSecondsHand() {
		const double secondsAngle = 
			static_cast<double>(lastTick.second) * 6.0;

		const auto secondsPosition = 
			GetClockCoords(secondsAngle, innerRadius - 1);

		DrawLine(center,
			secondsPosition, 
			olc::WHITE);
	}


private:
	bool NextTick() {
		time_t osBinaryTime;  // C run-time time (defined in <time.h>)
		time(&osBinaryTime);  // Get the current time from the OS.
		CTime t(osBinaryTime);

		hhmmss currentTick = { t.GetHour(),
		t.GetMinute(),
		t.GetSecond() };

		if (currentTick != lastTick) {
			lastTick = currentTick;
			return true;
		}

		return false;
	}


public:
	SimpleClock()
	{
		sAppName = "Classic Clock";
	}


public:
	bool OnUserCreate() override
	{
		DrawClockFace();
		return true;
	}


public:
	bool OnUserUpdate(float fElapsedTime) override
	{
		if (NextTick()) {
			Clear(olc::BLACK);
			DrawClockFace();
			DrawHands();
		}

		return true;
	}
};


int main()
{
	SimpleClock clock;

	if (clock.Construct(256, 240, 4, 4)) {
		clock.Start();
	}

	return 0;
}
