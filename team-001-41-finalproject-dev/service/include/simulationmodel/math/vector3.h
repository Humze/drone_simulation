/**
 * AI Acknowledgement: Anthropic Claude 3.7 Sonnet
 * - Used for assistence with Doxygen Commenting
 */
#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <cmath>
#include <iostream>
#include <vector>

/**
 * @class Vector3
 * @brief A simple class used for vector math operations in 3D space
 *
 * This class provides functionality for vector operations including
 * addition, subtraction, multiplication, division, dot product,
 * cross product, magnitude calculation, and normalization.
 */
class Vector3 {
 public:
  double x = 0;
  double y = 0;
  double z = 0;

  /**
   * @brief Default constructor.
   */
  Vector3();

  /**
   * @brief Constructor that sets all components to the same value
   *
   * @param[in] a Value to assign to all coordinates (x, y, z)
   */
  Vector3(double a);
  /**
   * @brief Parameter constructor.
   *
   * @param[in] a x-coordinate
   * @param[in] b y-coordinate
   * @param[in] c z-coordinate
   */
  Vector3(double a, double b, double c);

  /**
   * @brief Constructor from a vector of doubles
   *
   * @param[in] v Vector of doubles with at least 3 elements
   * @throws std::invalid_argument if the vector has fewer than 3 elements
   */
  Vector3(const std::vector<double>& v);

  /**
   * @brief Constructor from a vector of floats
   *
   * @param[in] v Vector of floats with at least 3 elements
   * @throws std::invalid_argument if the vector has fewer than 3 elements
   */
  Vector3(const std::vector<float>& v);

  /**
   * @brief Equality comparison operator
   *
   * @param[in] v Vector3 to compare with
   * @return true if vectors are equal (distance less than epsilon), false
   * otherwise
   */
  bool operator==(const Vector3& v) const;

  /**
   * @brief Array subscript operator (mutable)
   *
   * @param[in] i Index (0=x, 1=y, 2=z)
   * @return Reference to the component at specified index
   * @throws std::out_of_range if index is not in the range [0,2]
   */
  double& operator[](int i);

  /**
   * @brief Array subscript operator (const)
   *
   * @param[in] i Index (0=x, 1=y, 2=z)
   * @return Value of the component at specified index
   * @throws std::out_of_range if index is not in the range [0,2]
   */
  double operator[](int i) const;

  /**
   * @brief Overrides + operator.
   * @param[in] v The Vector3 object you would like to add to this Vector3
   * object
   * @return The Vector3 Object comprised of the sum of the two objects
   */
  Vector3 operator+(const Vector3& v) const;

  /**
   * @brief Overrides - operator.
   * @param[in] v The Vector3 object you would like to subtract to this Vector3
   * object
   * @return The Vector3 Object comprised of the subtraction of the two objects
   */
  Vector3 operator-(const Vector3& v) const;

  /**
   * @brief Overrides * operator.
   * @param s type double to mutiply vector by
   * @return The Vector3 Object comprised of the multiplication of the two
   * objects
   */
  Vector3 operator*(double s) const;
  /**
   * @brief Overrides / operator.
   * @param s double type s to divide vector by
   * @return The Vector3 Object comprised of the division of the two objects
   */
  Vector3 operator/(double s) const;
  double operator*(const Vector3& v) const;  // dot product
  // return std::vector version of this Vector3
  // template function should be defined in same file
  // with template keyword
  template <class T>
  std::vector<T> vec() const {
    return {static_cast<T>(x), static_cast<T>(y), static_cast<T>(z)};
  }

  /**
   * @brief Compute cross product with another vector
   *
   * @param[in] v Vector3 to compute cross product with
   * @return A new Vector3 representing the cross product
   */
  Vector3 cross(const Vector3& v) const;

  /**
   * @brief Calculate the magnitude (length) of the vector
   *
   * @return The magnitude of the vector
   */
  double magnitude() const;

  /**
   * @brief Normalize this vector in place
   *
   * Modifies this vector to have a magnitude of 1 while preserving direction
   *
   * @return Reference to this vector after normalization
   */
  Vector3& normalize();

  /**
   * @brief Get a unit vector in the same direction
   *
   * @return A new Vector3 with magnitude 1 in the same direction
   */
  Vector3 unit() const;  // normal vector in same direction

  /**
   * @brief Calculate Euclidean distance to another vector
   *
   * @param[in] v Vector3 to calculate distance to
   * @return The Euclidean distance between this vector and v
   */

  double dist(const Vector3& v) const;

  /**
   * @brief Output stream operator
   *
   * @param[in,out] strm Output stream to write to
   * @param[in] v Vector3 to output
   * @return Reference to the output stream
   */
  friend std::ostream& operator<<(std::ostream& strm, const Vector3& v);
};

#endif
