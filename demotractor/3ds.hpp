#ifndef _3DS_HPP_
#define _3DS_HPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include "list.hpp"
//#include "material.hpp"
#include "object.hpp"
//#include "camera.hpp"
//#include "light.hpp"

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{

//--------------------------------------------------------------------------------------------
//  Chunk-structur
//--------------------------------------------------------------------------------------------

	typedef struct
	{
		unsigned short id;
		unsigned long len;
	} Chunk;

//--------------------------------------------------------------------------------------------
//  Loader3DS class
//--------------------------------------------------------------------------------------------

	class __declspec(dllexport) Loader3DS
	{
		friend class Scene;
		friend class Object;
		friend class Particle;
		friend class ParticleSystem;	

		public:

			Loader3DS();
			~Loader3DS();

			bool import3DS(char *fname);
			
			// Chunks
			bool objectBlock(long p);
			bool vertexList(long p);
			bool faceList(long p);
			bool faceMaterial(long p);
			bool uvList(long p);
			bool readMatrix(long p);
			bool materialName(long p);
			bool materialMap(long p);
			bool lightReader(long p);
			bool spotlightReader(long p);
			bool cameraReader(long p);
			bool smooListReader(long p);
			
			bool readChunk(long p);
			void shut();
			void render();

			// operator overloadit: export Scene, Object, ParticleSystem jne.
			//bool operator >> (Scene *scene);		
			bool operator >> (Object *object);
			
		private:
			
			FILE *f;
			unsigned long fileLenght;
			unsigned long currentLenght;
			unsigned int nObjects, nCameras, nLights, nTextures, nMaterials;
			int type;

			//List <Material> materials;
			List <Object> objects;
			//List <Camera> cameras;
			//List <Light> lights;
	};
}

#endif