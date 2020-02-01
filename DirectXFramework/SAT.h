#pragma once

#include "PhysicsMath.h"
#include "OBB.h"

#include <limits>
#include <memory>
#include <assert.h>

struct Contact {
public:
	Vecf3 position_;
	float penetration_;
	float bias_;
	float constraint_mass_;
};

struct Manifold {
public:
	std::shared_ptr<OBB> A;
	std::shared_ptr<OBB> B;
	Vecf3 normal_;
	Contact contacts_[16];
	int contact_count_;
};

class SAT {
public:
	static bool OBBToOBB(Manifold& m, std::shared_ptr<OBB> A, std::shared_ptr<OBB> B) {
		PMat3 A_rotation = A->transform_.rotation_;
		PMat3 B_rotation = B->transform_.rotation_;

		Vecf3 A_extents = A->extents_;
		Vecf3 B_extents = B->extents_;

		// rotation to represent B in A's coordinate frame C = A^T * B
		// C in qu3e
		PMat3 B_to_A_frame = A_rotation.Transpose() * B_rotation;

		// absC in qu3e
		PMat3 Abs_B_to_A_frame = B_to_A_frame.Abs();

		// vector from A to B center in A space
		// t in qu3e
		Vecf3 vec_AB_in_A_frame = A_rotation.Transpose() * (B->transform_.position_ - A->transform_.position_);

		// query states
		float s;
		float aMax = -std::numeric_limits<float>::max();
		float bMax = -std::numeric_limits<float>::max();
		int aAxis = -1;
		int bAxis = -1;
		Vecf3 normalA = { 0.0f, 0.0f, 0.0f };
		Vecf3 normalB = { 0.0f, 0.0f, 0.0f };
		
		// face axis checks
		s = std::abs(vec_AB_in_A_frame.x) - (A_extents.x + (Abs_B_to_A_frame.GetColumn(0) * B_extents));
		if (FaceAxis(aAxis, 0, s, aMax, A_rotation.x_, normalA)) {
			return false;
		}

		s = std::abs(vec_AB_in_A_frame.y) - (A_extents.y + (Abs_B_to_A_frame.GetColumn(1) * B_extents));
		if (FaceAxis(aAxis, 1, s, aMax, A_rotation.y_, normalA)) {
			return false;
		}

		s = std::abs(vec_AB_in_A_frame.z) - (A_extents.z + (Abs_B_to_A_frame.GetColumn(2) * B_extents));
		if (FaceAxis(aAxis, 2, s, aMax, A_rotation.z_, normalA)) {
			return false;
		}

		s = std::abs(vec_AB_in_A_frame * B_to_A_frame.x_) - (B_extents.x + (Abs_B_to_A_frame.x_ * A_extents));
		if (FaceAxis(bAxis, 3, s, bMax, B_rotation.x_, normalB)) {
			return false;
		}

		s = std::abs(vec_AB_in_A_frame * B_to_A_frame.y_) - (B_extents.y + (Abs_B_to_A_frame.y_ * A_extents));
		if (FaceAxis(bAxis, 4, s, bMax, B_rotation.y_, normalB)) {
			return false;
		}

		s = std::abs(vec_AB_in_A_frame * B_to_A_frame.z_) - (B_extents.z + (Abs_B_to_A_frame.z_ * A_extents));
		if (FaceAxis(bAxis, 5, s, bMax, B_rotation.z_, normalB)) {
			return false;
		}
		
		// get max
		int axis;
		Vecf3 normal = { 0.0f, 0.0f, 0.0f };

		if (bMax > aMax) {
			axis = bAxis;
		}
		else {
			axis = aAxis;
			normal = normalA;
		}

		// check position for inverse normal
		if ((normal * (B->transform_.position_ - A->transform_.position_)) < 0.0f) {
			normal = -normal;
		}

		assert(axis != -1);

		// calculate reference and incidence face
		Transform reference_transform;
		Transform incidence_transform;
		Vecf3 reference_extent;
		Vecf3 incidence_extent;
		bool flip;
		
		if (axis < 3) {
			reference_transform = A->transform_;
			incidence_transform = B->transform_;
			reference_extent = A_extents;
			incidence_extent = B_extents;
			flip = false;
		}
		else {
			reference_transform = B->transform_;
			incidence_transform = A->transform_;
			reference_extent = B_extents;
			incidence_extent = A_extents;
			flip = true;
			normal = -normal;
		}

		Vecf3 vertices[4] = {};
		ComputeIncidentFace(incidence_transform, incidence_extent, normal, vertices);
		int clip_edges[4] = {};
		PMat3 basis = PMat3();
		Vecf3 extent = { 0.0f, 0.0f, 0.0f };
		ComputeReferenceEdgesAndBasis(reference_extent, reference_transform, normal, axis, clip_edges, basis, extent);

		Vecf3 vertices_out[16] = {};
		float depths[16] = {};
		int outNum;
		outNum = Clip(reference_transform.position_, extent, clip_edges, basis, vertices, vertices_out, depths);

		if (outNum > 0) {
			m.A = A;
			m.B = B;
			m.contact_count_ = outNum;
			m.normal_ = flip ? -normal : normal;

			for (int i = 0; i < outNum; i++) {
				Contact c = Contact();
				c.position_ = vertices_out[i];
				c.penetration_ = depths[i];
				m.contacts_[i] = c;
			}
			return true;
		}
		else {
			return false;
		}
	}

	static bool FaceAxis(int& controlaxis, const int& axisindex, const float& s, float& sMax, const Vecf3& normal, Vecf3& axisnormal) {
		if (s > 0.0f) {
			return true;
		}
		if (s > sMax) {
			sMax = s;
			controlaxis = axisindex;
			axisnormal = normal;
		}
		return false;
	}

	static void ComputeIncidentFace(Transform& incidencetransform, const Vecf3& extent, const Vecf3& normal, Vecf3* vertices) {
		Vecf3 rotatedRN = -(incidencetransform.rotation_.Transpose() * normal);
		Vecf3 absRN = PhysicsMath::AbsVec(rotatedRN);

		if (absRN.x > absRN.y && absRN.x > absRN.z) {
			if (rotatedRN.x > 0.0f) {
				vertices[0] = { extent.x,  extent.y, -extent.z };
				vertices[1] = { extent.x,  extent.y,  extent.z };
				vertices[2] = { extent.x, -extent.y,  extent.z };
				vertices[3] = { extent.x, -extent.y, -extent.z };
			}
			else {
				vertices[0] = { -extent.x, -extent.y,  extent.z };
				vertices[1] = { -extent.x,  extent.y,  extent.z };
				vertices[2] = { -extent.x,  extent.y, -extent.z };
				vertices[3] = { -extent.x, -extent.y, -extent.z };
			}
		}
		else if (absRN.y > absRN.x && absRN.y > absRN.z) {
			if (rotatedRN.y > 0.0f) {
				vertices[0] = { -extent.x,  extent.y,  extent.z };
				vertices[1] = {  extent.x,  extent.y,  extent.z };
				vertices[2] = {  extent.x,  extent.y, -extent.z };
				vertices[3] = { -extent.x,  extent.y, -extent.z };
			}
			else {
				vertices[0] = {  extent.x, -extent.y,  extent.z };
				vertices[1] = { -extent.x, -extent.y,  extent.z };
				vertices[2] = { -extent.x, -extent.y, -extent.z };
				vertices[3] = {  extent.x, -extent.y, -extent.z };
			}
		}
		else {
			if (rotatedRN.z > 0.0f) {
				vertices[0] = { -extent.x,  extent.y,  extent.z };
				vertices[1] = { -extent.x, -extent.y,  extent.z };
				vertices[2] = {  extent.x, -extent.y,  extent.z };
				vertices[3] = {  extent.x,  extent.y,  extent.z };
			}
			else {
				vertices[0] = {  extent.x, -extent.y, -extent.z };
				vertices[1] = { -extent.x, -extent.y, -extent.z };
				vertices[2] = { -extent.x,  extent.y, -extent.z };
				vertices[3] = {  extent.x,  extent.y, -extent.z };
			}
		}

		for (int i = 0; i < 4; i++) {
			vertices[i] = incidencetransform * vertices[i];
		}
	}

	static void ComputeReferenceEdgesAndBasis(const Vecf3& referenceextent, Transform& referencetransform, const Vecf3& normal, int axis, int* output, PMat3& basis, Vecf3& extent) {
		Vecf3 rotatedRN = referencetransform.rotation_.Transpose() * normal;

		if (axis >= 3) {
			axis -= 3;
		}
		
		switch (axis) {
		case 0:
			if (rotatedRN.x >= 0.0f) {
				output[0] = 1;
				output[1] = 8;
				output[2] = 7;
				output[3] = 9;

				extent = { referenceextent.x, referenceextent.z, referenceextent.z };
				basis.x_ = referencetransform.rotation_.y_;
				basis.y_ = referencetransform.rotation_.z_;
				basis.z_ = referencetransform.rotation_.x_;
			}
			else {
				output[0] = 11;
				output[1] = 3;
				output[2] = 10;
				output[3] = 5;

				extent = { referenceextent.z, referenceextent.y, referenceextent.x };
				basis.x_ = referencetransform.rotation_.z_;
				basis.y_ = referencetransform.rotation_.y_;
				basis.z_ = referencetransform.rotation_.x_;
			}
			break;
		case 1:
			if (rotatedRN.y >= 0.0f) {
				output[0] = 0;
				output[1] = 1;
				output[2] = 2;
				output[3] = 3;

				extent = { referenceextent.z, referenceextent.x, referenceextent.y };
				basis.x_ = referencetransform.rotation_.z_;
				basis.y_ = referencetransform.rotation_.x_;
				basis.z_ = referencetransform.rotation_.y_;
			}
			else {
				output[0] = 4;
				output[1] = 5;
				output[2] = 6;
				output[3] = 7;

				extent = { referenceextent.z, referenceextent.x, referenceextent.y };
				basis.x_ = referencetransform.rotation_.z_;
				basis.y_ = referencetransform.rotation_.x_;
				basis.z_ = referencetransform.rotation_.y_;
			}
			break;
		case 2:
			if (rotatedRN.z >= 0.0f) {
				output[0] = 11;
				output[1] = 4;
				output[2] = 8;
				output[3] = 0;

				extent = { referenceextent.y, referenceextent.x, referenceextent.z };
				basis.x_ = referencetransform.rotation_.y_;
				basis.y_ = referencetransform.rotation_.x_;
				basis.z_ = referencetransform.rotation_.z_;
			}
			else {
				output[0] = 6;
				output[1] = 10;
				output[2] = 2;
				output[3] = 9;

				extent = { referenceextent.y, referenceextent.x, referenceextent.z };
				basis.x_ = referencetransform.rotation_.y_;
				basis.y_ = referencetransform.rotation_.x_;
				basis.z_ = referencetransform.rotation_.z_;
			}
			break;
		}
	}

	static bool InFront(const float& a) {
		return a < 0.0f;
	}

	static bool Behind(const float& a) {
		return a >= 0.0f;
	}

	static bool On(const float& a) {
		return (a < 0.005f && a > -0.005f);
	}

	static int Orthographic(const float& sign, const float& extent, int axis, const int& clipedge, Vecf3* vertices, const int& incount, Vecf3* verticesout) {
		int outCount = 0;
		Vecf3 a = vertices[incount - 1];
		for (int i = 0; i < incount; i++) {
			Vecf3 b = vertices[i];

			float da = 0.0f;
			float db = 0.0f;

			switch (axis) {
			case 0:
				da = sign * a.x - extent;
				db = sign * b.x - extent;
				break;
			case 1:
				da = sign * a.y - extent;
				db = sign * b.y - extent;
				break;
			case 2:
				da = sign * a.z - extent;
				db = sign * b.z - extent;
				break;
			}

			Vecf3 cv = {0.0f, 0.0f, 0.0f};

			if ((InFront(da) && InFront(db)) || On(da) || On(db))
			{
				if (!(outCount < 8))
				{
					//Debug.Log("OUTPUT COUNT EXCEED!");
				}
				verticesout[outCount] = b;
				outCount++;
			}
			else if (InFront(da) && Behind(db))
			{
				cv = a + (b - a) * (da / (da - db));
				verticesout[outCount] = cv;
				outCount++;
			}
			else if (Behind(da) && InFront(db))
			{
				cv = a + (b - a) * (da / (da - db));
				verticesout[outCount] = cv;
				outCount++;
				verticesout[outCount] = b;
				outCount++;
			}
		}
		return outCount;
	}

	static int Clip(const Vecf3& reference_position, const Vecf3& extent, int* clipedges, PMat3& basis, Vecf3* vertices, Vecf3* outputvertices, float* outputdepths) {
		int inCount = 4;
		int outCount;
		Vecf3 vertices_in[16] = {};
		Vecf3 vertices_out[16] = {};

		for (int i = 0; i < 4; i++)
		{
			vertices_in[i] = basis.Transpose() * (vertices[i] - reference_position);
		}

		outCount = Orthographic(1.0f, extent.x, 0, clipedges[0], vertices_in, inCount, vertices_out);

		if (outCount == 0)
		{
			return 0;
		}

		inCount = Orthographic(1.0f, extent.y, 1, clipedges[1], vertices_out, outCount, vertices_in);

		if (inCount == 0)
		{
			return 0;
		}

		outCount = Orthographic(-1.0f, extent.x, 0, clipedges[2], vertices_in, inCount, vertices_out);

		if (outCount == 0)
		{
			return 0;
		}

		inCount = Orthographic(-1.0f, extent.y, 1, clipedges[3], vertices_out, outCount, vertices_in);

		outCount = 0;

		for (int i = 0; i < inCount; i++)
		{
			float d = vertices_in[i].z - extent.z;

			if (d <= 0.0f)
			{
				outputvertices[outCount] = basis * (vertices_in[i]) + reference_position;
				outputdepths[outCount] = d;
				outCount++;
			}
		}
		return outCount;
	}
};