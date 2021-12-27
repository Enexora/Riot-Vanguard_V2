#pragma once

struct Vector3 {
	float x;
	float y;
	float z;
	Vector3 operator+=(Vector3 inc) {
		Vector3 clone = { x, y, z };
		clone.x += inc.x;
		clone.y += inc.y;
		clone.z += inc.z;
		return clone;
	}
	Vector3 operator/=(float fl) {
		Vector3 clone = { x,y,z };
		clone.x / fl;
		clone.y / fl;
		clone.z / fl;
		return clone;
	}
};

struct QAngle {
	float pitch;
	float yaw;
	float roll;
};

class VMatrix
{
public:

	VMatrix();
	VMatrix(
		Vector3 m00, Vector3 m01, Vector3 m02, Vector3 m03,
		Vector3 m10, Vector3 m11, Vector3 m12, Vector3 m13,
		Vector3 m20, Vector3 m21, Vector3 m22, Vector3 m23,
		Vector3 m30, Vector3 m31, Vector3 m32, Vector3 m33
	);

	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	VMatrix(const Vector3& forward, const Vector3& left, const Vector3& up);

	// Construct from a 3x4 matrix
	//explicit VMatrix(const matrix3x4_t& matrix3x4);

	// Set the values in the matrix.
	void		Init(
		Vector3 m00, Vector3 m01, Vector3 m02, Vector3 m03,
		Vector3 m10, Vector3 m11, Vector3 m12, Vector3 m13,
		Vector3 m20, Vector3 m21, Vector3 m22, Vector3 m23,
		Vector3 m30, Vector3 m31, Vector3 m32, Vector3 m33
	);


	// Initialize from a 3x4
	//void		Init(const matrix3x4_t& matrix3x4);

	// array access
	inline float* operator[](int i)
	{
		return (float*)m[i];
	}

	inline const float* operator[](int i) const
	{
		return (float*)m[i];
	}

	// Get a pointer to m[0][0]
	inline float* Base()
	{
		return (float*)&m[0][0];
	}

	inline const float* Base() const
	{
		return (float*)&m[0][0];
	}

	void		SetLeft(const Vector3& vLeft);
	void		SetUp(const Vector3& vUp);
	void		SetForward(const Vector3& vForward);

	void		GetBasisVectors(Vector3& vForward, Vector3& vLeft, Vector3& vUp) const;
	void		SetBasisVectors(const Vector3& vForward, const Vector3& vLeft, const Vector3& vUp);

	// Get/set the translation.
	Vector3& GetTranslation(Vector3& vTrans) const;
	void		SetTranslation(const Vector3& vTrans);

	void		PreTranslate(const Vector3& vTrans);
	void		PostTranslate(const Vector3& vTrans);

	//matrix3x4_t& As3x4();
	//const matrix3x4_t& As3x4() const;
	//void		CopyFrom3x4(const matrix3x4_t& m3x4);
	//void		Set3x4(matrix3x4_t& matrix3x4) const;

	bool		operator==(const VMatrix& src) const;
	bool		operator!=(const VMatrix& src) const { return !(*this == src); }

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// Access the basis vectors.
	Vector3		GetLeft() const;
	Vector3		GetUp() const;
	Vector3		GetForward() const;
	Vector3		GetTranslation() const;
#endif


	// Matrix->vector operations.
public:
	// Multiply by a 3D vector (same as operator*).
	void		V3Mul(const Vector3& vIn, Vector3& vOut) const;

	// Multiply by a 4D vector.
	//void		V4Mul(const Vector4D& vIn, Vector4D& vOut) const;

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
	Vector3		ApplyRotation(const Vector3& vVec) const;

	// Multiply by a vector (divides by w, assumes input w is 1).
	Vector3		operator*(const Vector3& vVec) const;

	// Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
	Vector3		VMul3x3(const Vector3& vVec) const;

	// Apply the inverse (transposed) rotation (only works on pure rotation matrix)
	Vector3		VMul3x3Transpose(const Vector3& vVec) const;

	// Multiply by the upper 3 rows.
	Vector3		VMul4x3(const Vector3& vVec) const;

	// Apply the inverse (transposed) transformation (only works on pure rotation/translation)
	Vector3		VMul4x3Transpose(const Vector3& vVec) const;
#endif


	// Matrix->plane operations.
public:
	// Transform the plane. The matrix can only contain translation and rotation.
	//void		TransformPlane(const VPlane& inPlane, VPlane& outPlane) const;

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// Just calls TransformPlane and returns the result.
	//VPlane		operator*(const VPlane& thePlane) const;
#endif

	// Matrix->matrix operations.
public:

	VMatrix& operator=(const VMatrix& mOther);

	// Multiply two matrices (out = this * vm).
	void		MatrixMul(const VMatrix& vm, VMatrix& out) const;

	// Add two matrices.
	const VMatrix& operator+=(const VMatrix& other);

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// Just calls MatrixMul and returns the result.	
	VMatrix		operator*(const VMatrix& mOther) const;

	// Add/Subtract two matrices.
	VMatrix		operator+(const VMatrix& other) const;
	VMatrix		operator-(const VMatrix& other) const;

	// Negation.
	VMatrix		operator-() const;

	// Return inverse matrix. Be careful because the results are undefined 
	// if the matrix doesn't have an inverse (ie: InverseGeneral returns false).
	VMatrix		operator~() const;
#endif

	// Matrix operations.
public:
	// Set to identity.
	void		Identity();

	bool		IsIdentity() const;

	// Setup a matrix for origin and angles.
	void		SetupMatrixOrgAngles(const Vector3& origin, const QAngle& vAngles);

	// General inverse. This may fail so check the return!
	bool		InverseGeneral(VMatrix& vInverse) const;

	// Does a fast inverse, assuming the matrix only contains translation and rotation.
	void		InverseTR(VMatrix& mRet) const;

	// Usually used for debug checks. Returns true if the upper 3x3 contains
	// unit vectors and they are all orthogonal.
	bool		IsRotationMatrix() const;

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// This calls the other InverseTR and returns the result.
	VMatrix		InverseTR() const;

	// Get the scale of the matrix's basis vectors.
	Vector3		GetScale() const;

	// (Fast) multiply by a scaling matrix setup from vScale.
	VMatrix		Scale(const Vector3& vScale);

	// Normalize the basis vectors.
	VMatrix		NormalizeBasisVectors() const;

	// Transpose.
	VMatrix		Transpose() const;

	// Transpose upper-left 3x3.
	VMatrix		Transpose3x3() const;
#endif

public:
	// The matrix.
	Vector3		m[4][4];
};
