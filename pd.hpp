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
PD operator+(const PD& other) const;
PD operator-(const PD& other) const;
PD operator*(float scalar) const;
friend PD operator*(float scalar, const PD& vec); //para que vaya  escalar * vector
PD operator/(float scalar) const;
float dot(const PD& other) const;
PD cross(const PD& other) const;
float module() const;
PD normalized();
#endif // PD_HPP