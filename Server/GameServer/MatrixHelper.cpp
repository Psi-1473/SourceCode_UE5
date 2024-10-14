#include "pch.h"
#include "MatrixHelper.h"

#include <numbers>
#include <cmath>

array<array<double, 4>, 4> MatrixHelper::GetRotationMatrixRoll(double radians)
{
    double c = cos(radians); // 코사인 값
    double s = sin(radians); // 사인 값
    //s = (radians > 0) ? (s * -1) : s;
    return { {
        {1, 0, 0, 0},
        {0, c, -s, 0},
        {0, s, c, 0},
        {0, 0, 0, 1}
    } };
}

array<array<double, 4>, 4> MatrixHelper::GetRotationMatrixPitch(double radians)
{
    double c = cos(radians); // 코사인 값
    double s = sin(radians); // 사인 값
    //s = (radians > 0) ? (s * -1) : s;
    return { {
        {c, 0, -s, 0},
        {0, 1, 0, 0},
        {s, 0, c, 0},
        {0, 0, 0, 1}
    } };
}

array<array<double, 4>, 4> MatrixHelper::GetRotationMatrixYaw(double radians)
{
    double c = cos(radians); // 코사인 값
    double s = sin(radians); // 사인 값
    //s = (radians > 0) ? (s * -1) : s;
    return { {
        {c, -s, 0, 0},
        {s, c, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    } };
}

array<array<double, 4>, 4> MatrixHelper::GetRotationMatrix(const Vec3& rotation)
{
    double xRadians = GetRadians(rotation.x);
    double yRadians = GetRadians(rotation.y);
    double zRadians = GetRadians(rotation.z);

    array<array<double, 4>, 4> rotMatrixRoll = GetRotationMatrixRoll(xRadians);
    array<array<double, 4>, 4> rotMatrixPitch = GetRotationMatrixPitch(yRadians);
    array<array<double, 4>, 4> rotMatrixYaw = GetRotationMatrixYaw(zRadians);
    array<array<double, 4>, 4> rotMatrixZY = MultiplyMatrix(rotMatrixYaw, rotMatrixPitch);
    array<array<double, 4>, 4> rotMatrix = MultiplyMatrix(rotMatrixZY, rotMatrixRoll);


    return rotMatrix;
}



array<array<double, 4>, 4> MatrixHelper::MultiplyMatrix(const array<array<double, 4>, 4>& A, const array<array<double, 4>, 4>& B)
{
    array<array<double, 4>, 4> result = {};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

Vec3 MatrixHelper::ApplyMatrix(const std::array<std::array<double, 4>, 4>& matrix, const Vec3& vec) {
    std::array<double, 4> vec4 = { vec.x, vec.y, vec.z, 1.0f };
    std::array<double, 4> result = {};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i] += matrix[i][j] * vec4[j];
        }
    }
    return Vec3(result[0], result[1], result[2]);
}

Vec3 MatrixHelper::GetYawFromDirection(const Vec3 direction)
{
    std::cout << "Dir = " << direction.x << ", " << direction.y << std::endl;
    float yaw = std::atan2(direction.y, direction.x) * (180.f / std::numbers::pi);
    std::cout << "Yaw : " << yaw << std::endl;
    return Vec3(0, 0, yaw);
}

array<array<double, 4>, 4> MatrixHelper::Transpose(const array<array<double, 4>, 4>& matrix)
{
    array<array<double, 4>, 4> transposed = {}; // 결과 행렬 초기화

    // 행과 열을 바꾸어가며 전치 행렬을 계산
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            transposed[i][j] = matrix[j][i];
        }
    }

    return transposed;
}

Vec3 MatrixHelper::GetForwardVector(const Vec3& rotation)
{
    double radX = GetRadians(rotation.x);
    double radY = GetRadians(rotation.y);
    double radZ = GetRadians(rotation.z);

    double cosZ = std::cos(radZ);
    double sinZ = std::sin(radZ);
    double cosY = std::cos(radY);
    double sinY = std::sin(radY);
    double cosX = std::cos(radX);
    double sinX = std::sin(radX);

    // 전방 벡터 계산
    double forwardX = cosY * cosZ;
    double forwardY = cosY * sinZ;
    double forwardZ = sinY;

    return Vec3(forwardX, forwardY, forwardZ);
}

Vec3 MatrixHelper::GetRightVector(const Vec3& rotation)
{
    double radX = GetRadians(rotation.x);
    double radY = GetRadians(rotation.y);
    double radZ = GetRadians(rotation.z);

    double cosZ = std::cos(radZ);
    double sinZ = std::sin(radZ);
    double cosY = std::cos(radY);
    double sinY = std::sin(radY);
    double cosX = std::cos(radX);
    double sinX = std::sin(radX);

    // Right 벡터 계산
    double rightX = cosZ * cosY;
    double rightY = cosZ * sinY * sinX - sinZ * cosX;
    double rightZ = cosZ * sinY * cosX + sinZ * sinX;

    return Vec3(rightX, rightY, rightZ);
}

Vec3 MatrixHelper::GetUpVector(const Vec3& rotation)
{
    double radX = GetRadians(rotation.x);
    double radY = GetRadians(rotation.y);
    double radZ = GetRadians(rotation.z);

    double cosZ = std::cos(radZ);
    double sinZ = std::sin(radZ);
    double cosY = std::cos(radY);
    double sinY = std::sin(radY);
    double cosX = std::cos(radX);
    double sinX = std::sin(radX);

    // Up 벡터 계산
    double upX = sinZ * cosY;
    double upY = sinZ * sinY * sinX + cosZ * cosX;
    double upZ = sinZ * sinY * cosX - cosZ * sinX;

    return Vec3(upX, upY, upZ);
}

double MatrixHelper::GetRadians(double angle)
{
	return angle * (std::numbers::pi / 180.0f);
}
