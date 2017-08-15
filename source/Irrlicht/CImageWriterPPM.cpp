// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CImageWriterPPM.h"

#ifdef _IRR_COMPILE_WITH_PPM_WRITER_

#include "IWriteFile.h"
#include "IImage.h"
#include "dimension2d.h"
#include "irrString.h"

namespace irr
{
namespace video
{


IImageWriter* createImageWriterPPM()
{
	return new CImageWriterPPM;
}


CImageWriterPPM::CImageWriterPPM()
{
#ifdef _DEBUG
	setDebugName("CImageWriterPPM");
#endif
}


bool CImageWriterPPM::isAWriteableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension ( filename, "ppm" );
}


bool CImageWriterPPM::writeImage(io::IWriteFile *file, IImage *image, uint32_t param) const
{
	char cache[70];
	int size;

	const core::dimension2d<uint32_t>& imageSize = image->getDimension();

	const bool binary = false;

	if (binary)
		size = snprintf(cache, 70, "P6\n");
	else
		size = snprintf(cache, 70, "P3\n");

	if (file->write(cache, size) != size)
		return false;

	size = snprintf(cache, 70, "%d %d\n", imageSize.Width, imageSize.Height);
	if (file->write(cache, size) != size)
		return false;

	size = snprintf(cache, 70, "255\n");
	if (file->write(cache, size) != size)
		return false;

	if (binary)
	{
		for (uint32_t h = 0; h < imageSize.Height; ++h)
		{
			for (uint32_t c = 0; c < imageSize.Width; ++c)
			{
				const video::SColor& pixel = image->getPixel(c, h);
				const uint8_t r = (uint8_t)(pixel.getRed() & 0xff);
				const uint8_t g = (uint8_t)(pixel.getGreen() & 0xff);
				const uint8_t b = (uint8_t)(pixel.getBlue() & 0xff);
				file->write(&r, 1);
				file->write(&g, 1);
				file->write(&b, 1);
			}
		}
	}
	else
	{
		int32_t n = 0;

		for (uint32_t h = 0; h < imageSize.Height; ++h)
		{
			for (uint32_t c = 0; c < imageSize.Width; ++c, ++n)
			{
				const video::SColor& pixel = image->getPixel(c, h);
				size = snprintf(cache, 70, "%.3u %.3u %.3u%s", pixel.getRed(), pixel.getGreen(), pixel.getBlue(), n % 5 == 4 ? "\n" : "  ");
				if (file->write(cache, size) != size)
					return false;
			}
		}
	}

	return true;
}


} // namespace video
} // namespace irr

#endif

