#include<boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

#ifndef HAMMING
#define HAMMING

class Hamming
{
	public:
		Hamming(){};
		Hamming(uint8_t[]);
		void Encode(uint8_t[]);
		bool Decode(uint8_t[]) const;
		bool Decode(uint512_t, uint8_t[]) const;
		void Set_Encoding(uint512_t);
		void Set_Encoding(Hamming);
		uint512_t Export_Encoding() const;
		void FlipBit(int);
	private:
		uint512_t Encoding;
		const uint512_t BitMasks[16] =
		{uint512_t("0x0020a800021050001050100b004c3004008100090780000481010002000026322400204010a1040020640020c8020d000460000052400a09206184058084"),
		uint512_t("0x82014200044126212028006000800400024044a41000150000008110800210001090022c0a0230050280840406001011000c32000810c060041c20580850"),
		uint512_t("0x10000080102401120000ca8449008a015c2021408022609042000a090400000900600902814001400000210200058242080001dc85003410000009001003"),
		uint512_t("0x000404662800008044042010100001400006081048000a2014a40404081101844206540144000208480842491010202c2202480020820184128040202400"),
		uint512_t("0x69c00109000a880c09000500a0114028201082000001004a2852302010288840880080802014c09000131080206840008181040100090002010212000020"),
		uint512_t("0x041a1010c1800040828300000622009281081002205c8001000840c063c44000010900100008082295000810018000805010802200240000c80000824308"),
		uint512_t("0x55555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555"),
		uint512_t("0x33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333"),
		uint512_t("0x0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
		uint512_t("0x00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff00ff"),
		uint512_t("0x0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff"),
		uint512_t("0x00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000ffffffff"),
		uint512_t("0x0000000000000000ffffffffffffffff0000000000000000ffffffffffffffff0000000000000000ffffffffffffffff0000000000000000ffffffffffffffff"),
		uint512_t("0x00000000000000000000000000000000ffffffffffffffffffffffffffffffff00000000000000000000000000000000ffffffffffffffffffffffffffffffff"),
		uint512_t("0x0000000000000000000000000000000000000000000000000000000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"),
		uint512_t("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")};
		bool Parity(uint512_t, uint512_t) const;
		uint512_t InsertBit(uint512_t, int, bool) const;
		uint512_t RemoveBit(uint512_t, int) const;
		uint512_t FlipBit(uint512_t, int) const;
};
#endif
