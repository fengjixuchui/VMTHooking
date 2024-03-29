#include "Others.hh"

/* Note: In this file I'm going through the Hooking
   process, declaring the class access point, handling
   the hook's Original object. The instances shouldn't
   be done in the same file, but this is just done
   to give out an example to what you have to do
   in order to run this on your project.
   
   Credit for this project: Cristei Gabriel-Marian; 
   Laxol; The UC Community as a whole; 
   GuidedHacking's community as a whole; 
   everyone who I talked to within the time of writing 
   this class.
   Licensed under the MIT license, to be used under
   your will if the credits are provided. Market
   use will not be supported on the GIT repository,
   as in you will likely not receive support from
   the user base and/or creator. We reserve
   the rights to do so.
*/

static VMT_Hooking ClassAccessPoint;
decltype(&Hooking::Example::Hooked) Hooking::Example::Original;

/* Check the note for "Instance".
   Static is recommended to be
   used for your actual instance
   so you don't call it everytime.
   The instance will always be
   the same. 
*/
static void *Instance;

/* Your entry point, to be called in the
   constructor / main thread.
*/
void HookEntryPoint()
{
    /* If Instance is initialized for ClassAccessPoint
       apply the hook using the Example struct under the
       index "1". */
    Hook(
        ClassAccessPoint,
        Instance,
        Hooking::Example,
        1
    );
}

/* Example of how to do a hook. */
void Hooking::Example::Hooked(void *Argument) 
{
    /* Call original */
    Hooking::Example::Original(Argument);
}
