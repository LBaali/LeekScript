#include "ObjectSTD.hpp"

ObjectSTD::ObjectSTD() : Module("Object") {}

void ObjectSTD::include() {

	method("map", Type::OBJECT, {Type::OBJECT}, (void*) &object_map);
}

LSObject* object_map(const LSObject* object) {

	LSObject* new_obj = new LSObject();

	return new_obj;
}