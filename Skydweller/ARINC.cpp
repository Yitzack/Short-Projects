#include<iostream>
#include<cstdint>
#include<cmath>
using namespace std;

enum Status_Matrix : uint8_t
{
	NO = 0,	//Normal Operation
	NCD = 1	//No computed data
};

class ARINC_312
{
	public:
		ARINC_312()
		{
			data = 0;
			Set_Parity();
		}
		ARINC_312(const ARINC_312& A)
		{
			data = A.data;
			ssm = A.ssm;
			parity = A.parity;
		}
		ARINC_312 operator=(const ARINC_312 A)
		{
			data = A.data;
			ssm = A.ssm;
			parity = A.parity;
			return(A);
		}
		bool Decode(const uint32_t);	//Returns validity of code (true is good, false is error state) and writes it to itself if good
		uint32_t Encode(const ARINC_312) const;
		uint32_t Encode() const;
		bool Parity() const;
		float Data() const;
		Status_Matrix SSM() const;
		uint8_t SDI() const;
		uint8_t Label() const;
		void Set_Data(const float);
		void Set_SSM(const Status_Matrix);
		ostream& print(ostream&) const;
	private:
		void Set_Parity();
		bool Check_Parity(uint32_t) const;
		uint32_t Partial_Encode() const;	//Encode, but Parity bit isn't known
		bool parity;
		uint32_t data;
		Status_Matrix ssm = NCD;
		const uint8_t sdi = 1;
		const uint8_t label = 202;
};

bool ARINC_312::Decode(const uint32_t Code)
{
	if(!Check_Parity(Code) || (Code & uint32_t(1) << 28) || ((Code & 0x3FF) != 0x153))	//Parity fail, or SDI not 1, or label not 202, or negative data bit
		return(false);

	uint8_t Status = (Code & (uint32_t(3) << 29)) >> 29;
	switch(Status)
	{
		case 0:
			ssm = NO;
			break;
		case 1:
			ssm = NCD;
			break;
		default:	//Not NCD or NO state, not a valid label 202
			return(false);
	}
	parity = (Code & (uint32_t(1) << 31)) >> 31;
	data = (Code & (uint32_t(0x7ffff) << 10)) >> 10;

	return(true);
}

uint32_t ARINC_312::Encode() const
{
	return((uint32_t(parity) << 31) | Partial_Encode());
}

uint32_t ARINC_312::Partial_Encode() const
{
	uint32_t backwards_label = 0;
	for(int i = 0; i < 8; i++)
	{
		backwards_label |= ((label & (uint32_t(1) << i)) >> i) << (7-i);
	}

/*cout << hex << uint32_t(ssm) << " " << (uint32_t(ssm) << 29) << endl;
cout << data << " " << (data << 10) << endl;
cout << uint32_t(sdi) << " " << (uint32_t(sdi) << 8) << endl;
cout << uint32_t(label) << " " << backwards_label << endl;*/

	return((uint32_t(ssm) << 29) | (data << 10) | (uint32_t(sdi) << 8) | backwards_label);
}

uint32_t ARINC_312::Encode(const ARINC_312 A) const
{
	return(A.Encode());
}

bool ARINC_312::Parity() const
{
	return(parity);
}

float ARINC_312::Data() const
{
	return(float(data)*pow(2.,-6));
}

Status_Matrix ARINC_312::SSM() const
{
	return(ssm);
}

uint8_t ARINC_312::SDI() const
{
	return(sdi);
}

uint8_t ARINC_312::Label() const
{
	return(label);
}

void ARINC_312::Set_Data(const float speed)
{
	data = speed*pow(2.,6);
	Set_Parity();
	return;
}

void ARINC_312::Set_SSM(const Status_Matrix S)
{
	ssm = S;
	Set_Parity();
	return;
}

void ARINC_312::Set_Parity()
{
	uint32_t Bits = Partial_Encode();

	parity = true;	//Object parity
	while(Bits)	//Loop only executes as many times as there are set bits
	{
		parity = !parity;	//Flip parity every time there's a set bit
		Bits = Bits & (Bits - 1);	//Clear the least significant set bit. For example 0b01010000 & 0b01001111 = 0b01000000
	}

	return;
}

bool ARINC_312::Check_Parity(uint32_t Bits) const
{
	bool parity = false;	//function scope parity
	while(Bits)	//Loop only executes as many times as there are set bits
	{
		parity = !parity;	//Flip parity every time there's a set bit
		Bits = Bits & (Bits - 1);	//Clear the least significant set bit. For example 0b01010000 & 0b01001111 = 0b01000000
	}

	return(parity);
}

ostream& ARINC_312::print(ostream & os) const
{
	if(parity)
		os << "Parity ";
	else
		os << "No Parity ";

	switch(ssm)
	{
		case NO:
			os << "Normal Operation Speed:";
			break;
		case NCD:
			os << "No Computed Data Speed:";
			break;
	}

	os << Data() << " knots SDI: " << uint32_t(sdi) << " Label: " << uint32_t(label) << endl;

	return(os);
}

const float kn_to_ms = .5144;
const float ms_to_kn = 1./kn_to_ms;

int main()
{
	ARINC_312 Test;
	uint32_t Word = Test.Encode();
	cout << hex << "Default: 0x" << Word << " " << dec << Test.Data() << " " << Test.Data()*kn_to_ms << hex << endl;

	Test.Set_SSM(NO);
	Word = Test.Encode();
	cout << "Default Normal Operation: 0x" << Word << " " << dec << Test.Data() << " " << Test.Data()*kn_to_ms << hex << endl;

	Test.Set_Data(4095.984375);
	Word = Test.Encode();
	cout << "Normal Operation, 4095.984375 kt: 0x" << Word << " " << dec << Test.Data() << " " << Test.Data()*kn_to_ms << hex << endl;

	Test.Set_Data(.015625);
	Word = Test.Encode();
	cout << "Normal Operation, .015 kt: 0x" << Word << " " << dec << Test.Data() << " " << Test.Data()*kn_to_ms << hex << endl;

	Test.Set_Data(15.*ms_to_kn);
	Word = Test.Encode();
	cout << "Normal Operation, 15 m/s: 0x" << Word << " " << dec << Test.Data() << " " << Test.Data()*kn_to_ms << endl << endl;

	Word = 0xa0000153;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code" << endl;

	Word = 0x00000153;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code" << endl;

	Word = 0xffffd53;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code" << endl;

	Word = 0x80000553;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code" << endl;

	Word = 0x1d2953;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code" << endl;

	cout << "\nThe following bad codes are by design, program only knows bad code" << endl;
	Word = 0x90000153;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code (bad data sign bit)" << endl;

	Word = 0x90000150;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code (bad label)" << endl;

	Word = 0x80000053;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code (bad sdi)" << endl;

	Word = 0x90100153;
	if(Test.Decode(Word))
		Test.print(cout);
	else
		cout << "Bad Code (bad parity)" << endl;

	return(0);
}













