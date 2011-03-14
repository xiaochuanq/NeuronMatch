/*
 * desc.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: xiaochuanq
 */
#include "desc.h"

Matrixf vv2matrix(const vector<Vector3>& points) {

} // dump a vector of Vector3 to a matrix

bool is_comparable(const Descritpor&, const Descriptor&) {
	return true;
}

float distance(const Descritpor&, const Descriptor&) {
	return 0.0f;
}

bool inregion(const Vector3& shpercoord) {
	return true;
}

Descriptor::Descriptor(int nz, int na, const vector<float> rlimits) {
}
Descriptor::~Descriptor() {
}
void Descriptor::create() {
}
void Descriptor::reset() {
}

void Descriptor::calcZenith() {
}
void Descriptor::alignZenith() {
}
void Descriptor::card2Polar() {
}
