#include <stdio.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include "rl_utils.h"
#include "point_cloud.h"
#include "byteview_color.h"

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768


static void LoadPointCloudFromPath(ViewerState* viewerState);


int main(void) {
	FilePathList filePathList;
	ViewerState viewerState;

	viewerState = (ViewerState){ 0 };

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D File Viewer");

	viewerState.camera.fovy = 45.0f;
	viewerState.camera.projection = CAMERA_PERSPECTIVE;
	viewerState.camera.target = Vector3Zero();
	viewerState.camera.up = Vector3Make(0.0f, 1.0f, 0.0f);
	viewerState.camera.position = Vector3Make(15.0f, 10.0f, 15.0f);

	viewerState.pointCloudInfo.radius = 1.0f;
	viewerState.pointCloudInfo.pointCount = 0;
	viewerState.pointCloudOffset = Vector3Zero();
	viewerState.pointCloudInfo.drawOffset = Vector3Zero();

	viewerState.byteViewColorType = BYTEVIEW_COLOR_TYPE_R8;
	TextCopy(viewerState.byteViewColorTypeText, GetByteViewColorTypeName(viewerState.byteViewColorType));

	TextCopy((char*)&viewerState.sourceText, "Source: None");
	TextCopy((char*)&viewerState.pointCountText, "Point Count: 0");
	TextCopy((char*)&viewerState.statusText, "Drop a file into the window to rebuild the cloud");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		UpdateCamera(&viewerState.camera, CAMERA_ORBITAL);

		if (IsFileDropped()) {
			filePathList = LoadDroppedFiles();

			if (filePathList.count > 0) {
				viewerState.droppedFile = _strdup(filePathList.paths[0]);
				LoadPointCloudFromPath(&viewerState);
			}

			UnloadDroppedFiles(filePathList);
		}

		if (IsKeyPressed(KEY_LEFT)) {
			viewerState.byteViewColorType = (ByteViewColorType)(viewerState.byteViewColorType - 1U);
			if (viewerState.byteViewColorType <= BYTEVIEW_COLOR_TYPE_NONE) { viewerState.byteViewColorType = BYTEVIEW_COLOR_TYPE_CLASS; }
			if (viewerState.byteViewColorType >= BYTEVIEW_COLOR_TYPE_COUNT) { viewerState.byteViewColorType = BYTEVIEW_COLOR_TYPE_R8; }
			TextCopy(viewerState.byteViewColorTypeText, GetByteViewColorTypeName(viewerState.byteViewColorType));
			LoadPointCloudFromPath(&viewerState);
		}

		if (IsKeyPressed(KEY_RIGHT)) {
			viewerState.byteViewColorType = (ByteViewColorType)(viewerState.byteViewColorType + 1U);
			if (viewerState.byteViewColorType <= BYTEVIEW_COLOR_TYPE_NONE) { viewerState.byteViewColorType = BYTEVIEW_COLOR_TYPE_CLASS; }
			if (viewerState.byteViewColorType >= BYTEVIEW_COLOR_TYPE_COUNT) { viewerState.byteViewColorType = BYTEVIEW_COLOR_TYPE_R8; }
			TextCopy(viewerState.byteViewColorTypeText, GetByteViewColorTypeName(viewerState.byteViewColorType));
			LoadPointCloudFromPath(&viewerState);
		}

		BeginDrawing();
		{
			ClearBackground(BLACK);

			if (IsModelValid(viewerState.model)) {
				BeginMode3D(viewerState.camera);
				{
					DrawModelPoints(viewerState.model, viewerState.pointCloudOffset, 1.0f, WHITE);
				}
				EndMode3D();
			}

			DrawText("Drop a file into the window", 16, 40, 20, WHITE);
			DrawText(viewerState.statusText, 16, SCREEN_HEIGHT - 36, 24, WHITE);
			DrawText(viewerState.sourceText, 16, SCREEN_HEIGHT - 72, 24, WHITE);
			DrawText(viewerState.pointCountText, 16, SCREEN_HEIGHT - 108, 24, WHITE);
			DrawText(viewerState.byteViewColorTypeText, 16, SCREEN_HEIGHT - 140, 24, WHITE);

			DrawFPS(16, 16);
		}
		EndDrawing();
	}

	UnloadModel(viewerState.model);
	CloseWindow();

	return 0;
}

static void LoadPointCloudFromPath(ViewerState* viewerState) {
	int fileSize;
	int pointCountClamped;
	unsigned int availablePoints;
	unsigned char* fileData;
	Mesh newMesh;

	fileSize = 0;
	availablePoints = 0;
	pointCountClamped = 0;
	fileData = NULL;

	viewerState->pointCloudInfo.radius = 1.0f;
	viewerState->pointCloudInfo.pointCount = 0;
	viewerState->pointCloudInfo.drawOffset = Vector3Zero();

	fileData = LoadFileData(viewerState->droppedFile, &fileSize);

	if (fileData == NULL) {
		TextCopy(viewerState->statusText, "Could not read dropped file");
		return;
	}

	if (fileSize < 3U) {
		TextCopy(viewerState->statusText, "File needs at least 3 bytes to build a point cloud");
		UnloadFileData(fileData);
		return;
	}

	availablePoints = fileSize - 2U;
	pointCountClamped = (availablePoints > POINT_CLOUD_MAX_POINTS);
	newMesh = GenMeshFromFileData(fileData, fileSize, viewerState);

	if ((newMesh.vertices != NULL) && (newMesh.colors != NULL) && (newMesh.vertexCount > 0)) {
		if (IsModelValid(viewerState->model)) {
			UnloadModel(viewerState->model);
		}

		viewerState->mesh = newMesh;
		viewerState->model = LoadModelFromMesh(viewerState->mesh);
		TextCopy(viewerState->sourceText, TextFormat("Source: %s", GetFileName(viewerState->droppedFile)));
		TextCopy(viewerState->pointCountText, TextFormat("Point Count: %d", viewerState->pointCloudInfo.pointCount));

		FitCameraToPointCloud(&viewerState->camera, viewerState->pointCloudInfo.radius);

		if (pointCountClamped) {
			TextCopy(viewerState->statusText, TextFormat("Loaded %u bytes, capped to %d points", fileSize, viewerState->pointCloudInfo.pointCount));
		} else {
			TextCopy(viewerState->statusText, TextFormat("Loaded %u bytes into %d points", fileSize, viewerState->pointCloudInfo.pointCount));
		}
	} else {
		TextCopy(viewerState->statusText, "Not enough memory to build the point cloud");
	}

	UnloadFileData(fileData);
}
