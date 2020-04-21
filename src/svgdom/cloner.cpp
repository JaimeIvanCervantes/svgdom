#include "cloner.hpp"

using namespace svgdom;

void cloner::clone_children(const container& e, container& clone){
	auto oldParent = this->cur_parent;
	this->cur_parent = &clone;
	relayAccept(e);
	this->cur_parent = oldParent;
}

void cloner::visit(const g_element& e) {
	auto clone = std::make_unique<g_element>(e);
	this->clone_children(e, *clone);
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const symbol_element& e) {
	auto clone = std::make_unique<symbol_element>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const svg_element& e) {
	auto clone = std::make_unique<svg_element>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const radial_gradient_element& e) {
	auto clone = std::make_unique<radial_gradient_element>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const linear_gradient_element& e) {
	auto clone = std::make_unique<linear_gradient_element>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const defs_element& e) {
	auto clone = std::make_unique<defs_element>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const filter_element& e) {
	auto clone = std::make_unique<filter_element>(e);
	this->clone_children(e, *clone); 
	this->cur_parent->children.push_back(std::move(clone));
}


void cloner::visit(const polyline_element& e) {
	auto clone = std::make_unique<polyline_element>(e);
	this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const circle_element& e) {
	auto clone = std::make_unique<circle_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const use_element& e) {
	auto clone = std::make_unique<use_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const gradient::stop_element& e) {
	auto clone = std::make_unique<gradient::stop_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const path_element& e) {
	auto clone = std::make_unique<path_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const rect_element& e) {
	auto clone = std::make_unique<rect_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const line_element& e) {
	auto clone = std::make_unique<line_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const ellipse_element& e) {
	auto clone = std::make_unique<ellipse_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const polygon_element& e) {
	auto clone = std::make_unique<polygon_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const fe_gaussian_blur_element& e) {
	auto clone = std::make_unique<fe_gaussian_blur_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}

void cloner::visit(const image_element& e) {
	auto clone = std::make_unique<image_element>(e);
    this->cur_parent->children.push_back(std::move(clone));
}
