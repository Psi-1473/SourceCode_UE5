#pragma once
#include <array>
#include "Collider.h"

using namespace std;

/*
	회전 행렬과 관련된 기능을 제공하는 유틸리티 클래스
	나중에 이름 바꾸자
*/
class MatrixHelper
{
public:
	static array<array<double, 4>, 4> GetRotationMatrixRoll(double radians);
	static array<array<double, 4>, 4> GetRotationMatrixPitch(double radians);
	static array<array<double, 4>, 4> GetRotationMatrixYaw(double radians);
	static array<array<double, 4>, 4> GetRotationMatrix(const Vec3& rotation);
	static array<array<double, 4>, 4> MultiplyMatrix(const array<array<double, 4>, 4>& A, const array<array<double, 4>, 4>& B);
	static Vec3 ApplyMatrix(const std::array<std::array<double, 4>, 4>& matrix, const Vec3& vec);
	static Vec3 GetYawFromDirection(const Vec3 direction);

	static  array<array<double, 4>, 4> Transpose(const array<array<double, 4>, 4>& matrix);
	static Vec3 GetForwardVector(const Vec3& rotation);
	static Vec3 GetRightVector(const Vec3& rotation);
	static Vec3 GetUpVector(const Vec3& rotation);
	static double GetRadians(double angle);
};
