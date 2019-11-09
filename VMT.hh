#include <immintrin.h>
#include <functional>
#include <cassert>
#include <cstdint>
#include <cstring>

/* Note: In this file you can see the whole VMT class.
   Usage of it is done in "VMT.cc" 
*/

/* Documentation from GuidedHacking, UC. */
/* Following Namsazo's class. */


class VMT_Hooking
{
public:
    constexpr VMT_Hooking() : NewVMT{ nullptr }, 
                              OldVMT{ nullptr } { }

    /* Delete post initialization. */
    ~VMT_Hooking() noexcept
    {
        if (NewVMT)
        {
            delete[] (NewVMT - 1);
        } 
    }

    /* Initialization */
    void Initialize(void **OriginalTable)
    {
        OldVMT = OriginalTable;

        int Size = 0;
        
        while (OldVMT[Size]) ++Size;

        NewVMT = (new void *[Size + 1]) + 1;
        std::memcpy(NewVMT - 1, OldVMT - 1, sizeof(void *) * (Size + 1));
    }

    /* Initialize and hook instance. */
    bool InitializeAndHook(void *Instance)
    {
        void **&VTable   = *reinterpret_cast<void ***>
                            (Instance);
        /* Declaration for initialization,
           always false unless told otherwise. */
        bool Initialized = false;

        if (!OldVMT)
        {
            /* Since there might be nothing to initialize,
               if there is no old VMT, run initialization. */
            Initialized = true;
            Initialize(VTable);  
        }

        /* Finally, hook the instance after it is initialized. */
        HookInstance(Instance);

        /* Return the state of initialization. */
        return Initialized;
    }


/* Constant Expressions */

    /* TIL: You can run the "||" operator on "assert". */
    constexpr void HookInstance(void *Instance)
    {
        void **&VTable = *reinterpret_cast<void ***>
                          (Instance);
        assert(VTable == OldVMT || VTable == NewVMT);
        /* Your stored VTable is passed as a new VMT. */
        VTable = NewVMT;
    }

    constexpr void UnhookInstance(void *Instance)
    {
        void **&VTable = *reinterpret_cast<void ***>
                          (Instance);
        assert(VTable == OldVMT || NewVMT);
        /* Your stored VTable is passed as an old VMT. */
        VTable = OldVMT;
    }

/* */

/* Templates */

    /* Hook the function. */
    template <typename Func>
    Func HookFunc(Func HookedFunc, const int Index)
    {
        NewVMT[Index] = reinterpret_cast<void *>
                        (HookedFunc);
        return reinterpret_cast<Func>
               (OldVMT[Index]);
    }

    /* Apply the hook. */
    template <typename Func>
    void ApplyHook(int Index)
    {
        Func::Original = reinterpret_cast<decltype(Func::Original)>
                         (HookFunc(&Func::Hooked, Index));
    }

    /* Get original func. */
    template <typename Func = std::uintptr_t>
    Func GetOriginalFunc(const int Index)
    {
        return reinterpret_cast<Func>
               (OldVMT[Index]);
    }

/* */
private:
    void **NewVMT = nullptr;
    void **OldVMT = nullptr;
};
