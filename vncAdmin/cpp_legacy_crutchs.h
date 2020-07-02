#ifndef __CPP_LEGACY_CRUTCHS__H
#define __CPP_LEGACY_CRUTCHS__H

/*
char fromBinary(int x)
{
	char result = ((x / 01ul) % 010) * (2 >> 1);
}
/**/

#if defined(_MSC_VER) && _MSC_VER < 1900
#define BIN___(x)                                        \
	(                                                \
	((x / 01ul) % 010)*(2>>1) +                      \
	((x / 010ul) % 010)*(2<<0) +                     \
	((x / 0100ul) % 010)*(2<<1) +                    \
	((x / 01000ul) % 010)*(2<<2) +                   \
	((x / 010000ul) % 010)*(2<<3) +                  \
	((x / 0100000ul) % 010)*(2<<4) +                 \
	((x / 01000000ul) % 010)*(2<<5) +                \
	((x / 010000000ul) % 010)*(2<<6)                 \
	)
#else
#endif

#define BIN8(x) BIN___(0##x)

#define BIN16(x1,x2) \
    ((BIN(x1)<<8)+BIN(x2))

#define BIN24(x1,x2,x3) \
    ((BIN(x1)<<16)+(BIN(x2)<<8)+BIN(x3))

#define BIN32(x1,x2,x3,x4) \
    ((BIN(x1)<<24)+(BIN(x2)<<16)+(BIN(x3)<<8)+BIN(x4))

#define BIN64(x1,x2,x3,x4,x5,x6,x7,x8) \
    ((__int64(BIN32(x1,x2,x3,x4)) << 32) + __int64(BIN32(x5,x6,x7,x8)))




template< char FIRST, char... REST > struct binary
{
	static_assert(FIRST == '0' || FIRST == '1', "invalid binary digit");
	enum { value = ((FIRST - '0') << sizeof...(REST)) + binary<REST...>::value };
};

template<> struct binary<'0'> { enum { value = 0 }; };
template<> struct binary<'1'> { enum { value = 1 }; };

/*
// raw literal operator
template<  char... LITERAL > inline
constexpr unsigned int operator "" _b() { return binary<LITERAL...>::value; }

// raw literal operator
template<  char... LITERAL > inline
constexpr unsigned int operator "" _B() { return binary<LITERAL...>::value; }
*/

#endif