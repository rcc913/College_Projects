/*//////////////////////////////////////////////////////////////////////
// File: semaphore.h
// --------------------------------------------------------------------
// This is a POSIX threads semaphore implementation.  See the docs
// at http://dis.cs.umass.edu/~wagner/threads_html/tutorial.html
// for information on POSIX threads and how to use this lib.
// --------------------------------------------------------------------
// Written by: Tom A. Wagner
//             wagner@cs.umass.edu
//             Multi-Agent Systems Lab
//             Department of Computer and Information Science
//             University of Massachusetts
//             Amherst, Massachusetts 01003.
//
// This code was written at the Multi-Agent Systems Lab. at the 
// Department of Computer Science, University of Massachusetts,
// Amherst, MA 01003.
//
// Copyright (c) 1996 UMASS CS Dept. All rights are reserved.
//
// Development of this code was partially supported by:
//    ONR grant N00014-92-J-1450
//    NSF grant IRI-9523419
//    DARPA grant, RaDEO program, 70NANB6H0074 as subcontractor for 
//          Boeing Helicoptor
////////////////////////////////////
//
//*/

#include <stdio.h>
#include <pthread.h>
#include <string>

#ifndef SEMAPHORES
#define SEMAPHORES

typedef struct Semaphore
{
    int         v;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}
Semaphore;


int         semaphore_down (Semaphore * s);
int         semaphore_up (Semaphore * s);
void        semaphore_destroy (Semaphore * s);
void        semaphore_init (Semaphore * s,int i);
int         semaphore_value (Semaphore * s);
int         tw_pthread_cond_signal (pthread_cond_t * c);
int         tw_pthread_cond_wait (pthread_cond_t * c, pthread_mutex_t * m);
int         tw_pthread_mutex_unlock (pthread_mutex_t * m);
int         tw_pthread_mutex_lock (pthread_mutex_t * m);
//void        do_error (char *msg);
void        do_error (std::string msg);

#endif


/*//////////////////////////////////////////////////////////////////////
// File: semaphore.c
// --------------------------------------------------------------------
// This is a POSIX threads semaphore implementation.  See the docs
// at http://dis.cs.umass.edu/~wagner/threads_html/tutorial.html
// for information on POSIX threads and how to use this lib.
// --------------------------------------------------------------------
// Written by: Tom A. Wagner
//             wagner@cs.umass.edu
//             Multi-Agent Systems Lab
//             Department of Computer and Information Science
//             University of Massachusetts
//             Amherst, Massachusetts 01003.
//
// This code was written at the Multi-Agent Systems Lab. at the 
// Department of Computer Science, University of Massachusetts, 
// Amherst, MA 01003.
//
// Copyright (c) 1996 UMASS CS Dept. All rights are reserved.
//
// Development of this code was partially supported by:
//    ONR grant N00014-92-J-1450
//    NSF grant IRI-9523419
//    DARPA grant, RaDEO program, 70NANB6H0074 as subcontractor for 
//          Boeing Helicoptor
////////////////////////////////////
//
//*/

#include "semaphore.h"


/*
 * function must be called prior to semaphore use.
 *
 */
void
semaphore_init (Semaphore * s,int i)
{
    s->v = i;
    if (pthread_mutex_init (&(s->mutex), 0) == -1)
     do_error ("Error setting up semaphore mutex");

    if (pthread_cond_init (&(s->cond), 0) == -1)
     do_error ("Error setting up semaphore condition signal");
}

/*
 * function should be called when there is no longer a need for
 * the semaphore.
 *
 */
void
semaphore_destroy (Semaphore * s)
{
    if (pthread_mutex_destroy (&(s->mutex)) == -1)
     do_error ("Error destroying semaphore mutex");

    if (pthread_cond_destroy (&(s->cond)) == -1)
     do_error ("Error destroying semaphore condition signal");
}

/*
 * function increments the semaphore and signals any threads that
 * are blocked waiting a change in the semaphore.
 *
 */
int
semaphore_up (Semaphore * s)
{
    int         value_after_op;

    tw_pthread_mutex_lock (&(s->mutex));

    (s->v)++;
    value_after_op = s->v;

    tw_pthread_mutex_unlock (&(s->mutex));
    tw_pthread_cond_signal (&(s->cond));

    return( value_after_op );
}

/*
 * function decrements the semaphore and blocks if the semaphore is
 * <= 0 until another thread signals a change.
 *
 */
int
semaphore_down (Semaphore * s)
{
    int         value_after_op;

    tw_pthread_mutex_lock (&(s->mutex));
    while (s->v <= 0)
    {
     tw_pthread_cond_wait (&(s->cond), &(s->mutex));
    }

    (s->v)--;
    value_after_op = s->v;

    tw_pthread_mutex_unlock (&(s->mutex));

    return (value_after_op);
}

/*
 * function does NOT block but simply decrements the semaphore.
 * should not be used instead of down -- only for programs where
 * multiple threads must up on a semaphore before another thread
 * can go down, i.e., allows programmer to set the semaphore to
 * a negative value prior to using it for synchronization.
 *
 */
int
semaphore_decrement (Semaphore * s)
{
    int         value_after_op;

    tw_pthread_mutex_lock (&(s->mutex));
    s->v--;
    value_after_op = s->v;
    tw_pthread_mutex_unlock (&(s->mutex));

    return (value_after_op);
}

/*
 * function returns the value of the semaphore at the time the
 * critical section is accessed.  obviously the value is not guarenteed
 * after the function unlocks the critical section.  provided only
 * for casual debugging, a better approach is for the programmar to
 * protect one semaphore with another and then check its value.
 * an alternative is to simply record the value returned by semaphore_up
 * or semaphore_down.
 *
 */
int
semaphore_value (Semaphore * s)
{
    /* not for sync */
    int         value_after_op;

    tw_pthread_mutex_lock (&(s->mutex));
    value_after_op = s->v;
    tw_pthread_mutex_unlock (&(s->mutex));

    return (value_after_op);
}



/* -------------------------------------------------------------------- */
/* The following functions replace standard library functions in that   */
/* they exit on any error returned from the system calls.  Saves us     */
/* from having to check each and every call above.                      */
/* -------------------------------------------------------------------- */


int
tw_pthread_mutex_unlock (pthread_mutex_t * m)
{
    int         return_value;

    if ((return_value = pthread_mutex_unlock (m)) == -1)
     do_error ("pthread_mutex_unlock");

    return (return_value);
}

int
tw_pthread_mutex_lock (pthread_mutex_t * m)
{
    int         return_value;

    if ((return_value = pthread_mutex_lock (m)) == -1)
     do_error ("pthread_mutex_lock");

    return (return_value);
}

int
tw_pthread_cond_wait (pthread_cond_t * c, pthread_mutex_t * m)
{
    int         return_value;

    if ((return_value = pthread_cond_wait (c, m)) == -1)
     do_error ("pthread_cond_wait");

    return (return_value);
}

int
tw_pthread_cond_signal (pthread_cond_t * c)
{
    int         return_value;

    if ((return_value = pthread_cond_signal (c)) == -1)
     do_error ("pthread_cond_signal");

    return (return_value);
}


/*
 * function just prints an error message and exits 
 *
 */
void 
do_error (std::string msg) 
{
    perror (msg.c_str());
    exit (1);
}
