#include "length.hpp"

#include <sstream>
#include <iomanip>
#include <cmath>

#include "util.hxx"

using namespace svgdom;

length length::parse(const std::string& str) {
	length ret;

	std::istringstream ss(str);
	
	ss >> std::skipws;
	
	ret.value = read_in_real(ss);
	
	std::string u;
	
	ss >> std::setw(2) >> u >> std::setw(0);
	
	if(u.length() == 0){
		ret.unit = length_unit::number;
	}else if(u == "%"){
		ret.unit = length_unit::percent;
	}else if(u == "em"){
		ret.unit = length_unit::em;
	}else if(u == "ex"){
		ret.unit = length_unit::ex;
	}else if(u == "px"){
		ret.unit = length_unit::px;
	}else if(u == "cm"){
		ret.unit = length_unit::cm;
	}else if(u == "mm"){
		ret.unit = length_unit::mm;
	}else if(u == "in"){
		ret.unit = length_unit::in;
	}else if(u == "pt"){
		ret.unit = length_unit::pt;
	}else if(u == "pc"){
		ret.unit = length_unit::pc;
	}else{
		ret.unit = length_unit::unknown;
	}
	
	return ret;
}

real length::to_px(real dpi) const noexcept{
	switch(this->unit){
		default:
			return 0;
		case svgdom::length_unit::number:
		case svgdom::length_unit::px:
			return this->value;
		case svgdom::length_unit::in:
			return std::ceil(this->value * dpi);
		case svgdom::length_unit::cm:
			return std::ceil(this->value * (dpi / real(2.54)));
		case svgdom::length_unit::mm:
			return std::ceil(this->value * (dpi / real(25.4)));
		case svgdom::length_unit::pt: // 1 pt = 1/72 of an inch
			return std::ceil(this->value * (dpi / real(72)));
		case svgdom::length_unit::pc: // 1 pc = 1/6 of an inch
			return std::ceil(this->value * (dpi / real(6)));
		case svgdom::length_unit::em:
		case svgdom::length_unit::ex:
			// em and ex depend on the font size. Text is not supported by svgdom, so return 0 size.
			return 0;
	}
}

std::ostream& operator<<(std::ostream& s, const length& l){
	s << l.value;
	
	switch(l.unit){
		case length_unit::unknown:
		case length_unit::number:
		default:
			break;
		case length_unit::percent:
			s << "%";
			break;
		case length_unit::em:
			s << "em";
			break;
		case length_unit::ex:
			s << "ex";
			break;
		case length_unit::px:
			s << "px";
			break;
		case length_unit::cm:
			s << "cm";
			break;
		case length_unit::mm:
			s << "mm";
			break;
		case length_unit::in:
			s << "in";
			break;
		case length_unit::pt:
			s << "pt";
			break;
		case length_unit::pc:
			s << "pc";
			break;
	}
	
	return s;
}
