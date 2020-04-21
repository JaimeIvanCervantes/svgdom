#pragma once

#include "elements/structurals.hpp"
#include "elements/shapes.hpp"
#include "elements/gradients.hpp"
#include "elements/filter.hpp"
#include "elements/image_element.hpp"
#include "elements/text_element.hpp"

namespace svgdom{
/**
 * @brief Visitor interface.
 * A visitor interface which allows traversing of the SVG element tree.
 * It utilizes the 'visitor' pattern.
 * Each element-based class can override the 'accept' method which will call
 * corresponding 'visit' method from visitor. And user can override visitor's methods
 * to implement their own operation to perform on each SVG element.
 */
class visitor{
private:
	container* curParent_v = nullptr;
	decltype(container::children)::iterator curIter_v;
	
protected:
	/**
	 * @brief Get current container whose children are being visited.
	 * @return Pointer to current traversed container.
	 * @return nullptr if root SVG element is being visited.
	 */
	decltype(curParent_v) cur_parent()const{
		return this->curParent_v;
	}

	// TODO: deprecated, remove.
	decltype(curParent_v) curParent()const{
		return this->cur_parent();
	}
	
	/**
	 * @brief Get iterator if current visited child element.
	 * Returns iterator into the parent container of the currently visited child element.
	 * Note, that removing the visited element from its parent during the element is visited will
	 * lead to undefined behavior. Instead, one should store the iterators until the whole SVG tree
	 * traversing is completed and only then perform elements removal if needed.
	 * @return Iterator of currently visited child element.
	 */
	decltype(container::children)::iterator cur_iter()const{
		return this->curIter_v;
	}

	// TODO: deprecated, remove.
	decltype(container::children)::iterator curIter()const{
		return this->cur_iter();
	}
	
	/**
	 * @brief Relay accept to children.
	 * @param container - container to whose children the 'accept' should be relayed.
	 */
	void relay_accept(container& c);

	// TODO: deprecated, remove.
	void relayAccept(container& c){
		this->relay_accept(c);
	}
	
public:
	virtual void visit(path_element& e);
	virtual void visit(rect_element& e);
	virtual void visit(circle_element& e);
	virtual void visit(ellipse_element& e);
	virtual void visit(line_element& e);
	virtual void visit(polyline_element& e);
	virtual void visit(polygon_element& e);
	virtual void visit(g_element& e);
	virtual void visit(svg_element& e);
	virtual void visit(symbol_element& e);
	virtual void visit(use_element& e);
	virtual void visit(defs_element& e);
	virtual void visit(gradient::stop_element& e);
	virtual void visit(linear_gradient_element& e);
	virtual void visit(radial_gradient_element& e);
	virtual void visit(filter_element& e);
	virtual void visit(fe_gaussian_blur_element& e);
	virtual void visit(fe_color_matrix_element& e);
	virtual void visit(FeBlendElement& e);
	virtual void visit(fe_composite_element& e);
	virtual void visit(image_element& e);
	virtual void visit(mask_element& e);
	virtual void visit(text_element& e);
	
	/**
	 * @brief Default visit method.
	 * This method is called by all the visit methods by default.
	 * @param e - SVG element to visit.
	 */
	virtual void default_visit(element& e){
		this->defaultVisit(e);
	}

	// TODO: deprecated, remove.
	virtual void defaultVisit(element& e){}
	
	/**
	 * @brief Default visit method for container elements.
	 * Default implementation of this method calls this->default_visit(e) and
	 * then calls this->relay_accept(c);
	 * @param e - element to visit.
	 * @param c - 'container' ancestor of the element to visit.
	 */
	virtual void default_visit(element& e, container& c);

	// TODO: deprecated, remove.
	virtual void defaultVisit(element& e, container& c){
		this->default_visit(e, c);
	}
	
	virtual ~visitor()noexcept{}
};

/**
 * @brief Constant version of visitor.
 * Same as visitor, but it takes all elements as 'const' arguments, so it cannot modify elements.
 */
class const_visitor{
protected:
	
	/**
	 * @brief Relay accept to children.
	 * @param container - container to whose children the 'accept' should be relayed.
	 */
	void relay_accept(const container& c);

	// TODO: deprecated, remove.
	void relayAccept(const container& c){
		this->relay_accept(c);
	}
	
public:
	virtual void visit(const path_element& e);
	virtual void visit(const rect_element& e);
	virtual void visit(const circle_element& e);
	virtual void visit(const ellipse_element& e);
	virtual void visit(const line_element& e);
	virtual void visit(const polyline_element& e);
	virtual void visit(const polygon_element& e);
	virtual void visit(const g_element& e);
	virtual void visit(const svg_element& e);
	virtual void visit(const symbol_element& e);
	virtual void visit(const use_element& e);
	virtual void visit(const defs_element& e);
	virtual void visit(const gradient::stop_element& e);
	virtual void visit(const linear_gradient_element& e);
	virtual void visit(const radial_gradient_element& e);
	virtual void visit(const filter_element& e);
	virtual void visit(const fe_gaussian_blur_element& e);
	virtual void visit(const fe_color_matrix_element& e);
	virtual void visit(const FeBlendElement& e);
	virtual void visit(const fe_composite_element& e);
	virtual void visit(const image_element& e);
	virtual void visit(const mask_element& e);
	virtual void visit(const text_element& e);
	
	/**
	 * @brief Default visit method.
	 * This method is called by all the visit methods by default.
	 * @param e - SVG element to visit.
	 */
	virtual void default_visit(const element& e){
		this->defaultVisit(e);
	}

	// TODO: deprecated, remove.
	virtual void defaultVisit(const element& e){}
	
	/**
	 * @brief Default visit method for container elements.
	 * Default implementation of this method calls this->default_visit(e) and
	 * then calls this->relay_accept(c);
	 * @param e - element to visit.
	 * @param c - 'container' ancestor of the element to visit.
	 */
	virtual void default_visit(const element& e, const container& c);

	// TODO: deprecated, remove.
	virtual void defaultVisit(const element& e, const container& c){
		this->default_visit(e, c);
	}
	
	virtual ~const_visitor()noexcept{}
};

// TODO: deprecated, remove.
typedef visitor Visitor;

// TODO: deprecated, remove.
typedef const_visitor ConstVisitor;

}
