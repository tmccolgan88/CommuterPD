/*
*  Particle Framework
*
*  Author : Tim McColgan
*  Date : 04/27/2022
*
*  Purpose : Provide a set of structs and functions to create/upate/draw particles
*
*  A group of particles (burst), is stored as a linked list of individual particles.  Those lists are children
*  to a ParticleBurst linked list.
*/

#include "pd_api.h"
#include "particles.h"
#include "../globals.h"

typedef struct Particle
{
	LCDBitmap *bmp;
	int x, y;
	int dx, dy;
	int deltaTime;
	int timer;

} Particle;

typedef struct ParticleListNode
{
	Particle *p;
	struct ParticleListNode *next;

} ParticleListNode;

typedef struct ParticleBurstNode
{
	ParticleListNode* burstHead;
	struct ParticleBurstNode* next;
} ParticleBurstNode;

ParticleBurstNode *particleBurstHead = NULL;
ParticleBurstNode *particleBurstCurrent = NULL;

/*
* adds a burst of particles that disperse at random angles and speeds
*
* @param LCDBitmap *particleBMP - the bitmap of the particle image
* @param int x - the x location of the center of the burst
* @param int y - the y loaction of the center of the burst
*
* return - void
*/
void addParticleBurst(LCDBitmap *particleBMP, int x, int y)
{
	ParticleListNode *headParticle = NULL;
    ParticleListNode *currentParticle = NULL;
    p->system->logToConsole("creating particle burst");
	int i = 0;
	int numParticles = 6;

	for (i = 0; i < numParticles; i++)
	{
		Particle* particle = p->system->realloc(NULL, sizeof(Particle));
		particle->bmp = particleBMP;
		particle->x = x;
		particle->y = y;
		particle->dx = (rand() % 20) - 10;
		particle->dy = (rand() % 20) - 10;
		particle->timer = 200;
		particle->deltaTime = 0;

		ParticleListNode* node;
		node = p->system->realloc(NULL, sizeof(ParticleListNode));
		node->p = particle;
		node->next = NULL; 

		if (headParticle == NULL)
		{
			headParticle = node;
            headParticle->next = NULL;

			currentParticle = headParticle;
		}
		else
		{
			currentParticle->next = node;
			currentParticle = currentParticle->next;
		}
	}

    ParticleBurstNode* burstNode = p->system->realloc(NULL, sizeof(ParticleBurstNode));
    burstNode->burstHead = headParticle;
    burstNode->next = NULL;

    if (particleBurstHead == NULL)
    {
        particleBurstHead = burstNode;
        particleBurstHead->next = NULL;

        particleBurstCurrent = particleBurstHead;
    }
    else
    {
        particleBurstCurrent->next = burstNode;
        particleBurstCurrent = particleBurstCurrent->next;
        particleBurstCurrent->next = NULL;
    }
}

void addTeleportParticleBurst(LCDBitmap *particleBMP, int x, int y) 
{
    ParticleListNode *headParticle = NULL;
    ParticleListNode *currentParticle = NULL;

	int i = 0;
	int numParticles = 22; //(rand() % 14) + 6;

	for (i = 0; i < numParticles; i++)
	{
		Particle* particle = p->system->realloc(NULL, sizeof(Particle));
		particle->bmp = particleBMP;
		particle->x = x + (rand() % 20) - 10;
		particle->y = y + (rand() % 20) - 10;
		particle->dx = (rand() % 4) - 1;
		particle->dy = 0; //(rand() % 6) - 3;
		particle->timer = 200;
		particle->deltaTime = 0;

		ParticleListNode* node;
		node = p->system->realloc(NULL, sizeof(ParticleListNode));
		node->p = particle;
		node->next = NULL; 

		if (headParticle == NULL)
		{
			headParticle = node;
            headParticle->next = NULL;

			currentParticle = headParticle;
		}
		else
		{
			currentParticle->next = node;
			currentParticle = currentParticle->next;
		}
	}

    ParticleBurstNode* burstNode = p->system->realloc(NULL, sizeof(ParticleBurstNode));
    burstNode->burstHead = headParticle;
    burstNode->next = NULL;

    if (particleBurstHead == NULL)
    {
        particleBurstHead = burstNode;
        particleBurstHead->next = NULL;

        particleBurstCurrent = particleBurstHead;
    }
    else
    {
        particleBurstCurrent->next = burstNode;
        particleBurstCurrent = particleBurstCurrent->next;
        particleBurstCurrent->next = NULL;
    }
}

void updateParticles(int deltaTime)
{
    ParticleBurstNode* headBurstNode = particleBurstHead;
    ParticleListNode* particleNode = NULL;

    while (headBurstNode != NULL)
    {
        particleNode = headBurstNode->burstHead;

        while (particleNode != NULL)
        {
            particleNode->p->x += particleNode->p->dx;
		    particleNode->p->y += particleNode->p->dy;
		    particleNode->p->deltaTime += deltaTime;
            particleNode = particleNode->next;
        }

        headBurstNode = headBurstNode->next;
    }

}

void removeAllParticles()
{
    ParticleBurstNode* headBurstNode = particleBurstHead;
    ParticleBurstNode* burstSaveNode = NULL;
    ParticleListNode* particleNode = NULL;
    ParticleListNode* savedNode = NULL;

    while (headBurstNode != NULL)
    {
        particleNode = headBurstNode->burstHead;

	    while (particleNode != NULL)
	    {
            savedNode = particleNode->next;

		    p->system->realloc(particleNode, sizeof(ParticleBurstNode));
		
		    particleNode = savedNode;
	    }

        burstSaveNode = headBurstNode;
        headBurstNode = headBurstNode->next;
        p->system->realloc(burstSaveNode, sizeof(ParticleBurstNode));

    }
    particleBurstHead = NULL;
}

int drawParticles()
{
	ParticleBurstNode* headBurstNode = particleBurstHead;
    ParticleListNode* particleNode = NULL;

    while (headBurstNode != NULL)
    {
        particleNode = headBurstNode->burstHead;

	    while (particleNode != NULL)
	    {
		    if (particleNode->p->deltaTime < particleNode->p->timer)
			    p->graphics->drawBitmap(particleNode->p->bmp, particleNode->p->x, particleNode->p->y, kBitmapUnflipped);
		
		    particleNode = particleNode->next;
	    }

        headBurstNode = headBurstNode->next;
    }
	return 1;
}