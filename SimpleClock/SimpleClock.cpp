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

using olc::Pixel;
using olc::vi2d;

using std::min;
using std::string;
using std::to_string;

enum class Symbol {
	Numerals, Roman
};

class SimpleClock : public olc::PixelGameEngine
{
private:
	CTime lastTick;
	Symbol hourSymbols = Symbol::Roman;

	const vi2d center = { ScreenWidth() / 2, ScreenHeight() / 2 };

	const double outerRadius = min(center.x, center.y) - 1.0;
	const double midfieldRadius = outerRadius * 0.96;
	const double innerRadius = outerRadius * 0.9;
	const double hourHandLength = outerRadius * 0.6;

private:
	vi2d TransposeAroundClockAxis(const double angleInDegrees,
		const double radius) const
	{
		const auto angleInRadians = degToRad(angleInDegrees);

		return {
		static_cast<int32_t>(sin(angleInRadians) * radius) + center.x,
		static_cast<int32_t>(-cos(angleInRadians) * radius) + center.y
		};
	}

private:
	string Romanize(int n) {
		string str_romans[] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
		int values[] = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };

		string result = string();

		for (auto i = 0; i < 13; ++i)
		{
			while (n - values[i] >= 0)
			{
				result += str_romans[i];
				n -= values[i];
			}
		}
		return result;
	}

private: 
	void DrawNumeral(const int numericValue) 
	{
		const string numeral = (hourSymbols == Symbol::Numerals) ? 
			to_string(numericValue) : 
			Romanize(numericValue);

		const auto numeralSize = GetTextSizeProp(numeral);

		const auto inner =
			TransposeAroundClockAxis(numericValue * (360 / 12),
				midfieldRadius * 0.8);

		const auto adjustedInner = 
			vi2d(inner.x - (numeralSize.x / 2), 
			inner.y - (numeralSize.y / 2));

		DrawString(adjustedInner, numeral);
	}

private:
	void DrawClockFace()
	{
		for (short i = 0; i < 360; i += 6) {
			const bool isFiveMinuteMark = i % 30 == 0;

			const double minuteMarkInnerRadius =
				isFiveMinuteMark ? innerRadius : midfieldRadius;

			const auto outer = TransposeAroundClockAxis(i, outerRadius - 1.0);
			const auto inner = TransposeAroundClockAxis(i, minuteMarkInnerRadius);

			DrawLine(outer, 
				inner, 
				isFiveMinuteMark ? olc::DARK_GREY : olc::VERY_DARK_GREY);
		}

		for (int i = 1; i <= 12; ++i) {
			DrawNumeral(i);
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
		Sleep(10);
	}

private: 
	inline double GetHourhandAngleDegrees() const {
		const auto hoursAnalog = lastTick.GetHour() % 12;
		// Hour hand position depends on hours and minutes (also seconds, but
		// we ignore that for now).
		return
			(static_cast<double>(hoursAnalog) * (360.0 / 12.0))
			+ (((360.0 / 12.0) * static_cast<double>(lastTick.GetMinute())) / 60.0);	
	}

	inline bool IsVertical(const double angle) const {
		return (static_cast<int>(angle + 45) / 90) % 2;
	}

private:
	inline void DrawHourHand() {
		const auto hourHandAngle = GetHourhandAngleDegrees();

		const auto hourPosition = 
			TransposeAroundClockAxis(hourHandAngle, hourHandLength);

		// Draw with appropriate thickness.
		// Is it mostly vertical or mostly horizontal?
		const bool mostlyVertical = IsVertical(hourHandAngle);
		const int xIncrement = (mostlyVertical) ? 0 : 1;
		const int yIncrement = (mostlyVertical) ? 1 : 0;

		DrawLine({ center.x + xIncrement, center.y + yIncrement },
			{ hourPosition.x + xIncrement, hourPosition.y + yIncrement },
			olc::DARK_RED);
		DrawLine(center,
			hourPosition,
			olc::RED);
		DrawLine({ center.x - xIncrement, center.y - yIncrement },
			{ hourPosition.x - xIncrement, hourPosition.y - yIncrement },
			olc::RED);
	}


private:
	inline void DrawMinuteHand() {
		const double minuteAngle = 
			static_cast<double>(lastTick.GetMinute()) * 6.0;

		const auto minutePosition = 
			TransposeAroundClockAxis(minuteAngle, innerRadius - 1);

		DrawLine(center,
			minutePosition,
			olc::GREY);
	}
private:
	inline void DrawSecondsHand() {
		const double secondsAngle = 
			static_cast<double>(lastTick.GetSecond()) * 6.0;

		const auto secondsPosition = 
			TransposeAroundClockAxis(secondsAngle, innerRadius - 1);

		DrawLine(center,
			secondsPosition, 
			olc::WHITE);
	}


private:
	bool NextTick() {
		time_t osBinaryTime;  // C run-time time (defined in <time.h>)
		time(&osBinaryTime);  // Get the current time from the OS.
		CTime current(osBinaryTime);

		if (lastTick == current) {
			return false;
		}
		
		lastTick = current;
		return true;
	}


public:
	SimpleClock()
	{
		sAppName = "Classic Clock";
	}


public:
	bool OnUserCreate() override
	{
		lastTick = CTime(0);

		DrawClockFace();
		return true;
	}


public:
	bool OnUserUpdate(float fElapsedTime) override
	{
		const bool tabKeyReleased = GetKey(olc::Key::TAB).bReleased;

		if (tabKeyReleased || NextTick()) {
			if (tabKeyReleased) {
				hourSymbols = (hourSymbols == Symbol::Numerals) ?
					Symbol::Roman :
					Symbol::Numerals;
			}

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

	if (clock.Construct(256, 240, 2, 2)) {
		clock.Start();
	}

	return 0;
}
