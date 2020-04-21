#pragma once

#include "element.hpp"
#include "container.hpp"
#include "referencing.hpp"
#include "Transformable.hpp"
#include "styleable.hpp"
#include "coordinate_units.hpp"

namespace svgdom{

enum class gradient_spread_method{
	default_,
	DEFAULT = default_, // TODO: deprecated, remove.
	pad,
	PAD = pad, // TODO: deprecated, remove.
	reflect,
	REFLECT = reflect, // TODO: deprecated, remove.
	repeat,
	REPEAT = repeat // TODO: deprecated, remove.
};

/**
 * @brief Common base for gradient elements.
 */
struct gradient :
		public element,
		public container,
		public referencing,
		public Transformable,
		public styleable
{
	gradient_spread_method spread_method = gradient_spread_method::default_;

	// TODO: deprecated, remove.
	gradient_spread_method& spreadMethod = spread_method;

	// TODO: deprecated, remove.
	typedef gradient_spread_method SpreadMethod_e;
	
	coordinate_units units = coordinate_units::unknown;
	
	struct stop_element :
			public element,
			public styleable
	{
		real offset;
		
		void accept(visitor& v)override;
		void accept(const_visitor& v) const override;
	};

	// TODO: deprecated, remove.
	typedef stop_element StopElement;
	
	std::string spread_method_to_string()const;

	// TODO: deprecated, remove.
	std::string spreadMethodToString()const{
		return this->spread_method_to_string();
	}
};

struct linear_gradient_element : public gradient{
	length x1 = length(0, length_unit::unknown);
	length y1 = length(0, length_unit::unknown);
	length x2 = length(100, length_unit::unknown);
	length y2 = length(0, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

struct radial_gradient_element : public gradient{
	length cx = length(50, length_unit::unknown);
	length cy = length(50, length_unit::unknown);
	length r = length(50, length_unit::unknown);
	length fx = length(50, length_unit::unknown);
	length fy = length(50, length_unit::unknown);
	
	void accept(visitor& v)override;
	void accept(const_visitor& v) const override;
};

// TODO: deprecated, remove.
typedef gradient Gradient;

// TODO: deprecated, remove.
typedef linear_gradient_element LinearGradientElement;

// TODO: deprecated, remove.
typedef radial_gradient_element RadialGradientElement;

}