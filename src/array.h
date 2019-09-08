#include <vector>
#include <cmath>
#include <string>
#include <numeric>
#include <algorithm>

#include <iostream>

namespace numcpp{
	class Shape{
	public:
		Shape(){}

		Shape(const std::vector<unsigned int>& v) : dimensions(v) {}

		Shape(const Shape& other) : dimensions(other.dimensions) {}

		unsigned int dims() const {
			return dimensions.size();
		}

		unsigned int total_size() const {
			if(dimensions.size() == 0){
				return 0;
			}

			return std::accumulate(std::begin(dimensions),
				std::end(dimensions),
				1, std::multiplies<int>());
		}

		unsigned int sub_size(unsigned int i) const {
			if(i > dimensions.size()){
				return 0;
			}

			return std::accumulate(std::begin(dimensions) + i,
				std::end(dimensions),
				1, std::multiplies<int>());
		}

		bool operator==(const Shape& rhs) const noexcept {
			if(this->dimensions.size() != rhs.dimensions.size()){
				return false;
			}
			for(unsigned int i = 0; i < this->dimensions.size(); i++){
				if(this->dimensions[i] != rhs.dimensions[i]){
					return false;
				}
			}
			return true;
		}

		unsigned int operator[](unsigned int i) const{
			return dimensions[i];
		}

	private:
		std::vector<unsigned int> dimensions;
	};

	template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
	class array{
	public:
		// Constuctors
		array() : values(std::unique_ptr<T[]>(0)), shape(Shape()) {}

		array(T val, const std::vector<unsigned int>& dim) {
			array(val, Shape(dim));
		}

		array(T val, const Shape& shape_in) : shape(shape_in) {
			unsigned int count = shape.total_size();
			T* temp = new T[count]();
			std::fill_n(temp, val, count);
			values = std::unique_ptr<T[]>(temp);
		}

		array(const array<T>& other) {
			this->copy(other);
		}

		array(const std::vector<T>& input){
			T* temp = new T[input.size()]();
			std::copy(input.begin(), input.end(), temp);
			values = std::unique_ptr<T[]>(temp);

			this->shape = Shape(std::vector<unsigned int>(1, input.size()));
		}

		friend std::ostream &operator<<(std::ostream &out, const array<T>& arr) {
			if(arr.shape.total_size() == 0){
				out << "array([])";
				return out;
			}

			out << "array(" + std::string(arr.shape.dims(), '[');

			for(unsigned int i = 0; i < arr.shape.total_size();){
				out << arr.values[i];
				i += 1;

				unsigned int brackets = 0;
				for(int j = arr.shape.dims() - 1; j >= 0; j--){
					int prod = arr.shape.sub_size(j);
					if((i % prod) == 0){
						brackets += 1;
					}
				}
				out << std::string(brackets, ']');
				if(i != arr.shape.total_size()){
					out << ", " << std::string(brackets, '\n');
					if(brackets){
						out << std::string(6 + arr.shape.dims() - brackets, ' ');
					}
					out << std::string(brackets, '[');
				}

			}
			out << ")";
			return out;            
		}

		array<T>& operator=(const array<T>& other) noexcept {
			if(&other == this){
				return *this;
			}
			
			this->copy(other);

			return *this;
		}

		array<T> operator+(T value) const noexcept {
			array<T> other = array(*this);
			for(unsigned int i = 0; i < other.shape.total_size(); i++){
				other.values[i] += value;
			}
			return other;
		}

		// array<T> operator-(T value) const noexcept {
		// 	array<T> other = array(*this);
		// 	for(unsigned int i = 0; i < other.size(); i++){
		// 		other.values[i] -= value;
		// 	}
		// 	return other;
		// }

		// array<T> operator*(T value) const noexcept {
		// 	array<T> other = array(*this);
		// 	for(unsigned int i = 0; i < other.size(); i++){
		// 		other.values[i] *= value;
		// 	}
		// 	return other;
		// }

		// array<T> operator/(T value) const noexcept {
		// 	array<T> other = array(*this);
		// 	for(unsigned int i = 0; i < other.size(); i++){
		// 		other.values[i] /= value;
		// 	}
		// 	return other;
		// }

		// array<T> operator%(T value) const noexcept {
		// 	array<T> other = array(*this);
		// 	for(unsigned int i = 0; i < other.size(); i++){
		// 		other.values[i] %= value;
		// 	}
		// 	return other;
		// }

		// array<T> operator^(T value) const {
		// 	array<T> other = array(*this);
		// 	for(unsigned int i = 0; i < other.size(); i++){
		// 		other.values[i] = pow(this->values[i], value);
		// 	}
		// 	return other;
		// }

		array<T> operator+=(T value) noexcept {
			for(unsigned int i = 0; i < this->shape.total_size(); i++){
				this->values[i] += value;
			}
			return *this;
		}

		// array<T> operator-=(T value) noexcept {
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		this->values[i] -= value;
		// 	}
		// 	return *this;
		// }

		// array<T> operator*=(T value) noexcept {
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		this->values[i] *= value;
		// 	}
		// 	return *this;
		// }

		// array<T> operator/=(T value) noexcept {
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		this->values[i] /= value;
		// 	}
		// 	return *this;
		// }

		// array<T> operator%=(T value) noexcept {
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		this->values[i] %= value;
		// 	}
		// 	return *this;
		// }

		// array<T> operator^=(T value) noexcept {
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		this->values[i] = pow(this->values[i], value);
		// 	}
		// 	return *this;
		// }

		// array<bool> operator>(T value) const noexcept {
		// 	array<bool> other;
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		other.push_back(this->values[i] > value);
		// 	}
		// 	return other;
		// }

		// array<bool> operator<(T value) const noexcept {
		// 	array<bool> other;
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		other.push_back(this->values[i] < value);
		// 	}
		// 	return other;
		// }

		// array<bool> operator!=(T value) const noexcept {
		// 	array<bool> other;
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		other.push_back(this->values[i] != value);
		// 	}
		// 	return other;
		// }

		// array<bool> operator==(T value) const noexcept {
		// 	array<bool> other;
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		other.push_back(this->values[i] == value);
		// 	}
		// 	return other;
		// }

		// array<bool> operator>=(T value) const noexcept {
		// 	array<bool> other;
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		other.push_back(this->values[i] >= value);
		// 	}
		// 	return other;
		// }

		// array<bool> operator<=(T value) const noexcept {
		// 	array<bool> other;
		// 	for(unsigned int i = 0; i < this->size(); i++){
		// 		other.push_back(this->values[i] <= value);
		// 	}
		// 	return other;
		// }

		T& operator[](int i) const {
			if(i >= this->shape[i]){
				throw std::out_of_range("index " 
					+ std::to_string(i) 
					+ " out of bounds for axis 0 of size " 
					+ std::to_string(this->shape[i]));
			}
			else if(i < 0){
				if ((-1 * i) > this->shape[i]){
					throw std::out_of_range("Index " 
						+ std::to_string(i) 
						+ " out of bounds for axis 0 of size "
						+ std::to_string(this->shape[i]));
				}
				i += this->shape[i];
			}
			std::vector<T> v(this->values[i], this->values[i] + this->shape.sub_size(1));
			return this->values[i];
		}

		// array<T> operator[](const array<int>& indeces){
		// 	array<T> other;
		// 	for(unsigned int i = 0; i < indeces.size(); i++){
		// 		other.push_back((*this)[indeces[i]]);
		// 	}
		// 	return other;
		// }

		// array<T> operator[](const array<bool>& indeces){
		// 	array<T> other;
		// 	for(unsigned int i = 0; i < indeces.size(); i++){
		// 		if(indeces[i] == true){
		// 			other.push_back((*this)[i]);
		// 		}
		// 	}
		// 	return other;
		// }

		template <typename T2>
		array<T2> asType(){
			array<T2> otherValues = array<T2>(0, shape);
			for(unsigned int i = 0; i < shape.total_size(); i++){
				otherValues[i] = values[i];
			}
			return otherValues;
		}

	private:

		std::unique_ptr<T[]> values;
		Shape shape;

		template <typename T2, typename std::enable_if<std::is_arithmetic<T2>::value>::type*>
		friend class array;

		void copy(const array<T>& other) {
			shape = other.shape;

			unsigned int count = shape.total_size();
			T* temp = new T[count]();
			for(unsigned int i = 0; i < count; ++i){
				temp[i] = other.values[i];
			}

			values = std::unique_ptr<T[]>(temp);
		}

		// void push_back(T val){
		// 	values.push_back(val);
		// }
	};

	array<double> zeros(int n){
		return array<double>(0.0, std::vector<unsigned int>(1, n));
	}

	array<double> ones(int n){
		return array<double>(1.0, std::vector<unsigned int>(1, n));
	}
}