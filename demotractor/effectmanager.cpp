//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#ifdef _DEBUG	
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "effectmanager.hpp"
#include "globals.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Static members
//--------------------------------------------------------------------------------------------

bool EffectManager::createdFlag = false;
EffectManager *EffectManager::instance = NULL;

//--------------------------------------------------------------------------------------------
//  Class code
//--------------------------------------------------------------------------------------------

EffectManager::EffectManager()
{
	nEffects = 0;	
}

EffectManager::~EffectManager()
{
}

EffectManager *EffectManager::create()
{
	if(!createdFlag)
	{				
		dmsMsg("EffectManager::create(): EffectManager instance created\n");
		
		instance = new EffectManager;
		if(!instance)
		{
			throw "EffectManager::create(): Memory allocation error";
			return NULL;
		}

		createdFlag = true;
	}
	else
	{		
		throw "EffectManager::create(): One instance of class EffectManager is allready running";
	}	

	return instance;
}

bool EffectManager::initEffects()
{
	Node <Base> *getNode = NULL;
	Base *effect = NULL;
	unsigned int i;

	for(i = 0; i < effects.getNodeCount(); i++)
	{
		getNode = effects.get(i);
		if(getNode)
		{
			effect = getNode->a;
			if(effect)
			{
				unsigned long s, e;
				s = effect->startTime;
				e = effect->endTime;
				if(!effect->init(s, e)) return false;
			}
		}
	}

	return true;
}

bool EffectManager::addEffect(unsigned long s, unsigned long e, Base *effect, char *name)
{
	int size = strlen(name);
						
	effect->startTime = s;
	effect->endTime = e;
	effects.addTail(effect, name);
	nEffects++;

	dmsMsg("EffectManager::addEffect(): Effect \"%s\" added\n", name);
			
	return true;
}

unsigned int EffectManager::getEffectCount()
{
	return nEffects;
}

Base *EffectManager::getEffect(unsigned int index)
{
	Node<Base> *getNode = NULL;

	(index > nEffects-1) ? index = nEffects-1 : index = index;
	(index < 0) ? index = 0 : index = index;
	
	getNode = effects.get(index);

	if(getNode) return getNode->a;

	return NULL;
}

void EffectManager::release()
{
	unsigned int i;
	Node<Base> *getNode;
	Base *effect = NULL;

	if(createdFlag)
	{

		for(i = 0; i < nEffects; i++)
		{
			getNode = effects.get(i);
			effect = getNode->a;

			if(effect)
			{
				effect->release();
				effect = NULL;
			}
		}

		effects.freeList();

		createdFlag = false;
		nEffects = 0;

		if(instance)
		{
			delete instance;
			instance = NULL;
		}

		dmsMsg("EffectManager::release(): Instance deleted\n");		
	}
}

void EffectManager::debug()
{
	unsigned int i;
	Node<Base> *getNode;
	Base *effect;

	printf("\nEffects in EffectManager(%d): \n\n", nEffects);
	for(i = 0; i < nEffects; i++)
	{
		getNode = effects.get(i);
		effect = getNode->a;
		dmsMsg("Effect nro. %d, name %s\n", i, getNode->name);
	}		
}