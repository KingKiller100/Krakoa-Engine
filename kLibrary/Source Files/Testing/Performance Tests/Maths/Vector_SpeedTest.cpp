#include "pch.hpp"
#include "Vector_SpeedTest.hpp"

#include "../../../Maths/Vectors/PredefinedVectors.hpp"


#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	template<typename T>
	struct Vec
	{
		explicit Vec(const float _x = 0, const float _y = 0, const float _z = 0, const float _w = 0)
		{
			const auto ptr = (T*)&vec;

			ptr[0] = _x;
			ptr[1] = _y;
			ptr[2] = _z;
			ptr[3] = _w;
		}

		GETTER_CONSTEXPR(float, X, ((T*)&vec)[0])
			CONST_GETTER_CONSTEXPR(float, X, ((T*)&vec)[0])

			GETTER_CONSTEXPR(float, Y, ((T*)&vec)[1])
			CONST_GETTER_CONSTEXPR(float, Y, ((T*)&vec)[1])

			GETTER_CONSTEXPR(float, Z, ((T*)&vec)[2])
			CONST_GETTER_CONSTEXPR(float, Z, ((T*)&vec)[2])

			GETTER_CONSTEXPR(float, W, ((T*)&vec)[3])
			CONST_GETTER_CONSTEXPR(float, W, ((T*)&vec)[3])

	public:
		long long vec[sizeof(long long) == sizeof(T) ? 4
			: (sizeof(long long) / 2) == sizeof(T) ? 2
			: 1]{};
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
		Vec3f();
		Vec4f();
	}

	void VectorSpeedTest::Vec2f()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector3f", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<float> v2(5, 10);
		Vector2f kv2(5, 10);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
			}
		}
	}

	void VectorSpeedTest::Vec3f()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector3f", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<float> v2(5, 10, 15);
		Vector3f kv2(5, 10, 15);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
				kv2.Z() = kv2.Z()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
				v2.Z() = v2.Z()++;
			}
		}
	}

	void VectorSpeedTest::Vec4f()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector4f", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<float> v2(5, 10, 15, 20);
		Vector4f kv2(5, 10, 15, 20);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
				kv2.Z() = kv2.Z()++;
				kv2.W() = kv2.W()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
				v2.Z() = v2.Z()++;
				v2.W() = v2.W()++;
			}
		}
	}
}
#endif