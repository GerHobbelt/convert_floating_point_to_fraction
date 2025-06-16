
#include "./convert_to_fraction.h"

#include <cstdint>

using namespace cvt_2_fraction;


namespace
{
	template<typename int_type>
	void Test(void)
	{
		Fraction<int_type> ret;
		double vut;

		vut = 0.1;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 0.99999997;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = (0x40000000 - 1.0) / (0x40000000 + 1.0);
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 1.0 / 3.0;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 1.0 / (0x40000000 - 1.0);
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 320.0 / 240.0;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 6.0 / 7.0;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 320.0 / 241.0;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 720.0 / 577.0;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 2971.0 / 3511.0;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 3041.0 / 7639.0;
		ret = toFract<int_type>(vut);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = 1.0 / std::sqrt(2);
		ret = toFract<int_type>(vut, 1E-9);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
		vut = std::numbers::pi_v<double>;
		ret = toFract<int_type>(vut, 1E-9);
		assert(std::abs(vut - toFloat(ret)) < 1E-9);
	}



	void TestFractionConversion(void) {
		Test<int>();
		Test<long>();
		Test<int32_t>();
		Test<int64_t>();
	}
}


#if defined(BUILD_MONOLITHIC)
#define main cvt2frac_test_main
#endif

extern "C"
int main(void) {
	TestFractionConversion();
	std::cout << "All tests passed!" << std::endl;
	return 0;
}
