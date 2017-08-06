#pragma once

#include <vector>

#include "Element.hpp"

namespace svgdom{

/**
 * @brief An element which can have child elements.
 */
struct Container{
	std::vector<std::unique_ptr<Element>> children;

	/**
	 * @brief Push backs element with unique pointer.
	 * @param e - Element to push back (with unique pointer).
	 */
	template <class E>
	void pushBackUnique(E e) { children.push_back(utki::makeUnique<E>(e)); }

	/**
	 * @brief Relay accept to children.
	 * @param visitor - visitor to accept.
	 */
	void relayAccept(svgdom::Visitor& visitor)const;

};

}
