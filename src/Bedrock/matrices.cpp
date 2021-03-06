#include <Bedrock/matrices.h>

#include <cmath>

namespace perfectpixel {
namespace bedrock {

	const Matrix2x2 Matrix2x2::IDENTITY = Matrix2x2::generateIdentity();
	const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3::generateIdentity();
	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4::generateIdentity();


	perfectpixel::bedrock::Matrix2x2 Matrix2x2::rotate(Angle angle)
	{
		float cosine = std::cos(angle.radians());
		float sine = std::sin(angle.radians());
		return Matrix<2, 2>(std::array<float, 4>{
			cosine,		sine, // C1
			-sine,		cosine, // C2
		});
	}


}
}