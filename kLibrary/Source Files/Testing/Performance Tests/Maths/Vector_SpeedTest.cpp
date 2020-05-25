#include "pch.hpp"
#include "Vector_SpeedTest.hpp"

#include "../../../Maths/Vectors/PredefinedVectors.hpp"


#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	struct V2
	{
		GETTER_CONSTEXPR(float, X, ((float*)&vec)[0])
		CONST_GETTER_CONSTEXPR(float, X, ((float*)&vec)[0])

		GETTER_CONSTEXPR(float, Y, ((float*)&vec)[1])
		CONST_GETTER_CONSTEXPR(float, Y, ((float*)&vec)[1])
		
		V2(const float _x = 0, const float _y = 0)
		{
			const auto ptr = (float*)&vec;

			ptr[0] = _x;
			ptr[1] = _y;
		}
		
	public:
		long long vec;
	};
	
	VectorSpeedTest::VectorSpeedTest()
		: PerformanceTestBase("Vector Speed Test")
	{}

	VectorSpeedTest::~VectorSpeedTest()
		= default;

	constexpr auto maxIter = kmaths::Big_Int_Type(1e3);
	
	using namespace kmaths;
	void VectorSpeedTest::Test()
	{
		Vec2f();
	}

	void VectorSpeedTest::Vec2f()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector2f", "Tightly Packed Vector" };
		SetUpParticipants(participants);
		
		V2 v2(5, 10);
		Vector2f kv2(5, 10);

		const auto vx = v2.X();
		const auto vy = v2.Y();
		
		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
			}
			
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
			}
		}
	}
}
#endif