#include <stddef.h>

#include "point_cloud.h"
#include "rl_utils.h"


Mesh GenMeshFromFileData(const unsigned char* fileData, unsigned int fileSize, ViewerState* viewerState) {
	int i;
	int pointCount;
	unsigned int availablePoints;
	Mesh mesh;
	Color color;
	Vector3 minPoint;
	Vector3 maxPoint;

	i = 0;
	pointCount = 0;
	availablePoints = 0;
	color = BLACK;
	mesh = (Mesh){ 0 };
	minPoint = Vector3Zero();
	maxPoint = Vector3Zero();

	if ((fileData == NULL) || (fileSize < 3U)) {
		if (viewerState != NULL) {
			viewerState->pointCloudInfo.radius = 1.0f;
			viewerState->pointCloudInfo.pointCount = 0;
			viewerState->pointCloudInfo.drawOffset = Vector3Zero();
		}
		return mesh;
	}

	availablePoints = fileSize - 2U;
	pointCount = (availablePoints > POINT_CLOUD_MAX_POINTS) ? POINT_CLOUD_MAX_POINTS : (int)availablePoints;

	mesh.triangleCount = 1;
	mesh.vertexCount = pointCount;
	mesh.vertices = (float*)MemAlloc((size_t)pointCount * 3U * sizeof(float));
	mesh.colors = (unsigned char*)MemAlloc((size_t)pointCount * 4U * sizeof(unsigned char));

	if ((mesh.vertices == NULL) || (mesh.colors == NULL)) {
		if (mesh.vertices != NULL) MemFree(mesh.vertices);
		if (mesh.colors != NULL) MemFree(mesh.colors);

		mesh.vertexCount = 0;
		mesh.triangleCount = 0;
		mesh.vertices = NULL;
		mesh.colors = NULL;

		if (viewerState != NULL) {
			viewerState->pointCloudInfo.radius = 1.0f;
			viewerState->pointCloudInfo.pointCount = 0;
			viewerState->pointCloudInfo.drawOffset = Vector3Zero();
		}

		return mesh;
	}

	maxPoint = Vector3Make((float)fileData[0], (float)fileData[1], (float)fileData[2]);
	minPoint = Vector3Make((float)fileData[0], (float)fileData[1], (float)fileData[2]);

	for (i = 0; i < pointCount; i++) {
		float x;
		float y;
		float z;

		x = (float)fileData[i];
		y = (float)fileData[i + 1];
		z = (float)fileData[i + 2];
		color = GetByteViewColor(fileData, fileSize, (unsigned int)i, viewerState->byteViewColorType);

		mesh.vertices[i * 3 + 0] = x;
		mesh.vertices[i * 3 + 1] = y;
		mesh.vertices[i * 3 + 2] = z;

		mesh.colors[i * 4 + 0] = color.r;
		mesh.colors[i * 4 + 1] = color.g;
		mesh.colors[i * 4 + 2] = color.b;
		mesh.colors[i * 4 + 3] = color.a;

		if (x < minPoint.x) minPoint.x = x;
		if (y < minPoint.y) minPoint.y = y;
		if (z < minPoint.z) minPoint.z = z;
		if (x > maxPoint.x) maxPoint.x = x;
		if (y > maxPoint.y) maxPoint.y = y;
		if (z > maxPoint.z) maxPoint.z = z;
	}

	if (viewerState != NULL) {
		Vector3 size;
		Vector3 center;
		Vector3 drawOffset;
		float radius;

		center = Vector3Scale(Vector3Add(maxPoint, minPoint), 0.5f);
		size = Vector3Subtract(maxPoint, minPoint);
		radius = Vector3Length(size) * 0.5f;

		if (radius < 1.0f) radius = 1.0f;

		drawOffset = Vector3Make(-center.x, -center.y, -center.z);

		viewerState->pointCloudOffset = drawOffset;
		viewerState->pointCloudInfo.radius = radius;
		viewerState->pointCloudInfo.pointCount = pointCount;
		viewerState->pointCloudInfo.drawOffset = drawOffset;
	}

	UploadMesh(&mesh, 0);

	return mesh;
}

void FitCameraToPointCloud(Camera* camera, float radius) {
	float safeRadius;

	safeRadius = radius;

	if (safeRadius < PI) safeRadius = PI;

	camera->target = Vector3Zero();
	camera->position = Vector3Make(safeRadius * PI / 2.0f, safeRadius, safeRadius * PI / 2.0f);
}

void DrawModelPoints(Model model, Vector3 position, float scale, Color tint) {
	rlEnablePointMode();
	rlDisableBackfaceCulling();

	DrawModel(model, position, scale, tint);

	rlEnableBackfaceCulling();
	rlDisablePointMode();
}
