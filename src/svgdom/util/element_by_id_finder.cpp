#include "element_by_id_finder.hpp"

#include <utki/debug.hpp>

#include "../visitor.hpp"

using namespace svgdom;

namespace{
class cache_creator : virtual public svgdom::const_visitor{
private:
	void add_to_cache(const svgdom::element& e){
		if(!e.id.empty()){
			this->cache.insert(std::make_pair(e.id, &e));
		}
	}

public:
	std::unordered_map<std::string, const element*> cache;

	void default_visit(const element& e){
		this->add_to_cache(e);
	}
};
}

element_by_id_finder::element_by_id_finder(const svgdom::element& root) :
	cache([&root](){
		cache_creator cc;

		root.accept(cc);

		return std::move(cc.cache);
	}())
{}

const svgdom::element* element_by_id_finder::find(const std::string& id)const{
	if(id.length() == 0){
		return nullptr;
	}

	auto i = this->cache.find(id);
	if(i == this->cache.end()){
		return nullptr;
	}

	return i->second;
}



