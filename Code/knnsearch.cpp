/*
 * knnsearch.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: xiaochuanq
 */

#include "knnsearch.h"
#include "neuron.h"
#include "desc.h"
#include "quicksort.hpp"

NeuronKNN::NeuronKNN() {

}

NeuronKNN::~NeuronKNN() {

}

void NeuronKNN::train(const vector<NDPair>& data) {

}

NeuronType NeuronKNN::predict(const Descriptor&) {
	return 0;
}

void NeuronKNN::predict(const vector<NDPair>& data) {
}

void NeuronKNN::report() {
}

void NeuronKNN::crossValidate() {
}

void NeuronKNN::vote() {
}

NeuronType NeuronKNN::countVote() {
	return 0;
}
