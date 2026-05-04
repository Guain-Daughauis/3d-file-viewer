#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include "byteview_color.h"

#define SOURCE_TEXT_LENGTH 256
#define STATUS_TEXT_LENGTH 256
#define POINT_COUNT_TEXT_LENGTH 256
#define BYTE_VIEW_COLOR_TYPE_TEXT_LENGTH 256
#define POINT_CLOUD_MAX_POINTS 10000000


typedef struct PointCloudInfo {
	int pointCount;
	float radius;
	Vector3 drawOffset;
} PointCloudInfo;

typedef struct ViewerState {
	char sourceText[SOURCE_TEXT_LENGTH];
	char statusText[STATUS_TEXT_LENGTH];
	char pointCountText[POINT_COUNT_TEXT_LENGTH];
	char byteViewColorTypeText[BYTE_VIEW_COLOR_TYPE_TEXT_LENGTH];
	char* droppedFile;
	Mesh mesh;
	Model model;
	Camera camera;
	Vector3 pointCloudOffset;
	PointCloudInfo pointCloudInfo;
	ByteViewColorType byteViewColorType;
} ViewerState;

Mesh GenMeshFromFileData(const unsigned char* fileData, unsigned int fileSize, ViewerState* viewerState);
void FitCameraToPointCloud(Camera* camera, float radius);
void DrawModelPoints(Model model, Vector3 position, float scale, Color tint);

#endif
