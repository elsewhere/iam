#ifndef _IMAGE_CPP_
#define _IMAGE_CPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "image.hpp"
#include "globals.hpp"
#include "png.h"

// T‰n kanssa oli ongelmia
// Linkkeriin piti laittaa optio: /NODEFAULTLIB:LIBC jotta ei tullut konflikteja funkkareiden kanssa
#include "jpeglib.h"

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{
	// Customized functions for loading JPG from a memory
	void initJPEGDataSource(j_decompress_ptr cinfo);
	unsigned char fillJPEGInputBuffer(j_decompress_ptr cinfo);
	void skipJPEGInputData(j_decompress_ptr cinfo, long nBytes);
	unsigned char resyncJPEGToRestart(j_decompress_ptr cinfo, int desired);
	void terminateJPEGSource(j_decompress_ptr cinfo);

	unsigned char *jpegData;
	unsigned int jpegSize;
};

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Class code
//--------------------------------------------------------------------------------------------

Image::Image(void)
{
	width=0;
	height=0;
	imagedata=NULL;	
	jpegData = NULL;
	jpegSize = 0;
}

Image::~Image(void)
{
	width=0;
	height=0;
	if(imagedata)
	{
		delete [] imagedata;
		imagedata = NULL;
	}	
}

//--------------------------------------------------------------------------------------------
//
// "LoadTGA(char *name)
// lataa 24bpp tai 32bpp RLE-pakatun tai pakkaamattoman TGA filen
//
//--------------------------------------------------------------------------------------------

bool Image::LoadTGA(char *name)
{
	FILE *f;
	char buf[256];

    TGAHeader tgaheader;
    TGAData tga;

    // onko tga kuva RLE-pakattu vai ei
    unsigned char uncomp[12] = { 0, 0,  2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    unsigned char compre[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	memset(buf, 0, 255*sizeof(char));

    f = fopen(name,"rb");
    if(!f)
    {
		char buf[512] = {0};
		
		sprintf(buf, "Image::LoadTGA Error: Couldn't open %s\n", name);
		dmsErrorMsg("%s\n", buf);
		return false;
    }
		
    // luetaan tiedoston headeri
    if(fread(&tgaheader, sizeof(TGAHeader), 1, f) == 0)
    {
		char buf[512] = {0};
		
		sprintf(buf, "Image::LoadTGA Error: Couldn't read %s header\n", name);
		dmsErrorMsg("%s\n", buf);
		return false;
    }
			
    // tarkistetaan onko tiedosto sallittu .tga ja pakattu vai ei
    if(memcmp(uncomp, &tgaheader, sizeof(tgaheader)) == 0)
    {		
		// ladataan pakkaamaton TGA-file:
        if(fread(tga.header, sizeof(tga.header), 1, f) == 0)
        {
			char buf[512] = {0};
		
			sprintf(buf, "Image::LoadTGA Error: Couldn't read data from %s\n", name);
			dmsErrorMsg("%s\n", buf);
			return false;
        }

        width = tga.header[1] * 256 + tga.header[0];
        height = tga.header[3] * 256 + tga.header[2];
        bpp = tga.header[4];
        tga.width = width;
        tga.height = height;
        tga.bpp = bpp;	

		// tarkistetaan ett‰ arvot on oikeissa rajoissa
        if(width <= 0 || height <= 0 || (bpp != 24 && bpp != 32))
        {	
			char buf[512] = {0};

			sprintf(buf, "Image::LoadTGA Error: %s is not a valid TGA file\n", name);
			dmsErrorMsg("%s\n", buf);
			return false;
        }

        type = GL_RGBA;
        tga.bytesPerPixel = (tga.bpp / 8);
        tga.imagesize = (tga.width * tga.height * 4);

        imagedata = new unsigned int[tga.imagesize];
        if(!imagedata)
        {
			char buf[512] = {0};

			sprintf(buf, "Image::LoadTGA Error: memory allocation error\n");
			dmsErrorMsg("%s\n", buf);
			return false;
		}
		memset(imagedata, 0, tga.imagesize);

		unsigned int t;
		unsigned char *ptr = (unsigned char *)imagedata;
		for(t = 0; t < width*height; t++)
		{
			unsigned char r, g, b;
			r = fgetc(f);
			g = fgetc(f);
			b = fgetc(f);
			*ptr++ = b;
			*ptr++ = g;
			*ptr++ = r;

			// jos 32bpp kuva
			if(tga.bytesPerPixel == 4)
			{					
				*ptr++ = fgetc(f);				
			}
			else
			{
				*ptr++ = 255;
			}
		}				
	}
    else if(memcmp(compre, &tgaheader, sizeof(tgaheader)) == 0)
    {
		// ladataan RLE-pakattu TGA-file:
		if(fread(tga.header, sizeof(tga.header), 1, f) == 0)
        {
			char buf[512] = {0};

			sprintf(buf, "Image::LoadTGA Error: Couldn't read data from %s\n", name);
			dmsErrorMsg("%s\n", buf);
			return false;
        }

        width = tga.header[1] * 256 + tga.header[0];
        height = tga.header[3] * 256 + tga.header[2];
        bpp = tga.header[4];
        tga.width = width;
        tga.height = height;
        tga.bpp = bpp;
		
		// tarkistetaan ettÑ arvot on oikeissa rajoissa
        if(width <= 0 || height <= 0 || (bpp != 24 && bpp != 32))
        {
			char buf[512] = {0};

			sprintf(buf, "Image::LoadTGA Error: %s is not a valid TGA file\n", name);
			dmsErrorMsg("%s\n", buf);
			return false;
        }

        type = GL_RGBA;
        tga.bytesPerPixel = (tga.bpp / 8);
        tga.imagesize = tga.width * tga.height * tga.bytesPerPixel;				

        imagedata = new unsigned int[tga.imagesize];
        if(!imagedata)
        {
			char buf[512] = {0};

			sprintf(buf, "Image::LoadTGA Error: memory allocation error\n");
			dmsErrorMsg("%s\n", buf);
			return false;
        }
		memset(imagedata, 0, tga.imagesize);

		unsigned int currentPixel = 0;
		unsigned int currentByte = 0;
		unsigned char chunkHeader;
		unsigned char *ptr = (unsigned char *)imagedata;
		unsigned char *colBuf = new unsigned char[tga.bytesPerPixel];

		do
		{					
			if(fread(&chunkHeader, sizeof(chunkHeader), 1, f) == 0)
			{
				char buf[512] = {0};

				sprintf(buf, "Image::LoadTGA Error: Couldn't read data from %s\n", name);
				dmsErrorMsg("%s\n", buf);
				return false;
			}

			// Raaka-data
			if(chunkHeader < 128)
			{					
				chunkHeader++;
				for(int i = 0; i < chunkHeader; i++)
				{
					if(fread(colBuf, 1, tga.bytesPerPixel, f) != tga.bytesPerPixel)
					{								
						return false;
					}
							
					*ptr++ = colBuf[2];
					*ptr++ = colBuf[1];
					*ptr++ = colBuf[0];							
									
					// jos 32bpp kuva
					if(tga.bytesPerPixel == 4)
					{	
						*ptr++ = colBuf[3];
					}
					else
					{
						*ptr++ = 255;
					}

					currentByte += tga.bytesPerPixel;
					currentPixel++;

					if(currentPixel > width*height)
					{	
						char buf[512] = {0};
						
						sprintf(buf, "Image::LoadTGA Error: Too many pixels\n");
						dmsErrorMsg("%s\n", buf);
						return false;
					}
				}
			}
			// RLE pakattu
			else
			{
				chunkHeader -= 127;

				if(fread(colBuf, 1, tga.bytesPerPixel, f) != tga.bytesPerPixel)
				{
					char buf[512] = {0};

					sprintf(buf, "Image::LoadTGA Error: Couldn't read data from %s\n", name);
					dmsErrorMsg("%s\n", buf);
					return false;
				}

				for(int i = 0; i < chunkHeader; i++)
				{	
					*ptr++ = colBuf[2];
					*ptr++ = colBuf[1];
					*ptr++ = colBuf[0];							
								
					// jos 32bpp kuva							
					if(tga.bytesPerPixel == 4)
					{								
						*ptr++ = colBuf[3];							
					}
					else
					{
						*ptr++ = 255;
					}
												
					currentByte += tga.bytesPerPixel;
					currentPixel++;
					if(currentPixel > width*height) 
					{			
						char buf[512] = {0};

						sprintf(buf, "Image::LoadTGA Error: Too many pixels\n");
						dmsErrorMsg("%s\n", buf);
						return false;
					}
				}
			}

		} while(currentPixel < width*height);

		if(colBuf)
		{
			delete [] colBuf;
		}
	}
	else
    {
		char buf[512] = {0};

		sprintf(buf, "Image::LoadTGA Error: %s is not a valid TGA file\n", name);
		dmsErrorMsg("%s\n", buf);
		return false;
	}

    fclose(f);

    return true;
}

//--------------------------------------------------------------------------------------------
//
// "LoadMemoryTGA(char *name, unsigned char *fileData, unsigned int fileSize)
// lataa 24bpp tai 32bpp RLE-pakatun tai pakkaamattoman TGA filen
// suoraan muistista
//
//--------------------------------------------------------------------------------------------

bool Image::LoadMemoryTGA(char *name, unsigned char *fileData, unsigned int fileSize)
{
	char buf[256];
	unsigned int offset = 0;
	unsigned char *ptr = NULL;

    TGAHeader tgaheader;
    TGAData tga;

    // onko tga kuva RLE-pakattu vai ei
    unsigned char uncomp[12] = { 0, 0,  2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    unsigned char compre[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	memset(buf, 0, 255*sizeof(char));
	
    // luetaan tiedoston headeri
	memcpy(&tgaheader, fileData, sizeof(TGAHeader));
	    
	if(memcmp(uncomp, &tgaheader, sizeof(TGAHeader)) == 0)
	{
		// ladataan pakkaamaton TGA-file:
		memcpy(tga.header, fileData + 12, sizeof(tga.header));
		        		
        width = tga.header[1] * 256 + tga.header[0];
        height = tga.header[3] * 256 + tga.header[2];
        bpp = tga.header[4];
        tga.width = width;
        tga.height = height;
        tga.bpp = bpp;	

		// tarkistetaan ett‰ arvot on oikeissa rajoissa
        if(width <= 0 || height <= 0 || (bpp != 24 && bpp != 32))
        {	
			char buf[512] = {0};

			sprintf(buf, "Image::LoadMemoryTGA Error: %s is not a valid TGA file\n", name);
			dmsErrorMsg("%s\n", buf);
			return false;
        }

        type = GL_RGBA;
        tga.bytesPerPixel = (tga.bpp / 8);
        
        imagedata = new unsigned int[tga.width * tga.height * 4];
        if(!imagedata)
        {
			char buf[512] = {0};

			sprintf(buf, "Image::LoadMemoryTGA Error: memory allocation error\n");
			dmsErrorMsg("%s\n", buf);
			return false;
		}

		ptr = (unsigned char *)imagedata;
		offset = 18;
		for(unsigned int i = 0; i < tga.width * tga.height; i++)
		{
			unsigned char r, g, b, a;

			r = *(fileData + offset); offset++;
			g = *(fileData + offset); offset++;
			b = *(fileData + offset); offset++;

			if(bpp = 32)
			{
				a = *(fileData + offset);
				offset++;
			}
			else
			{
				a = 255;
			}

			*ptr++ = b;
			*ptr++ = g;
			*ptr++ = r;
			*ptr++ = a;
		}
	}
	else if(memcmp(compre, &tgaheader, sizeof(tgaheader)) == 0)
	{
		// ladataan pakkaamaton TGA-file:
		memcpy(tga.header, fileData + 12, sizeof(tga.header));
		        		
        width = tga.header[1] * 256 + tga.header[0];
        height = tga.header[3] * 256 + tga.header[2];
        bpp = tga.header[4];
        tga.width = width;
        tga.height = height;
        tga.bpp = bpp;	

		// tarkistetaan ett‰ arvot on oikeissa rajoissa
        if(width <= 0 || height <= 0 || (bpp != 24 && bpp != 32))
        {	
			char buf[512] = {0};

			sprintf(buf, "Image::LoadMemoryTGA Error: %s is not a valid TGA file\n", name);
			dmsErrorMsg("%s\n", buf);
			return false;
        }

        type = GL_RGBA;
        tga.bytesPerPixel = (tga.bpp / 8);
        
        imagedata = new unsigned int[tga.width * tga.height * 4];
        if(!imagedata)
        {
			char buf[512] = {0};

			sprintf(buf, "Image::LoadMemoryTGA Error: memory allocation error\n");
			dmsErrorMsg("%s\n", buf);
			return false;
		}
		
		unsigned int currentPixel = 0;
		unsigned int currentByte = 0;
		unsigned int offset = 18;
		unsigned char chunkHeader;
		unsigned char *ptr = (unsigned char *)imagedata;
		unsigned char *colBuf = new unsigned char[tga.bytesPerPixel];

		do
		{
			// Luetaan chunkheader
			memcpy(&chunkHeader, fileData + offset, sizeof(chunkHeader));
			offset += sizeof(chunkHeader);

			// Onko kuvadata pakkaamatonta...
			if(chunkHeader < 128)
			{
				chunkHeader++;
				for(int i = 0; i < chunkHeader; i++)
				{					
					memcpy(colBuf, fileData + offset, sizeof(colBuf));
					offset += sizeof(colBuf);
							
					*ptr++ = colBuf[2];
					*ptr++ = colBuf[1];
					*ptr++ = colBuf[0];							
									
					// jos 32bpp kuva
					if(bpp = 32)
					{	
						*ptr++ = colBuf[3];
					}
					else
					{
						*ptr++ = 255;
					}

					currentByte += tga.bytesPerPixel;
					currentPixel++;

					if(currentPixel > width*height)
					{	
						char buf[512] = {0};
						
						sprintf(buf, "Image::LoadMemoryTGA Error: Too many pixels\n");
						dmsErrorMsg("%s\n", buf);
						return false;
					}
				}
			}
			// ... vai RLE pakattua
			else
			{
				chunkHeader -= 127;
				
				memcpy(colBuf, fileData + offset, sizeof(colBuf));
				offset += sizeof(colBuf);
				
				for(int i = 0; i < chunkHeader; i++)
				{	
					*ptr++ = colBuf[2];
					*ptr++ = colBuf[1];
					*ptr++ = colBuf[0];							
								
					// jos 32bpp kuva							
					if(bpp == 32)
					{								
						*ptr++ = colBuf[3];							
					}
					else
					{
						*ptr++ = 255;
					}
												
					currentByte += tga.bytesPerPixel;
					currentPixel++;
					if(currentPixel > width*height) 
					{			
						char buf[512] = {0};

						sprintf(buf, "Image::LoadMemoryTGA Error: Too many pixels\n");
						dmsErrorMsg("%s\n", buf);
						return false;
					}
				}
			}

		} while(currentPixel < width*height);

		if(colBuf)
		{
			delete [] colBuf;
			colBuf = NULL;
		}
	}	
	else
    {
		char buf[512] = {0};

		sprintf(buf, "Image::LoadMemoryTGA Error: %s is not a valid TGA file\n", name);
		dmsErrorMsg("%s\n", buf);
		return false;
	}

    return true;
}

//--------------------------------------------------------------------------------------------
//
// "LoadPCX(char *name)
// lataa 8bpp tai 24bpp RLE-pakatun PCX tiedoston
//
//--------------------------------------------------------------------------------------------

bool Image::LoadPCX(char *name)
{
	unsigned int i;
	unsigned char tavu, tavu2;
	unsigned int vari=0,vari2=0;

	FILE *pcx;

	unsigned char *buffer;
	char *col[3], *ptr;

	PCXHeader Header;
	PCXPaletteEntry Palette[256];

	pcx = fopen(name,"rb");
	if(pcx == NULL)
	{
		dmsErrorMsg("Couldn't open file %s\n", name);
		return 0;
	}

	fread(&Header, 128, 1, pcx);
	if(Header.version!= 5 || Header.encoding!=1)
	{
		dmsErrorMsg("%s must be version 5 and RLE packed\n", name);
		return 0;
	}

	type = GL_RGB;
	width = Header.bytes_per_line;
	height= Header.ymax-Header.ymin+1;
	int bpp = Header.bits_per_plane*Header.color_planes;

	imagedata = new unsigned int[width*height];
	if(!imagedata)
	{
		dmsErrorMsg("Memory allocation error\n");
		return 0;
	}

	if(bpp == 8)
	{
		buffer = new unsigned char[width*height];
		fseek(pcx, -768, SEEK_END);
		fread(Palette, 768, 1, pcx);
		fseek(pcx, 128, SEEK_SET);
		
		for (i=0;i<width*height;i++)
		{
			tavu = fgetc(pcx);

			if (tavu < 192)
				buffer[i]=tavu;
			else
			{
				tavu2 = fgetc(pcx);
				while (tavu > 192)
				{
					buffer[i]=tavu2;
					if (i < (width*height) && tavu > 193)
						i++;
					tavu--;
				}
			}
		}
		unsigned char *dataptr = (unsigned char *)imagedata;
		for (i=0;i<width*height;i++)
		{
			unsigned char val = buffer[i];
			*dataptr++ = Palette[val].R;
			*dataptr++ = Palette[val].G;
			*dataptr++ = Palette[val].B;
			*dataptr++ = 255; //alpha channel, PCX:ll‰ tyhj‰			
		}
		delete [] buffer;
	}
	else		//24bit
	{
		col[0] = new char[width*height];
		col[1] = new char[width*height];
		col[2] = new char[width*height];
		for(vari=0;vari<(3*height);vari++)
		{
			vari2=vari%3;
			for(unsigned int x=0;x<width;x++)
			{
				tavu = fgetc(pcx);
				if(tavu<192)
				{
					*col[vari2]=tavu;
				}
				else
				{
					tavu2 = fgetc(pcx);
					while(tavu>192)
					{
						*col[vari2]=tavu2;
						if((x+vari)<height*width && tavu>193)
						{
							x++;
							col[vari2]++;
						}
						if(x>=width)
						{
							x=0;
							vari++;
							vari2=vari%3;
						}
						tavu--;
					} 
				}
				col[vari2]++;   //vari%3
			}
		}
		col[0]-=width*height;
		col[1]-=width*height;
		col[2]-=width*height;
		ptr=(char *)imagedata;
		for(i=0;i<width*height;i++)
		{
			*ptr++=col[0][i];
			*ptr++=col[1][i];
			*ptr++=col[2][i];
			*ptr++=255;			//alphakanava
		}
		delete [] col[0];
		delete [] col[1];
		delete [] col[2];
	}
	fclose(pcx);
	return 1;

}

//--------------------------------------------------------------------------------------------
//
// "LoadMemoryPCX(char *name, unsigned char *fileData, unsigned int fileSize)
// lataa 8bpp tai 24bpp RLE-pakatun PCX tiedoston suoraan muistista
//
//--------------------------------------------------------------------------------------------

bool Image::LoadMemoryPCX(char *name, unsigned char *fileData, unsigned int fileSize)
{	
	unsigned int i, x, y;
	unsigned char tavu, tavu2;
	unsigned int vari=0,vari2=0;
	unsigned int bytesPerLine = 0;
		
	unsigned char *buffer;
	unsigned char *ptr = NULL;

	PCXHeader Header;
	PCXPaletteEntry Palette[256];
		
	memcpy(&Header, fileData, sizeof(Header));
				
	if(Header.version!= 5 || Header.encoding!=1)
	{
		dmsErrorMsg("%s must be version 5 and RLE packed\n", name);
		return 0;
	}

	type = GL_RGBA;
	width = Header.bytes_per_line;
	height = Header.ymax-Header.ymin+1;
	int bpp = Header.bits_per_plane*Header.color_planes;

	bytesPerLine = Header.bytes_per_line;

	imagedata = new unsigned int[width*height];
	if(!imagedata)
	{
		dmsErrorMsg("Memory allocation error\n");
		return 0;
	}

	if(bpp == 8)
	{						
		buffer = new unsigned char[width*height];
		ptr = fileData + (fileSize - 768);
		
		for(i = 0; i < 256; i++)
		{
			Palette[i].R = *ptr++;
			Palette[i].G = *ptr++;
			Palette[i].B = *ptr++;
		}

		ptr = fileData + 128;
		
		for(i=0; i < width*height; i++)
		{
			tavu = *ptr++;
			if(tavu<192)
			{
				buffer[i]=tavu;
			}
			else
			{
				tavu2 = *ptr++;
				while(tavu>192)
				{
					buffer[i]=tavu2;
					if(i<height*width && tavu>193)
					{
						i++;						
					}
					tavu--;
				}			
			}
		}
				
		ptr = (unsigned char *)imagedata;
		for (i=0;i<width*height;i++)
		{
			unsigned char val = buffer[i];
			*ptr++ = Palette[val].R;
			*ptr++ = Palette[val].G;
			*ptr++ = Palette[val].B;
			*ptr++ = 255; //alpha channel, PCX:ll‰ tyhj‰					
		}

		delete [] buffer;
	}
	else		//24bit
	{		
		unsigned char *scanLine = new unsigned char[Header.bytes_per_line*Header.color_planes];
				
		ptr = fileData + 128;
/*
		memset(imagedata, 255, width*height*4);
		for(int yy = 0; yy < height; yy++)
		{
			for(int xx = 0; xx < width; xx++)
			{
				imagedata[yy*width+xx] = (255<<16)+(255<<8)+255;
			}
		}
*/					
		unsigned char *dest = (unsigned char *)imagedata;
		for(y = 0; y < height; y++)
		{							
			int c = 0;				
			unsigned char byteCount;
			unsigned char value;
			unsigned char tmp;
		
			memset(scanLine, 0, Header.bytes_per_line*Header.color_planes*sizeof(unsigned char));

			// Read in one scanline
			while(c < Header.bytes_per_line*Header.color_planes)
			{
				tmp = *ptr++;
				
				// Are top two bytes set?
				if(tmp > 192)
				{
					byteCount = tmp - 192;
					value = *ptr++;					
				}
				else
				{
					byteCount = 1;
					value = tmp;
				}
				
				for(i = 0; i < byteCount; i++, c++)
				{
					scanLine[c] = value;
				}					
			}						

			// Rest ...			
			for(x = 0; x < width; x++)
			{								
				*dest++ = scanLine[x];
				*dest++ = scanLine[x+Header.bytes_per_line];
				*dest++ = scanLine[x+Header.bytes_per_line*2];
				*dest++ = 255;
			}			
		}		
				
		delete [] scanLine;

	}
		

	return true;
}

//--------------------------------------------------------------------------------------------
//
// "LoadJPG(char *name)
// lataa 24bpp JPG tiedoston
//
//--------------------------------------------------------------------------------------------

bool Image::LoadJPG(char *name)
{
	unsigned char *ptr = NULL, *scanLine = NULL;
	struct jpeg_decompress_struct cinfo;
	JPGData *jpg = NULL;
	FILE *f;	
	
	if(!(f = fopen(name, "rb")))	
	{		
		dmsErrorMsg("Image::LoadJPG() error: Couldn't open \"%s\"\n", name);		
		return false;
	}
	
	// JPEG error handle
	jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	
	// Inits
	jpeg_create_decompress(&cinfo);	
	jpeg_stdio_src(&cinfo, f);
		
	jpg = new JPGData;

	// Decode JPG
	jpeg_read_header(&cinfo, TRUE);		
	jpeg_start_decompress(&cinfo);
	jpg->row = cinfo.image_width * cinfo.num_components;
	jpg->xsize   = cinfo.image_width;
	jpg->ysize   = cinfo.image_height;
	
	jpg->data = new unsigned char[jpg->row * jpg->ysize];
		
	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[jpg->ysize];
	for (int i = 0; i < jpg->ysize; i++)
		rowPtr[i] = &(jpg->data[i*jpg->row]);
	
	int rowsRead = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
	{		
		rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	}
		
	delete [] rowPtr;

	jpeg_finish_decompress(&cinfo);

	// Release
	jpeg_destroy_decompress(&cinfo);
	
	width = jpg->xsize;
	height = jpg->ysize;	
	bpp = 32;	

	imagedata = new unsigned int[jpg->xsize*jpg->ysize*4];
	scanLine = new unsigned char[jpg->row];
	if(!imagedata)
	{
		return false;
	}		

	// 24bit JPEG
	ptr = (unsigned char *)imagedata;
	for(unsigned int c = 0; c < width*height*3; c+=3)
	{
		*ptr++ = jpg->data[c+0];
		*ptr++ = jpg->data[c+1];
		*ptr++ = jpg->data[c+2];
		*ptr++ = 255;
	}

	fclose(f);

	if(jpg)
	{
		if(jpg->data)
		{
			delete [] jpg->data;
			jpg->data = NULL;
		}

		delete jpg;	
		jpg = NULL;
	}

	if(scanLine)
	{
		delete [] scanLine;
		scanLine = NULL;
	}

	return true;
}

//--------------------------------------------------------------------------------------------
//
// "LoadMemoryJPG(char *name, unsigned char *fileData, unsigned int fileSize)
// lataa 24bpp JPG tiedston suoraan muistista
//
//--------------------------------------------------------------------------------------------

bool Image::LoadMemoryJPG(char *name, unsigned char *fileData, unsigned int fileSize)
{
	struct jpeg_source_mgr mySourceManager;
	struct jpeg_error_mgr jpegErrorHandler;
	struct jpeg_decompress_struct cinfo;
	unsigned char *data = NULL, *ptr = NULL, *scanLine = NULL;
	unsigned int row;

	jpegData = fileData;
	jpegSize = fileSize;

	// Virheiden k‰sittelij‰
	cinfo.err = jpeg_std_error(&jpegErrorHandler);

	// Initti	
	jpeg_create_decompress(&cinfo);

	// T‰ytet‰‰n rakenteet k‰sin customointia varten:
	//
	// Kerrotaan ett‰ k‰ytet‰‰n omaa manageria lukemiseen, jotta saadaan
	// shitti ulos muistista eik‰ I/O streamist‰
	cinfo.src = &mySourceManager;
	mySourceManager.bytes_in_buffer = fileSize;
	mySourceManager.next_input_byte = fileData;

	// Funktio-osoittimet omiin funkkareihin	
	mySourceManager.init_source = initJPEGDataSource;
	mySourceManager.fill_input_buffer = fillJPEGInputBuffer;
	mySourceManager.skip_input_data = skipJPEGInputData;
	mySourceManager.resync_to_restart = resyncJPEGToRestart;
	mySourceManager.term_source = terminateJPEGSource;	

	// Decode JPG
	jpeg_read_header(&cinfo, TRUE);		
	jpeg_start_decompress(&cinfo);

	row = cinfo.image_width * cinfo.num_components;
	width = cinfo.image_width;
	height = cinfo.image_height;
	bpp = 32;
		
	data = new unsigned char[height*row];
	if(!data)
	{
		dmsErrorMsg("LoadMemoryJPG() error: Couldn't allocate memory\n");
		return false;
	}

	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[height];
	for (unsigned int i = 0; i < height; i++)
		rowPtr[i] = &(data[i*row]);
	
	int rowsRead = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
	{		
		rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	}
		
	delete [] rowPtr;

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);	

	// Fill in our imagedata
	imagedata = new unsigned int[width*height*4];
	scanLine = new unsigned char[row];
	if(!imagedata)
	{
		return false;
	}		

	// 24bit JPEG
	ptr = (unsigned char *)imagedata;
	for(unsigned int c = 0; c < width*height*3; c+=3)
	{
		*ptr++ = data[c+0];
		*ptr++ = data[c+1];
		*ptr++ = data[c+2];
		*ptr++ = 255;
	}

	if(data)
	{
		delete [] data;	
		data = NULL;
	}

	if(scanLine)
	{
		delete [] scanLine;
		scanLine = NULL;
	}

	return true;
}

bool Image::LoadPNG(char *name)
{
	return true;
}

bool Image::set(int w, int h, int b)
{
	width = w;
	height = h;
	bpp = b;

	imagedata = new unsigned int[w*h];
	if(!imagedata)
	{	
		dmsErrorMsg("Memory allocation error\n");
		return false;
	}

	memset(imagedata, 0, w*h*4);

	return true;
}

unsigned int Image::GetWidth(void)
{
	return width;
}

unsigned int Image::GetHeight(void)
{
	return height;
}

unsigned int *Image::GetPtr(void)
{
	return imagedata;
}

void Image::SetPtr(unsigned int *b)
{
	memcpy(imagedata, b, width*height*4);	
}


unsigned int Image::getType(void)
{
	return type;
}

unsigned int Image::getBpp(void)
{
	return bpp;
}

float Image::lum(unsigned int x, unsigned int y)
{
	int r, g, b;
	if((x<0) || (x>=width) || (y<0) || (y>=height)) return 0;

	r = imagedata[(y*width+x)];
	g = imagedata[(y*width+x)];
	b = imagedata[(y*width+x)];
	//r = imagedata[3*(y*width+x)];
	//g = imagedata[3*(y*width+x)+1];
	//b = imagedata[3*(y*width+x)+2];
	return float(sqrt(r*r+g*g+b*b) / sqrt(3));
}

void Image::release(void)
{	
	if (imagedata != NULL) 
	{
		delete [] imagedata;
		imagedata = NULL;
	}
	
}

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{

	//--------------------------------------------------------------------------------------------
	//
	// Kustomoidut rutiinit JPEG:n lataamiseksi muistista
	// 
	//--------------------------------------------------------------------------------------------

	void initJPEGDataSource(j_decompress_ptr cinfo)
	{
	}

	unsigned char fillJPEGInputBuffer(j_decompress_ptr cinfo)
	{
		cinfo->src->next_input_byte	= jpegData;
		cinfo->src->bytes_in_buffer	= jpegSize;

		return true;
	}

	void skipJPEGInputData(j_decompress_ptr cinfo, long nBytes)
	{
		cinfo->src->next_input_byte	+= nBytes;
		cinfo->src->bytes_in_buffer	-= nBytes;
	}

	unsigned char resyncJPEGToRestart(j_decompress_ptr cinfo, int desired)
	{
		jpeg_resync_to_restart(cinfo, desired);

		return true;
	}

	void terminateJPEGSource(j_decompress_ptr cinfo)
	{
	}
};

#endif
