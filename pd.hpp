#ifndef PD_HPP
#define PD_HPP

class PD {
public:
	PD(float x, float y, float z, float w);
	float& operator[](int i);
    const float& operator[](int i) const;

private:
	float vector[4];
}; 
PD operator+(const PD& other);
PD operator-(const PD& other);
PD operator*(float scalar);
float escalar(const PD& other);
PD vectorial(const PD& other);
float module(const PD& other);
#endif // PD_HPP