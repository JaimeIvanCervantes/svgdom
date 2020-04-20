#include "stream_writer.hpp"

#include <utki/util.hpp>

#include "util.hxx"

using namespace svgdom;

void StreamWriter::set_name(const std::string& name) {
	this->name = name;
}

void StreamWriter::add_attribute(const std::string& name, const std::string& value) {
	this->attributes.push_back(std::make_pair(name, value));
}

void StreamWriter::add_attribute(const std::string& name, const length& value){
	std::stringstream ss;
	ss << value;
	this->add_attribute(name, ss.str());
}

void StreamWriter::add_attribute(const std::string& name, real value){
	std::stringstream ss;
	ss << value;
	this->add_attribute(name, ss.str());
}

void StreamWriter::write(const container* children) {
	auto ind = indent_str();

	auto tag = std::move(this->name);
	
	this->s << ind << "<" << tag;
	
	for(auto& a : this->attributes){
		this->s << " " << a.first << "=\"" << a.second << "\"";
	}

	this->attributes.clear();
	this->name.clear();
	
	if (!children || children->children.size() == 0) {
		this->s << "/>";
	} else {
		this->s << ">" << std::endl;
		this->childrenToStream(*children);
		this->s << ind << "</" << tag << ">";
	}
	this->s << std::endl;
}

std::string StreamWriter::indent_str() {
	std::string ind;

	std::stringstream ss;
	for(unsigned i = 0; i != this->indent; ++i){
		ss << "\t";
	}
	return ss.str();
}

void StreamWriter::childrenToStream(const container& e) {
	++this->indent;
	utki::ScopeExit scopeExit([this]() {
		--this->indent;
	});
	for (auto& c : e.children) {
		c->accept(*this);
	}
}

void StreamWriter::add_element_attributes(const element& e) {
	if(e.id.length() != 0){
		this->add_attribute("id", e.id);
	}
}

void StreamWriter::add_transformable_attributes(const Transformable& e) {
	if(e.transformations.size() != 0){
		this->add_attribute("transform", e.transformationsToString());
	}
}

void StreamWriter::add_styleable_attributes(const Styleable& e) {
	if(e.styles.size() != 0){
		this->add_attribute("style", e.stylesToString());
	}
}

void StreamWriter::add_view_boxed_attributes(const ViewBoxed& e) {
	if (e.isViewBoxSpecified()) {
		this->add_attribute("viewBox", e.viewBoxToString());
	}
}

void StreamWriter::add_aspect_ratioed_attributes(const aspect_ratioed& e) {
	if (e.preserveAspectRatio.preserve != aspect_ratioed::PreserveAspectRatio_e::NONE || e.preserveAspectRatio.defer || e.preserveAspectRatio.slice) {
		this->add_attribute("preserveAspectRatio", e.preserve_aspect_ratio.to_string());
	}
}

void StreamWriter::add_rectangle_attributes(const rectangle& e, const rectangle& defaultValues) {
	if(e.isXSpecified() && e.x != defaultValues.x){
		this->add_attribute("x", e.x);
	}
	
	if(e.isYSpecified() && e.y != defaultValues.y){
		this->add_attribute("y", e.y);
	}
	
	if(e.isWidthSpecified() && e.width != defaultValues.width){
		this->add_attribute("width", e.width);
	}
	
	if(e.isHeightSpecified() && e.height != defaultValues.height){
		this->add_attribute("height", e.height);
	}
}

void StreamWriter::add_referencing_attributes(const referencing& e) {
	if(e.iri.length() != 0){
		this->add_attribute("xlink:href", e.iri);
	}
}

void StreamWriter::add_gradient_attributes(const gradient& e) {
	this->add_element_attributes(e);
	this->add_referencing_attributes(e);
	this->add_styleable_attributes(e);
	
	if(e.spreadMethod != gradient::spread_method_kind::default_kind){
		this->add_attribute("spreadMethod", e.spreadMethodToString());
	}
	
	if(e.units != coordinate_units::unknown){
		this->add_attribute("gradientUnits", coordinateUnitsToString(e.units));
	}
	
	if(e.transformations.size() != 0){
		this->add_attribute("gradientTransform", e.transformationsToString());
	}
}

void StreamWriter::add_filter_primitive_attributes(const FilterPrimitive& e) {
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_rectangle_attributes(e);
	
	if(e.result.length() != 0){
		this->add_attribute("result", e.result);
	}
}

void StreamWriter::add_inputable_attributes(const Inputable& e) {
	if(e.in.length() != 0){
		this->add_attribute("in", e.in);
	}
}

void StreamWriter::add_second_inputable_attributes(const SecondInputable& e) {
	if(e.in2.length() != 0){
		this->add_attribute("in2", e.in2);
	}
}

void StreamWriter::add_text_positioning_attributes(const TextPositioning& e) {
	//TODO: add missing attributes
}


void StreamWriter::add_shape_attributes(const shape& e) {
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
}


void StreamWriter::visit(const GElement& e) {
	this->setName("g");
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
	this->write(&e);
}

void StreamWriter::visit(const SvgElement& e) {
	this->setName("svg");
	
	if(this->indent == 0){//if outermost "svg" element
		this->add_attribute("xmlns", "http://www.w3.org/2000/svg");
		this->add_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
		this->add_attribute("version", "1.1");
	}
	
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_rectangle_attributes(e);
	this->add_view_boxed_attributes(e);
	this->add_aspect_ratioed_attributes(e);
	this->write(&e);
}

void StreamWriter::visit(const image_element& e){
	this->setName("image");
	
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_transformable_attributes(e);
	this->add_rectangle_attributes(
			e,
			rectangle(
					length(0, length_unit::NUMBER),
					length(0, length_unit::NUMBER),
					length(0, length_unit::NUMBER),
					length(0, length_unit::NUMBER)
				)
		);
	this->add_referencing_attributes(e);
	this->add_aspect_ratioed_attributes(e);
	this->write();
}

void StreamWriter::visit(const line_element& e) {
	this->setName("line");
	this->add_shape_attributes(e);
	
	if(e.x1.unit != length_unit::unknown){
		this->add_attribute("x1", e.x1);
	}
	
	if(e.y1.unit != length_unit::unknown){
		this->add_attribute("y1", e.y1);
	}
	
	if(e.x2.unit != length_unit::unknown){
		this->add_attribute("x2", e.x2);
	}
	
	if(e.y2.unit != length_unit::unknown){
		this->add_attribute("y2", e.y2);
	}
	
	this->write();
}

void StreamWriter::visit(const rect_element& e) {
	this->setName("rect");
	this->add_shape_attributes(e);
	this->add_rectangle_attributes(e, rect_element::rectangle_default_values());
	
	if(e.rx.unit != length_unit::unknown){
		this->add_attribute("rx", e.rx);
	}
	
	if(e.ry.unit != length_unit::unknown){
		this->add_attribute("ry", e.ry);
	}
	
	this->write();
}

void StreamWriter::visit(const ellipse_element& e) {
	this->setName("ellipse");
	this->add_shape_attributes(e);
	
	if(e.cx.unit != length_unit::unknown){
		this->add_attribute("cx", e.cx);
	}
	
	if(e.cy.unit != length_unit::unknown){
		this->add_attribute("cy", e.cy);
	}
	
	if(e.rx.unit != length_unit::unknown){
		this->add_attribute("rx", e.rx);
	}
	
	if(e.ry.unit != length_unit::unknown){
		this->add_attribute("ry", e.ry);
	}
	
	this->write();
}

void StreamWriter::visit(const polygon_element& e) {
	this->setName("polygon");
	this->add_shape_attributes(e);
	if(e.points.size() != 0){
		this->add_attribute("points", e.pointsToString());
	}
	this->write();
}

void StreamWriter::visit(const polyline_element& e) {
	this->setName("polyline");
	this->add_shape_attributes(e);
	if(e.points.size() != 0){
		this->add_attribute("points", e.pointsToString());
	}
	this->write();
}

void StreamWriter::visit(const circle_element& e) {
	this->setName("circle");
	this->add_shape_attributes(e);
	
	if(e.cx.unit != length_unit::unknown){
		this->add_attribute("cx", e.cx);
	}
	
	if(e.cy.unit != length_unit::unknown){
		this->add_attribute("cy", e.cy);
	}
	
	if(e.r.unit != length_unit::unknown){
		this->add_attribute("r", e.r);
	}
	this->write();
}

void StreamWriter::visit(const path_element& e){
	this->setName("path");
	this->add_shape_attributes(e);
	if(e.path.size() != 0){
		this->add_attribute("d", e.pathToString());
	}
	this->write();
}

void StreamWriter::visit(const UseElement& e) {
	this->setName("use");
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
	this->add_rectangle_attributes(e);
	this->add_referencing_attributes(e);
	this->write();
}

void StreamWriter::visit(const gradient::stop_element& e) {
	this->setName("stop");
	this->add_attribute("offset", e.offset);
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->write();
}

void StreamWriter::visit(const radial_gradient_element& e) {
	this->setName("radialGradient");
	this->add_gradient_attributes(e);
	if(e.cx.unit != length_unit::PERCENT || e.cx.value != 50){
		this->add_attribute("cx", e.cx);
	}
	if(e.cy.unit != length_unit::PERCENT || e.cy.value != 50){
		this->add_attribute("cy", e.cy);
	}
	if(e.r.unit != length_unit::PERCENT || e.r.value != 50){
		this->add_attribute("r", e.r);
	}
	if(e.fx.unit != length_unit::unknown){
		this->add_attribute("fx", e.fx);
	}
	if(e.fy.unit != length_unit::unknown){
		this->add_attribute("fy", e.fy);
	}
	this->write(&e);
}

void StreamWriter::visit(const linear_gradient_element& e) {
	this->setName("linearGradient");
	this->add_gradient_attributes(e);
	if(e.x1.unit != length_unit::PERCENT || e.x1.value != 0){
		this->add_attribute("x1", e.x1);
	}
	if(e.y1.unit != length_unit::PERCENT || e.y1.value != 0){
		this->add_attribute("y1", e.y1);
	}
	if(e.x2.unit != length_unit::PERCENT || e.x2.value != 100){
		this->add_attribute("x2", e.x2);
	}
	if(e.y2.unit != length_unit::PERCENT || e.y2.value != 0){
		this->add_attribute("y2", e.y2);
	}
	this->write(&e);
}

void StreamWriter::visit(const DefsElement& e) {
	this->setName("defs");
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
	this->write(&e);
}

void StreamWriter::visit(const MaskElement& e) {
	this->setName("mask");
	this->add_element_attributes(e);
	this->add_rectangle_attributes(e);
	this->add_styleable_attributes(e);
	
	if(e.maskUnits != coordinate_units::object_bounding_box && e.maskUnits != coordinate_units::unknown){
		this->add_attribute("maskUnits", coordinateUnitsToString(e.maskUnits));
	}
	
	if(e.maskContentUnits != coordinate_units::user_space_on_use && e.maskContentUnits != coordinate_units::unknown){
		this->add_attribute("maskContentUnits", coordinateUnitsToString(e.maskContentUnits));
	}
	
	this->write(&e);
}

void StreamWriter::visit(const TextElement& e){
	this->setName("text");
	this->add_element_attributes(e);
	this->add_transformable_attributes(e);
	this->add_styleable_attributes(e);
	this->add_text_positioning_attributes(e);
	
	//TODO: add text element attributes
	
	this->write(&e);
}

void StreamWriter::visit(const SymbolElement& e) {
	this->setName("symbol");
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_view_boxed_attributes(e);
	this->add_aspect_ratioed_attributes(e);
	this->write(&e);
}

void StreamWriter::visit(const FilterElement& e){
	this->setName("filter");
	this->add_element_attributes(e);
	this->add_styleable_attributes(e);
	this->add_rectangle_attributes(
			e,
			rectangle(
				length(-10, length_unit::PERCENT),
				length(-10, length_unit::PERCENT),
				length(120, length_unit::PERCENT),
				length(120, length_unit::PERCENT)
			)
		);
	this->add_referencing_attributes(e);
	
	if(e.filterUnits != coordinate_units::unknown && e.filterUnits != coordinate_units::object_bounding_box){
		this->add_attribute("filterUnits", coordinateUnitsToString(e.filterUnits));
	}

	if(e.primitiveUnits != coordinate_units::unknown && e.primitiveUnits != coordinate_units::user_space_on_use){
		this->add_attribute("primitiveUnits", coordinateUnitsToString(e.primitiveUnits));
	}
	
	this->write(&e);
}

void StreamWriter::visit(const FeGaussianBlurElement& e){
	this->setName("feGaussianBlur");
	
	this->add_filter_primitive_attributes(e);
	this->add_inputable_attributes(e);
	
	if(e.isStdDeviationSpecified()){
		this->add_attribute("stdDeviation", numberOptionalNumberToString(e.stdDeviation, -1));
	}
	this->write();
}

void StreamWriter::visit(const FeColorMatrixElement& e){
	this->setName("feColorMatrix");
	
	this->add_filter_primitive_attributes(e);
	this->add_inputable_attributes(e);
	
	{
		std::string typeValue;
		
		//write type
		switch(e.type){
			default:
			case FeColorMatrixElement::Type_e::MATRIX:
				//default value is 'matrix', so omit type attribute.
				break;
			case FeColorMatrixElement::Type_e::HUE_ROTATE:
				typeValue = "hueRotate";
				break;
			case FeColorMatrixElement::Type_e::SATURATE:
				typeValue = "saturate";
				break;
			case FeColorMatrixElement::Type_e::LUMINANCE_TO_ALPHA:
				typeValue = "luminanceToAlpha";
				break;
		}
		if(typeValue.length() != 0){
			this->add_attribute("type", typeValue);
		}
	}
	
	{
		std::string valuesValue;
		switch(e.type){
			default:
			case FeColorMatrixElement::Type_e::MATRIX:
				//write 20 values
				{
					std::stringstream ss;
					for(unsigned i = 0; i != e.values.size(); ++i){
						if(i != 0){
							ss << " ";
						}
						ss << e.values[i];
					}
					valuesValue = ss.str();
				}
				break;
			case FeColorMatrixElement::Type_e::HUE_ROTATE:
				//fall-through
			case FeColorMatrixElement::Type_e::SATURATE:
				//write 1 value
				valuesValue = std::to_string(e.values[0]);
				break;
			case FeColorMatrixElement::Type_e::LUMINANCE_TO_ALPHA:
				//'values' attribute can be omitted, so do nothing
				break;
		}
		if(valuesValue.length() != 0){
			this->add_attribute("values", valuesValue);
		}
	}
	
	this->write();
}


void StreamWriter::visit(const FeBlendElement& e){
	this->setName("feBlend");
	
	this->add_filter_primitive_attributes(e);
	this->add_inputable_attributes(e);
	this->add_second_inputable_attributes(e);
	
	{
		std::string modeValue;
		switch(e.mode){
			default:
			case FeBlendElement::Mode_e::NORMAL:
				//default value, can be omitted
				break;
			case FeBlendElement::Mode_e::MULTIPLY:
				modeValue = "multiply";
				break;
			case FeBlendElement::Mode_e::SCREEN:
				modeValue = "screen";
				break;
			case FeBlendElement::Mode_e::DARKEN:
				modeValue = "darken";
				break;
			case FeBlendElement::Mode_e::LIGHTEN:
				modeValue = "lighten";
				break;
		}
		this->add_attribute("mode", modeValue);
	}
	
	this->write();
}

void StreamWriter::visit(const FeCompositeElement& e){
	this->setName("feComposite");
	
	this->add_filter_primitive_attributes(e);
	this->add_inputable_attributes(e);
	this->add_second_inputable_attributes(e);
	
	{
		std::string operatorValue;
		switch(e.operator_v){
			default:
			case FeCompositeElement::Operator_e::OVER:
				//default value, can be omitted
				break;
			case FeCompositeElement::Operator_e::IN:
				operatorValue = "in";
				break;
			case FeCompositeElement::Operator_e::OUT:
				operatorValue = "out";
				break;
			case FeCompositeElement::Operator_e::ATOP:
				operatorValue = "atop";
				break;
			case FeCompositeElement::Operator_e::XOR:
				operatorValue = "xor";
				break;
			case FeCompositeElement::Operator_e::ARITHMETIC:
				operatorValue = "arithmetic";
				break;
		}
		if(operatorValue.length() != 0){
			this->add_attribute("operator", operatorValue);
		}
	}
	
	if(e.k1 != real(0)){
		this->add_attribute("k1", std::to_string(e.k1));
	}
	
	if(e.k2 != real(0)){
		this->add_attribute("k2", std::to_string(e.k2));
	}
	
	if(e.k3 != real(0)){
		this->add_attribute("k3", std::to_string(e.k3));
	}
	
	if(e.k4 != real(0)){
		this->add_attribute("k4", std::to_string(e.k4));
	}
	
	this->write();
}
