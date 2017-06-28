#include <iostream>
#include <cstdint>
#include <algorithm>


//=================================================
// MAC Commands
//=================================================
class LinkCheckReq {
};

class LinkCheckAns {
};

class LinkADRReq {
};

class LinkADRAns {
};

class MACCommand {
};

class DevAddr {
private:
	uint8_t  _nwkid;
	uint32_t _nwkaddr;
public:
	DevAddr(uint8_t nwkid, uint32_t nwkaddr) 
		: _nwkid(nwkid), _nwkaddr(nwkaddr)
	{
	}

	uint8_t nwkid() { return _nwkid; }
	uint32_t nwkaddr() { return _nwkaddr; }

	std::string encode() {
		std::string buf;

		char octs[4] = {
			static_cast<char>((_nwkaddr & 0x000000ff)),
			static_cast<char>((_nwkaddr & 0x0000ff00) >> 8),
			static_cast<char>((_nwkaddr & 0x00ff0000) >> 16),
			static_cast<char>(((_nwkaddr & 0x01000000) >> 24) | (_nwkid << 1))
		};
		buf += std::string(octs, 4);

		return buf;
	}

	static DevAddr * decode(std::string bytes) {
		uint8_t  nwkid   = (bytes[3] >> 1) & 0x7f;
		uint32_t nwkaddr = (
			((bytes[0] << 0)  & 0x000000ff) |
			((bytes[1] << 8)  & 0x0000ff00) |
			((bytes[2] << 16) & 0x00ff0000) |
			((bytes[3] << 24) & 0xff000000) 
			) & 0x01ffffff;
		
		return new DevAddr(nwkid, nwkaddr);
	}
};

class AppNonce {
};

class DevNonce {
};

class NetId {
};

class DLSettings {
};

class FCtrl {
private:
	bool _adr;
	bool _adrackreq;
	bool _ack;
	bool _fpending;
	uint8_t _foptslen;
public:
	FCtrl(bool adr, bool adrackreq, bool ack, bool fpending, uint8_t foptslen)
		: _adr(adr), _adrackreq(adrackreq), _ack(ack), 
		  _fpending(fpending), _foptslen(foptslen)
	{
	}

	bool adr() { return _adr; }
	bool adrackreq() { return _adrackreq; }
	bool ack() { return _ack; }
	bool fpending() { return _fpending; }
	uint8_t foptslen() { return _foptslen; }

	std::string encode() {
		std::string buf;

		char octs[1] = {0};
		octs[0] |= (_adr       << 7);
		octs[0] |= (_adrackreq << 6);
		octs[0] |= (_ack       << 5);
		octs[0] |= (_fpending  << 4);
		octs[0] |= (_foptslen & 0x0f);

		buf += std::string(octs, 1);

		return buf;
	}

	static FCtrl * decode(std::string bytes) {
		bool    adr       = ((bytes[0] >> 7) & 0x01);
		bool    adrackreq = ((bytes[0] >> 6) & 0x01);
		bool    ack       = ((bytes[0] >> 5) & 0x01);
		bool    fpending  = ((bytes[0] >> 4) & 0x01);
		uint8_t foptslen  = ((bytes[0]     ) & 0x0f);

		return new FCtrl(adr, adrackreq, ack, fpending, foptslen);
	}
};

class FOpts {
private:
public:
	std::string encode() {
		return std::string("");
	}

};

class FHDR {
private:
	DevAddr * _devaddr;
	FCtrl * _fctrl;
	uint32_t _fcnt;
	FOpts * _fopts;
public:
	FHDR(DevAddr * devaddr, FCtrl * fctrl, uint32_t fcnt, FOpts * fopts)
		: _devaddr(devaddr), _fctrl(fctrl), _fcnt(fcnt), _fopts(fopts)
	{
	}

	DevAddr * devaddr() { return _devaddr; };
	FCtrl * fctrl() { return _fctrl; }
	uint32_t fcnt() { return _fcnt; }
	FOpts * fopts() { return _fopts; }

	std::string encode() {
		std::string buf;

		buf += _devaddr->encode();
		buf += _fctrl->encode();
		char octs[2] = {
			static_cast<char>((_fcnt & 0xff)),
			static_cast<char>((_fcnt & 0xff00) >> 8)
		};
		buf += std::string(octs, 2);
		buf += _fopts->encode();

		return buf;
	}
};

class FPort {
private:
public:
};


class Payload {
private:
	std::string _payload;
public:
	Payload(std::string * payload) {
		_payload = * payload;
	}

	std::string encode() {
		return _payload;
	}
};


class FRMPayload {
private:
	Payload * _payload;
public:
	FRMPayload(Payload * payload)
		: _payload(payload)
	{
	}

	std::string encode() {
		return _payload->encode();
	}

};

class MHDR {
private:
	uint8_t _mtype;
public:
	MHDR(uint8_t mtype)
		: _mtype(mtype)
	{
	}

	uint8_t mtype() { return _mtype; }
	uint8_t major() { return 0; }

	std::string encode() {
		std::string buf;

		char octs[1] = {
			static_cast<char>((_mtype & 0x07) << 5)
		};
		buf += std::string(octs, 1);

		return buf;
	}

	static MHDR * decode(std::string bytes) {
		uint8_t mtype = ((bytes[0] >> 5) & 0x07);

		return new MHDR(mtype);
	}
};

class MACPayload {
private:
	FHDR * _fhdr;
	FRMPayload * _frmpayload;
	uint8_t _fport;
public:
	MACPayload(FHDR * fhdr, FRMPayload * frmpayload, uint8_t fport)
		: _fhdr(fhdr), _frmpayload(frmpayload), _fport(fport)
	{
	}

	FHDR * fhdr() { return _fhdr; }
	FRMPayload * frmpayload() { return _frmpayload; }
	uint8_t fport() { return _fport; }

	std::string encode() {
		std::string buf;

		buf += _fhdr->encode();
		buf += _frmpayload->encode();
		char octs[1] = {
			static_cast<char>(_fport)
		};
		buf += std::string(octs, 1);

		return buf;
	}
};

class MIC {
private:
public:
	std::string encode() {
		return std::string();
	}
};

class EUI {
private:
public:
};

class AppEUI : EUI {
private:
public:
};

class DevEUI : EUI {
private:
public:
};

class JoinRequestPayload {
private:
public:
};

class ChannelFrequency {
private:
public:
};

class CFList {
private:
public:
};

class Delay {
private:
public:
};

class RXDelay : Delay {
private:
public:
};

class JoinAcceptPayload {
private:
public:
};

class PHYPayload {
private:
	MHDR * _mhdr;
	MACPayload * _macpayload;
	MIC * _mic;
public:
	PHYPayload(MHDR * mhdr, MACPayload * macpayload)
		: _mhdr(mhdr), _macpayload(macpayload)
	{
	}

	MHDR * mhdr() { return _mhdr; }
	MACPayload * macpayload() { return _macpayload; }
	MIC * mic() { return _mic; }

	std::string encode() {
		std::string buf;

		buf += _mhdr->encode();
		buf += _macpayload->encode();
		buf += _mic->encode();

		return buf;
	}
};



