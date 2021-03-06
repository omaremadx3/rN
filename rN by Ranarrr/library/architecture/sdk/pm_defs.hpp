#ifndef PM_DEFS_HPP
#define PM_DEFS_HPP

#pragma once

#include "usercmd.hpp"
#include "movevars.hpp"
#include "util_vector.hpp"
#include "com_model.hpp"
#include "pmtrace.hpp"

#define	MAX_PHYSENTS		600 			// Must have room for all entities in the world.
#define MAX_MOVEENTS		64
#define	MAX_CLIP_PLANES		5

#define PM_NORMAL			0x00000000
#define PM_STUDIO_IGNORE	0x00000001		// Skip studio models
#define PM_STUDIO_BOX		0x00000002		// Use boxes for non-complex studio models (even in traceline)
#define PM_GLASS_IGNORE		0x00000004		// Ignore entities with non-normal rendermode
#define PM_WORLD_ONLY		0x00000008		// Only trace against the world

// Values for flags parameter of PM_TraceLine
#define PM_TRACELINE_ANYVISIBLE		0
#define PM_TRACELINE_PHYSENTSONLY	1

struct physent_s {
	char			name[ 32 ];             // Name of model, or "player" or "world".
	int				player;
	Vector			origin;					// Model's origin in world coordinates.
	model_s			*model;					// only for bsp models
	model_s			*studiomodel;			// SOLID_BBOX, but studio clip intersections.
	Vector			mins, maxs;				// only for non-bsp models
	int				info;					// For client or server to use to identify (index into edicts or cl_entities)
	Vector			angles;					// rotated entities need this info for hull testing to work.

	int				solid;					// Triggers and func_door type WATER brushes are SOLID_NOT
	int				skin;					// BSP Contents for such things like fun_door water brushes.
	int				rendermode;				// So we can ignore glass

	// Complex collision detection.
	float			frame;
	int				sequence;
	unsigned char	control[ 4 ];
	unsigned char	blending[ 2 ];

	int				movetype;
	int				takedamage;
	int				blooddecal;
	int				team;
	int				classnumber;

	// For mods
	int				iuser1;
	int				iuser2;
	int				iuser3;
	int				iuser4;
	float			fuser1;
	float			fuser2;
	float			fuser3;
	float			fuser4;
	Vector			vuser1;
	Vector			vuser2;
	Vector			vuser3;
	Vector			vuser4;
};

struct playermove_s {
	int				player_index;  // So we don't try to run the PM_CheckStuck nudging too quickly.					0x000
	int				server;        // For debugging, are we running physics code on server side?					0x004

	int				multiplayer;   // 1 == multiplayer server														0x008
	float			time;          // realtime on host, for reckoning duck timing									0x00B
	float			frametime;	   // Duration of this frame														0x00F

	Vector			forward, right, up; // Vectors for angles														0x013
	// player state
	Vector			origin;        // Movement origin.																0x0
	Vector			angles;        // Movement view angles.
	Vector			oldangles;     // Angles before movement view angles were looked at.
	Vector			velocity;      // Current movement direction.
	Vector			movedir;       // For waterjumping, a forced forward velocity so we can fly over lip of ledge.
	Vector			basevelocity;  // Velocity of the conveyor we are standing, e.g.

	// For ducking/dead
	Vector			view_ofs;      // Our eye position.
	float			flDuckTime;    // Time we started duck
	int				bInDuck;       // In process of ducking or ducked already?

	// For walking/falling
	int				flTimeStepSound;  // Next time we can play a step sound
	int				iStepLeft;

	float			flFallVelocity;
	Vector			punchangle;

	float			flSwimTime;

	float			flNextPrimaryAttack;

	int				effects;		// MUZZLE FLASH, e.g.

	int				flags;         // FL_ONGROUND, FL_DUCKING, etc.
	int				usehull;       // 0 = regular player hull, 1 = ducked player hull, 2 = point hull
	float			gravity;       // Our current gravity and friction.
	float			friction;
	int				oldbuttons;    // Buttons last usercmd
	float			waterjumptime; // Amount of time left in jumping out of water cycle.
	int				dead;          // Are we a dead player?
	int				deadflag;
	int				spectator;     // Should we use spectator physics model?
	int				movetype;      // Our movement type, NOCLIP, WALK, FLY

	int				onground;
	int				waterlevel;
	int				watertype;
	int				oldwaterlevel;

	char			sztexturename[ 256 ];
	char			chtexturetype;

	float			maxspeed;
	float			clientmaxspeed; // Player specific maxspeed

	// For mods
	int				iuser1;
	int				iuser2;
	int				iuser3;
	int				iuser4;
	float			fuser1;
	float			fuser2;
	float			fuser3;
	float			fuser4;
	Vector			vuser1;
	Vector			vuser2;
	Vector			vuser3;
	Vector			vuser4;

	// world state
	int				numphysent; // Number of entities to clip against.
	physent_s		physents[ MAX_PHYSENTS ]; // Number of momvement entities (ladders)
	
	int				nummoveent;
	
	physent_s		moveents[ MAX_MOVEENTS ]; // just a list of ladders

	
	int				numvisent; // All things being rendered, for tracing against things you don't actually collide with
	physent_s		visents[ MAX_PHYSENTS ];

	usercmd_s		cmd; // input to run through physics.

	int				numtouch; // Trace results for objects we collided with.
	pmtrace_s		touchindex[ MAX_PHYSENTS ];

	char			physinfo[ MAX_PHYSINFO_STRING ]; // Physics info string

	movevars_s		*movevars;
	Vector			player_mins[ 4 ];
	Vector			player_maxs[ 4 ];

	// Common functions
	const char		*( *PM_Info_ValueForKey )	( const char *s, const char *key );
	void			( *PM_Particle )			( Vector origin, int color, float life, int zpos, int zvel );
	int				( *PM_TestPlayerPosition )	( Vector pos, pmtrace_s *ptrace );
	void			( *Con_NPrintf )			( int idx, char *fmt, ... );
	void			( *Con_DPrintf )			( char *fmt, ... );
	void			( *Con_Printf )				( char *fmt, ... );
	double			( *Sys_FloatTime )			();
	void			( *PM_StuckTouch )			( int hitent, pmtrace_s *ptraceresult );
	int				( *PM_PointContents )		( Vector p, int *truecontents ); // truecontents is filled in if it is non-null
	int				( *PM_TruePointContents )	( Vector p );
	int				( *PM_HullPointContents )	( hull_s *hull, int num, Vector p );
	pmtrace_s		( *PM_PlayerTrace )			( Vector start, Vector end, int traceFlags, int ignore_pe );
	pmtrace_s		*( *PM_TraceLine )			( Vector start, Vector end, int flags, int usehull, int ignore_pe );
	long			( *RandomLong )				( long lLow, long lHigh );
	float			( *RandomFloat )			( float flLow, float flHigh );
	int				( *PM_GetModelType )		( model_s *mod );
	void			( *PM_GetModelBounds )		( model_s *mod, Vector mins, Vector maxs );
	void			*( *PM_HullForBsp )			( physent_s *pe, Vector offset );
	float			( *PM_TraceModel )			( physent_s *pEnt, Vector start, Vector end, trace_s *trace );
	int				( *COM_FileSize )			( char *filename );
	unsigned char	*( *COM_LoadFile )			( char *path, int usehunk, int *pLength );
	void			( *COM_FreeFile )			( void *buffer );
	char			*( *memfgets )				( unsigned char *pMemFile, int fileSize, int *pFilePos, char *pBuffer, int bufferSize );

	// Functions
	int				runfuncs; // Run functions for this frame?
	void			( *PM_PlaySound )			( int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch );
	const char		*( *PM_TraceTexture )		( int ground, Vector vstart, Vector vend );
	void			( *PM_PlaybackEventFull )	( int flags, int clientindex, unsigned short eventindex, float delay, Vector origin, Vector angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
};

#endif // !PM_DEFS_HPP