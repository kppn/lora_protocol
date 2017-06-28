#define BOOST_TEST_MAIN    // main関数を定義
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include "protocol.cpp"

using namespace std;


BOOST_AUTO_TEST_CASE(encode_mhdr)
{
	MHDR mhdr(
			1 // mtype
		);

	BOOST_CHECK_EQUAL(
		mhdr.encode(),
		"\x20"s
	);
}


BOOST_AUTO_TEST_CASE(encode_devaddr)
{
	DevAddr devaddr(
		0x1,    // nwkid
		0x1     // nwkaddr
	);

	BOOST_CHECK_EQUAL(
		devaddr.encode(),
		"\x01\x00\x00\x02"s
	);
}


BOOST_AUTO_TEST_CASE(decode_devaddr)
{
	DevAddr * devaddr;

	devaddr = DevAddr::decode("\x00\x00\x00\x00"s);

	BOOST_CHECK_EQUAL( devaddr->nwkid(),      0 );
	BOOST_CHECK_EQUAL( devaddr->nwkaddr(),    0 );

	devaddr = DevAddr::decode("\x01\x00\x00\x83"s);

	BOOST_CHECK_EQUAL( devaddr->nwkid(),      65 );
	BOOST_CHECK_EQUAL( devaddr->nwkaddr(),    16777217 );
}


BOOST_AUTO_TEST_CASE(encode_fctrl)
{
	FCtrl fctrl(
		true, // adr
		true, // adrackreq
		true, // ack
		true, // fpending
		3     // foptslen
	);

	BOOST_CHECK_EQUAL(
		fctrl.encode(),
		"\xf3"s
	);
}

BOOST_AUTO_TEST_CASE(decode_fctrl)
{
	FCtrl * fctrl;
	
	fctrl = FCtrl::decode("\xff"s);

	BOOST_CHECK_EQUAL( fctrl->adr(),         true );
	BOOST_CHECK_EQUAL( fctrl->adrackreq(),   true );
	BOOST_CHECK_EQUAL( fctrl->ack(),         true );
	BOOST_CHECK_EQUAL( fctrl->fpending(),    true );
	BOOST_CHECK_EQUAL( fctrl->foptslen(),    15 );

	fctrl = FCtrl::decode("\x00"s);

	BOOST_CHECK_EQUAL( fctrl->adr(),         false );
	BOOST_CHECK_EQUAL( fctrl->adrackreq(),   false );
	BOOST_CHECK_EQUAL( fctrl->ack(),         false );
	BOOST_CHECK_EQUAL( fctrl->fpending(),    false );
	BOOST_CHECK_EQUAL( fctrl->foptslen(),    0 );
}


BOOST_AUTO_TEST_CASE(encode_frmpayload)
{
	FRMPayload frmpayload(
		new Payload(
			new std::string("\x01\x02")
		)
	);

	BOOST_CHECK_EQUAL(
		frmpayload.encode(),
		"\x01\x02"s
	);
}

BOOST_AUTO_TEST_CASE(encode_macpayload)
{
	MACPayload macpayload(
		new FHDR(
			new DevAddr(
				0x1,    // nwkid
				0x1     // nwkaddr
			),
			new FCtrl(
				true, // adr
				true, // adrackreq
				true, // ack
				true, // fpending
				3     // foptslen
			),
			0, // fcnt
			new FOpts()
		),
		new FRMPayload(
			new Payload(
				new std::string("\xab\xcd")
			)
		),
		// fport
		1
	);

	BOOST_CHECK_EQUAL(
		macpayload.encode(),
		"\x01\x00\x00\x02" "\xf3" "\x00\x00" "\xab\xcd" "\x01"s
	);
}


BOOST_AUTO_TEST_CASE(encode_phypayload)
{


	auto phypayload = new PHYPayload(
		new MHDR(
			1 // mtype
		),
		new MACPayload(
			new FHDR(
				new DevAddr(
					0x1,    // nwkid
					0x1     // nwkaddr
				),
				new FCtrl(
					true, // adr
					true, // adrackreq
					true, // ack
					true, // fpending
					3     // foptslen
				),
				0, // fcnt
				new FOpts()
			),
			new FRMPayload(
				new Payload(
					new std::string("\xab\xcd")
				)
			),
			// fport
			1
		)
	);

	BOOST_CHECK_EQUAL(
		phypayload->encode(),
		"\x20" "\x01\x00\x00\x02" "\xf3" "\x00\x00" "\xab\xcd" "\x01"s
	);
}

