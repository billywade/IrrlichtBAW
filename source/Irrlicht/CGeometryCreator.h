// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_GEOMETRY_CREATOR_H_INCLUDED__
#define __C_GEOMETRY_CREATOR_H_INCLUDED__

#include "IGeometryCreator.h"

namespace irr
{

namespace scene
{

//! class for creating geometry on the fly
class CGeometryCreator : public IGeometryCreator
{
private:
#include "irr/irrpack.h"
	struct CubeVertex
	{
		float pos[3];
		uint8_t color[4]; // normalized
		uint8_t uv[2];
		int8_t normal[3];
		uint8_t dummy[3];

		void setPos(float x, float y, float z) { pos[0] = x; pos[1] = y; pos[2] = z; }
		void translate(float dx, float dy, float dz) { pos[0] += dx; pos[1] += dy; pos[2] += dz; }
		void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { color[0] = r; color[1] = g; color[2] = b; color[3] = a; }
		void setNormal(int8_t x, int8_t y, int8_t z) { normal[0] = x; normal[1] = y; normal[2] = z; }
		void setUv(uint8_t u, uint8_t v) { uv[0] = u; uv[1] = v; }
	} PACK_STRUCT;

    struct ConeVertex
    {
        inline ConeVertex(const core::vectorSIMDf& _pos, uint32_t _nml, const video::SColor& _color) : normal{_nml}
        {
            memcpy(pos, _pos.pointer, 12);
            _color.toOpenGLColor(color);
        }

        float pos[3];
        uint8_t color[4];
        uint32_t normal;
    } PACK_STRUCT;

    struct CylinderVertex
    {
        CylinderVertex() : pos{0.f, 0.f, 0.f}, color{0u, 0u, 0u, 0u}, uv{0.f, 0.f}, normal{0u} {}

        float pos[3];
        uint8_t color[4];
        float uv[2];
        uint32_t normal;
    } PACK_STRUCT;
#include "irr/irrunpack.h"

public:
	ICPUMesh* createCubeMeshCPU(const core::vector3df& size) const;
	IGPUMesh* createCubeMeshGPU(video::IVideoDriver* driver, const core::vector3df& size) const;

	ICPUMesh* createArrowMeshCPU(const uint32_t tesselationCylinder,
			const uint32_t tesselationCone, const float height,
			const float cylinderHeight, const float width0,
			const float width1, const video::SColor vtxColor0,
			const video::SColor vtxColor1) const;
	IGPUMesh* createArrowMeshGPU(video::IVideoDriver* driver, const uint32_t tesselationCylinder,
			const uint32_t tesselationCone, const float height,
			const float cylinderHeight, const float width0,
			const float width1, const video::SColor vtxColor0,
			const video::SColor vtxColor1) const;

	ICPUMesh* createSphereMeshCPU(float radius, uint32_t polyCountX, uint32_t polyCountY) const;
	IGPUMesh* createSphereMeshGPU(video::IVideoDriver* driver, float radius, uint32_t polyCountX, uint32_t polyCountY) const;

	ICPUMesh* createCylinderMeshCPU(float radius, float length, uint32_t tesselation,
				const video::SColor& color=0xffffffff,
				bool closeTop=true, float oblique=0.f) const;
	IGPUMesh* createCylinderMeshGPU(video::IVideoDriver* driver, float radius, float length, uint32_t tesselation,
				const video::SColor& color=0xffffffff,
				bool closeTop=true, float oblique=0.f) const;

	ICPUMesh* createConeMeshCPU(float radius, float length, uint32_t tesselation,
				const video::SColor& colorTop=0xffffffff,
				const video::SColor& colorBottom=0xffffffff,
				float oblique=0.f) const;
	IGPUMesh* createConeMeshGPU(video::IVideoDriver* driver, float radius, float length, uint32_t tesselation,
				const video::SColor& colorTop=0xffffffff,
				const video::SColor& colorBottom=0xffffffff,
				float oblique=0.f) const;

};

} // end namespace scene
} // end namespace irr

#endif

