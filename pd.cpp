	#include <Eigen/Core>
	#include "pd.hpp"
	
	PD::PD(float x, float y, float z, float w) {
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
		vector[3] = w;
	}
	const float& PD::operator[](int i) const {
		return vector[i];
	}
	float& PD::operator[](int i) {
		return vector[i];
	}
