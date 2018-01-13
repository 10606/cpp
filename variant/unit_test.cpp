#include <gtest/gtest.h>

#include "variant.h"

//#include <variant>

#include <string>



// copyable and movable struct

struct cm_my_struct {

	constexpr cm_my_struct() = default;



	cm_my_struct(cm_my_struct const&) {

		logger += "c";

	}



	cm_my_struct(cm_my_struct&&) noexcept {

		logger += "m";

	}



	cm_my_struct& operator=(cm_my_struct&&) noexcept {

		std::cout << "hello\n";

		return *this;

	}



	cm_my_struct& operator=(cm_my_struct const&) {

		std::cout << "bye\n";

		return *this;

	}



	static std::string logger;

};

std::string cm_my_struct::logger;



// copyable and not movable struct

struct c_my_struct {

	constexpr c_my_struct() = default;



	c_my_struct(c_my_struct const&) {

		logger += "c";

	}



	c_my_struct(c_my_struct&&) = delete;



	static std::string logger;

};

std::string c_my_struct::logger;



// movable and not copyable struct

struct m_my_struct {

	constexpr m_my_struct() = default;



	m_my_struct(m_my_struct const&) = delete;



	m_my_struct(m_my_struct&&) {

		logger += "m";

	}

	friend void swap(m_my_struct&, m_my_struct&) noexcept;

	static std::string logger;

};

std::string m_my_struct::logger;



// not copyable and not movable struct

struct my_struct {

	constexpr my_struct() = default;



	my_struct(my_struct const&) = delete;



	my_struct(my_struct&&) = delete;



	static std::string logger;

};

std::string my_struct::logger;



struct non_default_constructible_struct {

	non_default_constructible_struct() = delete;

};





TEST(testVariant, defaultConstructible) {

   // variant<std::string, bool> w("abc");

	bool b = std::is_default_constructible_v<variant<int, double>>;

	EXPECT_TRUE(b);

	b = std::is_default_constructible_v<variant<non_default_constructible_struct, int>>;

	EXPECT_FALSE(b);

}



struct non_trivially_destructible_struct {

	~non_trivially_destructible_struct() {}

};



TEST(testVariant, triviallyDestructible) {

	variant<int, non_trivially_destructible_struct> non_triv_destr;;

	variant<int, double> triv_destr;

	bool b = std::is_trivially_destructible_v<variant<int, non_trivially_destructible_struct>>;

	EXPECT_FALSE(b);

	b = std::is_trivially_destructible_v<variant<int, double>>;

	EXPECT_TRUE(b);

}



TEST(testVariant, constexprTest) {

	constexpr variant<cm_my_struct> cm;

	constexpr variant<c_my_struct> c;

	constexpr variant<m_my_struct> m;

	constexpr variant<my_struct> ncnm;



	EXPECT_TRUE(std::is_default_constructible_v<variant<cm_my_struct>>);

	EXPECT_TRUE(std::is_default_constructible_v<variant<c_my_struct>>);

	EXPECT_TRUE(std::is_default_constructible_v<variant<m_my_struct>>);

	EXPECT_TRUE(std::is_default_constructible_v<variant<my_struct>>);

	variant<non_default_constructible_struct, int> v(std::in_place_index_t<1>(), 42);

}



struct st0 {

	st0() {

		logger += "ct";

	}

	~st0() {

		logger += "d";

	}

	static std::string logger;

};

std::string st0::logger;



struct st {

	st() {

		logger += "ct";

	}



	st(int a, int b) {

		logger += "ct(a, b)";

	}



	~st() {

		logger += "d";

	}

	static std::string logger;

};

std::string st::logger;



TEST(testVariant, emplace1) {

	st::logger = st0::logger = "";

	variant<st0, st> v;

	EXPECT_EQ(st::logger, "");

	EXPECT_EQ(st0::logger, "ct");

	v.emplace<st>(10, 20);

	EXPECT_EQ(st::logger, "ct(a, b)");

	EXPECT_EQ(st0::logger, "ctd");

}



TEST(testVariant, emplace3) {

	st::logger = st0::logger = "";

	variant<st0, st> v;

	EXPECT_EQ(st::logger, "");

	EXPECT_EQ(st0::logger, "ct");

	v.emplace<1>(10, 20);

	EXPECT_EQ(st::logger, "ct(a, b)");

	EXPECT_EQ(st0::logger, "ctd");

}



TEST(testVariant, swap) {

	variant<int, double> v(std::in_place_index_t<1>(), 3.14), w(std::in_place_index_t<0>(), 42);

	v.swap(w);

	EXPECT_EQ(get<0>(v), 42);

	EXPECT_EQ(get<1>(w), 3.14);

	variant<int, double> v1(std::in_place_index_t<1>(), 3.14), w1(std::in_place_index_t<1>(), 1.234);

	w1.swap(v1);

	EXPECT_EQ(get<1>(v1), 1.234);

	EXPECT_EQ(get<1>(w1), 3.14);

	//v1 = w1;

}



struct st1 {

	st1() {

		std::cout << "st1()\n";

	}

	st1(st1 const&) {

		std::cout << "st1(st1 const&)\n";

	}

	st1(st1&&) {

		std::cout << "st1(st1&&)\n";

	}

};



struct st2 : st1 {

	st2() {

		std::cout << "st2()\n";

	}

	st2(st2 const&) {

		std::cout << "st2(st2 const&)\n";

	}

	st2(st2&&) = delete;

};



struct st3 : st2 {

	st3() {

		std::cout << "st3()\n";

	}

	st3(st3 const&) {

		std::cout << "st3(st3 const&)\n";

	}

	st3(st3&&) = default;

};



namespace std {

	void swap(m_my_struct&, m_my_struct&) {

		std::cout << "swap(m_my_struct&, m_my_struct&)\n";

	}

};



TEST(testVariant, exceptionSafety) {

	variant<cm_my_struct> v, w;

	v = w;

	//std::variant<m_my_struct, int> v, w(std::in_place_index_t<1>(), 42);

	//std::cout << std::is_swappable_v<m_my_struct>;

	//v.swap(w);

}



TEST(testVariant, holdsAlternative) {

	variant<int, std::string> v = std::string("abc");

	EXPECT_FALSE(holds_alternative<int>(v));

	EXPECT_TRUE(holds_alternative<std::string>(v));

	EXPECT_FALSE(holds_alternative<m_my_struct>(v));

}



TEST(testVariant, get_if) {

	variant<int, float> v(12);

	EXPECT_EQ(*get_if<int>(&v), 12);

}



TEST(testVariant, checkEquals) {

	variant<int, double> v(52), w(3.14);

	EXPECT_FALSE(v == w);

	w = 52;

	EXPECT_TRUE(v == w);

}



TEST(testVariant, checkNotEquals) {

	variant<int, double> v(52), w(3.14);

	EXPECT_TRUE(v != w);

	w = 52;

	EXPECT_FALSE(v != w);

}



struct testNotEq {

	friend bool operator==(testNotEq const&, testNotEq const&) {

		return false;

	}



	friend bool operator!=(testNotEq const&, testNotEq const&) {

		return false;

	}

};



TEST(testVariant, checkCheatingNotEquals) {

	variant<testNotEq> v, w;

	EXPECT_FALSE(v == w);

	EXPECT_FALSE(v != w);

}



TEST(testVariant, checkLess) {

	variant<int, double> v(3), w(2.14);

	EXPECT_TRUE(v < w);

	v = 3.0;

	EXPECT_FALSE(v < w);

}



TEST(testVariant, checkGreater) {

	variant<int, double> v(3), w(2.14);

	EXPECT_FALSE(v > w);

	v = 3.0;

	EXPECT_TRUE(v > w);

}



struct fo {

	std::string operator()(int a, int b) const {

		std::stringstream ss;

		ss << a << " + " << b;

		return ss.str();

	}

	std::string operator()(std::string a, std::string b) const {

		std::stringstream ss;

		ss << a << " + " << b;

		return ss.str();

	}

	std::string operator()(std::string a, int b) const {

		std::stringstream ss;

		ss << a << " + " << b;

		return ss.str();

	}

	std::string operator()(int a, std::string b) const {

		std::stringstream ss;

		ss << a << " + " << b;

		return ss.str();

	}

};



struct fo2 {

	template <typename T1, typename T2>

	std::string operator()(T1 a, T2 b) const {

		std::stringstream ss;

		ss << a << " + " << b;

		return ss.str();

	}

};

struct concat 
{
	int operator()(int a, int b) const {
		return a + b;
	}
	std::string operator()(std::string a, std::string b) const {
		std::stringstream ss;
		ss << a << " + " << b;
		return ss.str();
	}
};

TEST(testVariant, invoke) {

    variant<std::string, bool> x("abc");

	auto lambda = [](auto a, auto b) -> std::string {

		std::stringstream ss;

		ss << a << " + " << b;

		return ss.str();

	};

	variant<int, std::string> v(3), w(std::string("hello"));

	EXPECT_EQ(visit(fo2(), v, w), "3 + hello");

	EXPECT_EQ(visit(lambda, v, w), "3 + hello");

	EXPECT_EQ(visit(fo(), v, w), "3 + hello");

	v = std::string("world");

	EXPECT_EQ(visit(fo(), v, w), "world + hello");

	w = 42;

	EXPECT_EQ(visit(fo(), v, w), "world + 42");

	EXPECT_EQ(visit(lambda, v, w), "world + 42");

	variant<int, double> vv(3.14);

	EXPECT_EQ(visit(fo2(), v, vv), "world + 3.14");

	EXPECT_EQ(visit(lambda, v, vv), "world + 3.14");

	v = 100;

	EXPECT_EQ(visit(fo(), v, w), "100 + 42");

	EXPECT_EQ(visit(lambda, v, w), "100 + 42");

    /*
    EXPECT_EQ(visit(concat(), v, w), 142);
	v = std::string("100");
	w = std::string("42");
    EXPECT_EQ(visit(concat(), v, w), "100 + 42");
    */

	//EXPECT_EQ(std::invoke(fo2(), 3, 4), "3 + 4");

	//EXPECT_EQ(std::invoke(fo2(), std::string("hello"), std::string("world")), "hello + world");

	//lambda(3, 4);

	//lambda(std::string("hello "), std::string("world"));

}


#include <stdio.h>

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

