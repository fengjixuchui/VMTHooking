#include "VMT.hh"

/* Note: In this file I'm sorting all of my "dependencies"
   if you may, so we can have a clean file where the
   hooking is done. 
*/


/* Macros. */
/* Documented in "VMT.cc" */
#define Hook(AccessPoint, Instance, Structure, Index) \
        if (AccessPoint.InitializeAndHook(Instance))  \
        {                                             \
            AccessPoint.ApplyHook<Structure>(Index);  \
        }

/* Handle hook objects. */
namespace Hooking
{
    struct Example
    {
        using Func = void(void *); /* Datatype(Argument Datatypes)
        static Func Hooked;        /* Name used in "VMT.hh" */
        static Func *Original;     /* Name used in "VMT.hh" */
    };
}
