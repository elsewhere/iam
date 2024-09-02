//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "mmgr.h"
#include "3ds.hpp"
#include "chunks.hpp"
//#include "scene.hpp"
#include "globals.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Class code
//--------------------------------------------------------------------------------------------

Loader3DS::Loader3DS()
{
	type = 0;
	nObjects = 0;
	nCameras = 0;
	nLights = 0;
	nTextures = 0;
	nMaterials = 0;
}

Loader3DS::~Loader3DS()
{
	shut();
}

bool Loader3DS::import3DS(char *fname)
{
	f = fopen(fname, "rb");
	if(!f)
	{
		#ifdef _DEBUG

			char buf[256] = {0};
			sprintf(buf, "Tiedostoa %s ei voinut avata", fname);
			dmsMsg("%s\n", buf);

		#endif		

		return false;
	}

	// haetaan tiedoston koko
	fseek(f, 0, SEEK_END);
	fileLenght = ftell(f);
	fseek(f, 0, SEEK_SET);

	if(!readChunk(fileLenght)) return false;

	fclose(f);

	return true;
}

bool Loader3DS::objectBlock(long p)
{
        char c;
        char buf[64] = {0};
        int i = 0;        
          
        while ( (c = fgetc(f)) != EOF && c != '\0') 
		{
			buf[i++] = c;    
        }

        type = 0;
		(strncmp(buf, "Camera",6) == 0) ? type = 1 : type = type;
		(strncmp(buf, "Light", 5) == 0) ? type = 2 : type = type;
		(strncmp(buf, "Omni", 4) == 0) ? type = 3 : type = type;

		switch(type)
		{
			case 0: 
			{
				Object *object = new Object;

				objects.addTail(object, buf);
				nObjects++; 
			} break;
			
			case 1: 
			{
				//Camera *camera = new Camera;

				//cameras.addTail(camera, buf);
				nCameras++; 
			} break;

			case 2: 
			{
				//Light *light = new Light;

				//lights.addTail(light, buf);				
				nLights++; 
			} break;
			
			case 3: 
			{
				//Light *light = new Light;

				//lights.addTail(light, buf);				
				nLights++;
			} break;
		}
                                                          
        if(!readChunk(p)) return false;		

		return true;
}

bool Loader3DS::vertexList(long p)
{
        unsigned short nVertices;
        int i;
        float c[3];
		Node <Object> *getNode = NULL;
		Object *ptr = NULL;

		// haetaan oikea objekti indexin perusteella
		getNode = objects.get(nObjects-1);
		if(!getNode)
		{
			return false;
		}

		ptr = getNode->a;
		if(!ptr)
		{
			return false;
		}

        if(fread(&nVertices, sizeof(nVertices), 1, f) != 1) return false;
		ptr->initVertices(nVertices);
        		
        for(i = 0; i < nVertices; i++)
        {
			Vertex v;

            if(fread(&c, sizeof(c), 1, f) != 1) return false;

			#ifdef _OPENGL_HPP_

				v.setPosition(Vector(-c[0], c[2], c[1]));
				v.old = Vector(-c[0], c[2], c[1]);

			#else

				v.setPosition(Vector(c[0], c[1], c[2]));
				v.old = Vector(c[0], c[1], c[2]);

			#endif			

			if(!ptr->setVertex(i, v)) return false;            
        }		

		return true;
}

bool Loader3DS::faceList(long p)
{
        unsigned short nFaces, c[3], flags;
        int i;
		Node <Object> *getNode = NULL;
		Object *ptr = NULL;

		// haetaan oikea objekti indexin perusteella
		getNode = objects.get(nObjects-1);
		if(!getNode)
		{
			return false;
		}

		ptr = getNode->a;
		if(!ptr)
		{
			return false;
		}

        if(fread(&nFaces, sizeof(nFaces), 1, f) != 1) return false;
		ptr->initFaces(nFaces);        

        for(i = 0; i < nFaces; i++)		
        {
			Face tmpFace;
            if(fread(&c, sizeof(c), 1, f) != 1) return false;
            if(fread(&flags, sizeof(flags), 1, f) != 1) return false;
			
			//	vanhatapa: suorat indexit numerot verticelistaan.
			//	Ei käytössä, tässä vain esimerkin vuoksi.			
			tmpFace.aa = c[0];
			tmpFace.bb = c[1];
			tmpFace.cc = c[2];

			// uusitapa: osoittimet suoraan oikeisiin vertexeihin
			if(!tmpFace.setVertex(&ptr->vertex[c[0]], &ptr->vertex[c[1]], &ptr->vertex[c[2]])) return false;
			if(!ptr->setFace(i, tmpFace)) return false;

			if(!ptr->face[i].calculateNormal()) return false;	        
        }

		if(!ptr->calculateVertexNormals()) return false;

        if(!readChunk(p)) return false;

		return true;
}

bool Loader3DS::faceMaterial(long p)
{
	char c, buf[80] = {0};
    unsigned short nFaces, number;
    int i;
	Node <Object> *getNode = NULL;		
	Object *ptr = NULL;
		
	i = 0;
    while ( (c = fgetc(f)) != EOF && c != '\0')
	{
		buf[i++] = c;
    }

	// haetaan oikea objekti indexin perusteella
	getNode = objects.get(nObjects-1);		
	if(!getNode)
	{
		return false;
	}

	ptr = getNode->a;
	if(!ptr)
	{
		return false;
	}

    if(fread(&nFaces, sizeof(nFaces), 1, f) != 1) return false;

    for(i = 0; i < nFaces; i++)		
    {
		if(fread(&number, sizeof(number), 1, f) != 1) return false;
				
		// ei tehdä false tsekkiä koska ei ole virhe 
		// jos facelle ei ole merkitty texturea
		ptr->face[number].setTexture(buf);
    }

	return true;
}

bool Loader3DS::uvList(long p)
{
        unsigned short nCoords;
        int i;
        float c[2];
		Node <Object> *getNode = NULL;
		Object *ptr = NULL;

		// haetaan oikea objekti indexin perusteella
		getNode = objects.get(nObjects-1);
		if(!getNode)
		{
			return false;
		}

		ptr = getNode->a;
		if(!ptr)
		{
			return false;
		}

        if(fread(&nCoords, sizeof(nCoords), 1, f) != 1) return false;        

        for(i = 0; i < nCoords; i++)		
        {
                if(fread(&c, sizeof(c), 1, f) != 1) return false;
				ptr->vertex[i].setUV(c[0], c[1]);
        }

		return true;
}

bool Loader3DS::readMatrix(long p)
{
        float trans[3];  // 1x3 matrix
        float matrix[9]; // 3x3 matrix
		Node <Object> *getNode = NULL;
		Object *ptr = NULL;

		// haetaan oikea objekti indexin perusteella
		getNode = objects.get(nObjects-1);
		if(!getNode)
		{
			return false;
		}

		ptr = getNode->a;
		if(!ptr)
		{
			return false;
		}

        if(fread(&matrix, sizeof(matrix), 1, f) != 1) return false;
        if(fread(&trans, sizeof(trans), 1, f) != 1) return false;
        
		#ifdef _OPENGL_HPP_
			ptr->setPosition(Vector(-trans[0], trans[2], trans[1]));

			ptr->matrix.makeIdentity();
		#else

		#endif

		for(unsigned int i = 0; i < ptr->nVertices; i++)
		{
			Vector tmp;			
			Vector v = ptr->vertex[i].position;
			tmp = v * ptr->matrix;
			
			ptr->vertex[i].position = tmp;
		}

		return true;
}

bool Loader3DS::materialName(long p)
{
		char c, buf[64] = {0};		
		int i;		

		//Material *material = new Material;
					
		i = 0;		
		while ( (c = fgetc(f)) != EOF && c != '\0')
		{
			buf[i++] = c;	    
        }
/*				
		if(!material)
		{
			return false;
		}					
						
		material->setName(buf);
		materials.addTail(material, buf);
*/
		nMaterials++;
							
		return true;
}

bool Loader3DS::materialMap(long p)
{
		char c, buf[64] = {0};		
//		Node<Material> *getNode = NULL;
//		Material *t = NULL;
		int i;        
/*					
		getNode = materials.get(nMaterials-1);
		if(getNode)
		{
			t = getNode->a;		
		}		
*/		
		i = 0;				
		while ( (c = fgetc(f)) != EOF && c != '\0')
		{
		   buf[i++] = c;
	       printf("%c",c);
        }
		
		// - HACK WARNING! - 

		// muutetaan filenamen tiedostopääte PCX:ksi :)
		buf[i-1] = 'x';
		buf[i-2] = 'c';
		buf[i-3] = 'p';
/*		
		// - End of Hack Warning - 		
		if(t)
		{				
			t->setFileName(buf);			
		}
		
		if(tManager)
		{	
			char fileName[256] = {0};

			sprintf(fileName,"gfx\\%s",t->getFileName());
			if(!tManager->addTexture(fileName, t->getName(),PCX)) return false;
		}

		nTextures++;		
*/				
		return true;
}

bool Loader3DS::lightReader(long p)
{
        float c[3];
//		Node <Light> *getNode = NULL;
//		Light *ptr = NULL;
/*		
		getNode = lights.get(nLights-1);
		if(!getNode)
		{
			return false;
		}

		ptr = getNode->a;
		if(!ptr)
		{
			return false;
		}
*/
        if(fread(&c, sizeof(c), 1, f) != 1) return false;        

		#ifdef _OPENGL_HPP_
			//ptr->setPosition(Vector(-c[0], c[2], c[1]));
		#else
			//ptr->setPosition(Vector(c[0], c[1], c[2]));
		#endif

        if(!readChunk(p)) return false;

		return true;
}

bool Loader3DS::spotlightReader(long p) 
{
    float c[5];
    if (fread(&c, sizeof(c), 1, f) != 1) return false;    

	return true;
}

bool Loader3DS::cameraReader(long p)
{
        float c[8];
//		Node <Camera> *getNode = NULL;
//		Camera *ptr = NULL;
/*
		// haetaan oikea objekti indexin perusteella
		getNode = cameras.get(nCameras-1);
		if(!getNode)
		{
			return false;
		}

		ptr = getNode->a;
		if(!ptr)
		{
			return false;
		}
*/
        if(fread(&c, sizeof(c), 1, f) != 1) return false;        
/*
		#ifdef _OPENGL_HPP_
			ptr->setPosition(Vector(-c[0], c[2], c[1]));
			ptr->setTarget(Vector(-c[3], c[5], c[4]));
			ptr->setLens(c[7]);
			ptr->calculateFOV();
		#else
			ptr->setPosition(Vector(c[0], c[1], c[2]));
			ptr->setTarget(Vector(c[3], c[4], c[5]));
			ptr->setLens(c[7]);
			ptr->calculateFOV();
		#endif
*/
		return true;
}

bool Loader3DS::smooListReader(long p)
{
        unsigned long s;
        unsigned short i;

        while(ftell(f) < p)
        {
                if(fread(&s,sizeof(s),1,f) != 1) return false;
                
                for(i=0; i < 32; i++)
                {
                        if(s & (1 << i))
                        {
                                //printf("%d\n",i+1);
                        }
                }
        }

		return true;
}

bool Loader3DS::readChunk(long p)
{
        Chunk h;

        while(ftell(f) < p)
        {
                currentLenght = ftell(f);                				
				if (fread(&h.id, sizeof(h.id), 1, f) != 1) return false;
				if (fread(&h.len, sizeof(h.len), 1, f) != 1) return false;
				if (h.len == 0) return false;

                switch(h.id)
                {
                        case CHUNK_MAIN:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!readChunk(p)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;

                        case CHUNK_OBJMESH:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!readChunk(p)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;

                        case CHUNK_OBJBLOCK:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!objectBlock(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;

                        case CHUNK_TRIMESH:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!readChunk(p)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;

                        case CHUNK_VERTLIST:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!vertexList(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;

                        case CHUNK_FACELIST:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!faceList(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;

                        case CHUNK_FACEMAT:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!faceMaterial(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);                                
                        } break;


                        case CHUNK_MAPFILE:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!materialMap(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);                                
                        } break;

                        case CHUNK_MAPLIST:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!uvList(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);                                
                        } break;


                        case CHUNK_TRMATRIX:
                        {
                               
                                currentLenght = currentLenght+h.len;
                                
								if(!readMatrix(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);                                
                        } break;

                        case CHUNK_MATNAME:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!materialName(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);                                
                        } break;

                        case CHUNK_LIGHT:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!lightReader(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);                                
                        } break;

                        case CHUNK_CAMERA:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!cameraReader(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);                                
                        } break;                        

                        case CHUNK_SMOOLIST:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!smooListReader(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;


                        case CHUNK_SPOTLIGHT:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!spotlightReader(currentLenght)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;

                        case CHUNK_MATERIAL:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!readChunk(p)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;

                        case CHUNK_TEXTURE:
                        {                                
                                currentLenght = currentLenght+h.len;
                                
								if(!readChunk(p)) return false;

                                fseek(f, currentLenght, SEEK_SET);
                        } break;
                       
                        default:
                        {
                                fseek(f, currentLenght+h.len, SEEK_SET);
                        } break;
                }

                if(ferror(f)) break;
        }

		return true;
}

void Loader3DS::shut()
{
//	materials.freeList();
	objects.freeList();
//	cameras.freeList();
//	lights.freeList();
}

// Hidas ja huono tapa renderoida
void Loader3DS::render()
{	
/*
	Node <Object> *getNode = NULL;
	Node <Camera> *getCamera = NULL;
	Node <Light> *getLight = NULL;
	Object *ptr = NULL;	
	Camera *camera = NULL;
	Light *light = NULL;
	Vertex *v1 = NULL, *v2 = NULL, *v3 = NULL;
	unsigned int i; 
	
	// Haetaan ensimmäinen kamera listasta ja käytetään sitä
	getCamera = cameras.get(0);
	if(!getCamera)
	{
		return;
	}

	camera = getCamera->a;
	if(!camera)
	{
		return;
	}

	getLight = lights.get(0);
	if(!getLight)
	{
		return;
	}

	light = getLight->a;
	if(!light)
	{
		return;
	}

	Vector position = camera->getPosition();
	Vector target = camera->getTarget();
		
	glLoadIdentity();
	gluLookAt(position.x, position.y, position.z, target.x, target.y, target.z, 0, 1, 0);

	if(light)
	{
		float *amb, *dif, *pos;

		amb = light->getAmbient();
		dif = light->getDiffuse();
		pos = light->getPosition();

		glPushMatrix();
		
			glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
			glLightfv(GL_LIGHT0, GL_POSITION, pos);

		glPopMatrix();

		myEnable(MY_LIGHTING);
		myEnable(MY_LIGHT0);
	}	

	for(unsigned int index = 0; index < nObjects; index++)
	{	
		getNode = objects.get(index);
		if(!getNode)
		{
			return;
		}
	
		ptr = getNode->a;
		if(!ptr)
		{
			return;
		}
		Face *f = NULL;

		glPushMatrix();
		//glMultMatrixf(ptr->matrix.getMatrix());				
		
		//glTranslatef(ptr->position.x, ptr->position.y, ptr->position.z);
		//glRotatef(angle, 1.0f, 1.0f, 1.0f);

		for(i = 0; i < ptr->nFaces; i++)
		{
			f = &ptr->face[i];		

			//	vanhatapa:
			//v1 = &ptr->vertex[ptr->face[i].aa];
			//v2 = &ptr->vertex[ptr->face[i].bb];
			//v3 = &ptr->vertex[ptr->face[i].cc];
		
			// uusitapa
			v1 = f->a;
			v2 = f->b;
			v3 = f->c;

			glBindTexture(GL_TEXTURE_2D, f->texture->getID());
			glBegin(GL_TRIANGLES);

			if(v1 && v2 && v3)
			{	
				//glNormal3f(f->normal.x, f->normal.y, f->normal.z);
				glTexCoord2f(v1->u, v1->v); glNormal3f(v1->normal.x, v1->normal.y, v1->normal.z); glVertex3f(v1->position.x, v1->position.y, v1->position.z);
				glTexCoord2f(v2->u, v2->v); glNormal3f(v2->normal.x, v2->normal.y, v2->normal.z); glVertex3f(v2->position.x, v2->position.y, v2->position.z);
				glTexCoord2f(v3->u, v3->v); glNormal3f(v3->normal.x, v3->normal.y, v3->normal.z); glVertex3f(v3->position.x, v3->position.y, v3->position.z);
			}

			glEnd();

		}				

		glPopMatrix();
	}
	angle += 1.5f;
*/
}

/*
bool Loader3DS::operator >> (Scene *scene)
{
	Node <Material> *getMaterial = NULL;
	Node <Object> *getObject = NULL;
	Node <Camera> *getCamera = NULL;
	Node <Light> *getLight = NULL;
	Material *materialPtr = NULL;
	Object *objectPtr = NULL;
	Camera *cameraPtr = NULL;
	Light *lightPtr = NULL;
	unsigned int i;
	
	if(!scene->init(nMaterials, nObjects, nCameras, nLights)) return false;

	// Export materials 	
	for(i = 0; i < scene->nMaterials; i++)
	{	
		getMaterial = materials.get(i);
		if(getMaterial)
		{						
			Material *ptr = &scene->materials[i];

			materialPtr = getMaterial->a;
			if(materialPtr && ptr)
			{
				ptr->setFileName(materialPtr->getFileName());
				ptr->setName(materialPtr->getName());
			}
		}
	}

	// Export objects 
	for(i = 0; i < scene->nObjects; i++)
	{		
		getObject = objects.get(i);
		if(getObject)
		{						
			Object *ptr = &scene->objects[i];

			objectPtr = getObject->a;
			if(objectPtr && ptr)
			{
				unsigned int j, nVertices, nFaces;
				nVertices = objectPtr->getVertices();
				nFaces = objectPtr->getFaces();
			
				ptr->initVertices(nVertices);
				for(j = 0; j < nVertices; j++)
				{
					ptr->vertex[j] = objectPtr->vertex[j];
				}
				
				ptr->initFaces(nFaces);
				for(j = 0; j < nFaces; j++)
				{										
					ptr->face[j] = objectPtr->face[j];
					
					// Elvytetään osoittimet vertexeihin
					ptr->face[j].a = &ptr->vertex[ptr->face[j].aa];
					ptr->face[j].b = &ptr->vertex[ptr->face[j].bb];
					ptr->face[j].c = &ptr->vertex[ptr->face[j].cc];					
				}
			}
		}
	}

	// Export cameras 
	for(i = 0; i < scene->nCameras; i++)
	{		
		getCamera = cameras.get(i);		
		if(getCamera)
		{
			Camera *ptr = &scene->cameras[i];

			cameraPtr = getCamera->a;
			if(cameraPtr && ptr)
			{
				ptr->setPosition(cameraPtr->getPosition());
				ptr->setTarget(cameraPtr->getTarget());				
			}
		}
	}

	// Export lights 
	for(i = 0; i < scene->nLights; i++)
	{
		getLight = lights.get(i);
		if(getLight)
		{
			Light *ptr = &scene->lights[i];

			lightPtr = getLight->a;
			if(lightPtr && ptr)
			{
				float *pos = lightPtr->getPosition();
				float *amb = lightPtr->getAmbient();
				float *dif = lightPtr->getDiffuse();
								
				ptr->setPosition(Vector(pos[0], pos[1], pos[2]));
				ptr->setAmbient(amb[0], amb[1], amb[2]);
				ptr->setDiffuse(dif[0], dif[1], dif[2]);
				ptr->setIntensity(dif[3]);								
			}
		}
	}

	return true;
}
*/
bool Loader3DS::operator >> (Object *object)
{	
	Node <Object> *getObject = NULL;		
	Object *objectPtr = NULL;	
		
	getObject = objects.get(0);
	if(getObject)
	{								
		objectPtr = getObject->a;
		if(objectPtr && object)
		{
			unsigned int j, nVertices, nFaces;
			nVertices = objectPtr->getVertices();
			nFaces = objectPtr->getFaces();
			
			object->initVertices(nVertices);
			for(j = 0; j < nVertices; j++)
			{
				object->vertex[j] = objectPtr->vertex[j];
			}
				
			object->initFaces(nFaces);
			for(j = 0; j < nFaces; j++)
			{										
				object->face[j] = objectPtr->face[j];
				
				// Elvytetään osoittimet vertexeihin
				object->face[j].a = &object->vertex[object->face[j].aa];
				object->face[j].b = &object->vertex[object->face[j].bb];
				object->face[j].c = &object->vertex[object->face[j].cc];					
			}
		}
	}

	return true;
}
