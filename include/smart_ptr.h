#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

#include <functional>
#include <stdexcept>
#include <cassert>

//#define SMARTPTR_CONVERSION_CONSTRUCTOR_POLICY


////////////////////////////////////////////////////////////////////////////////
// Helper structure for the STATIC_CHECK macro
////////////////////////////////////////////////////////////////////////////////
template<int> struct CompileTimeError;
template<> struct CompileTimeError<true> {};

////////////////////////////////////////////////////////////////////////////////
// macro STATIC_CHECK
// Invocation: STATIC_CHECK(expr, id)
// where:
// expr is a compile-time integral or pointer expression
// id is a C++ identifier that does not need to be defined
// If expr is zero, id will appear in a compile-time error message.
////////////////////////////////////////////////////////////////////////////////
#define STATIC_CHECK(expr, msg) \
    { CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 

////////////////////////////////////////////////////////////////////////////////
// class template Select
// Selects one of two types based upon a boolean constant
// Invocation: Select<flag, T, U>::Result
// where:
// flag is a compile-time boolean constant
// T and U are types
// Result evaluates to T if flag is true, and to U otherwise.
////////////////////////////////////////////////////////////////////////////////
template <bool flag, typename T, typename U>
struct Select
{
	typedef T Result;
};

template <typename T, typename U>
struct Select<false, T, U>
{
	typedef U Result;
};


////////////////////////////////////////////////////////////////////////////////
// Helper types Small and Big - guarantee that sizeof(Small) < sizeof(Big)
////////////////////////////////////////////////////////////////////////////////
namespace Private
{
	template <class T, class U>
	struct ConversionHelper
	{
		typedef char Small;
		struct Big { char dummy[2]; };
		static Big   Test(...);
		static Small Test(U);
		static T MakeT();
	};
}


////////////////////////////////////////////////////////////////////////////////
// class template DefaultSPStorage
// Implementation of the StoragePolicy used by CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    template <class T>
    class DefaultSPStorage
    {
    public:
        typedef T* StoredType;    // the type of the pointee_ object
        typedef T* PointerType;   // type returned by operator->
        typedef T& ReferenceType; // type returned by operator*

        DefaultSPStorage() : pointee_(Default()) 
        {}

        // The storage policy doesn't initialize the stored pointer 
        //     which will be initialized by the OwnershipPolicy's Clone fn
        DefaultSPStorage(const DefaultSPStorage&)
        {}

        template <class U>
        DefaultSPStorage(const DefaultSPStorage<U>&) 
        {}
        
        DefaultSPStorage(const StoredType& p) : pointee_(p) {}
        
        PointerType operator->() const { return pointee_; }
        
        ReferenceType operator*() const { return *pointee_; }
        
        void Swap(DefaultSPStorage& rhs)
        { std::swap(pointee_, rhs.pointee_); }
    
        // Accessors
        friend inline PointerType GetImpl(const DefaultSPStorage& sp)
        { return sp.pointee_; }
        
        friend inline const StoredType& GetImplRef(const DefaultSPStorage& sp)
        { return sp.pointee_; }

        friend inline StoredType& GetImplRef(DefaultSPStorage& sp)
        { return sp.pointee_; }

    protected:
        // Destroys the data stored
        // (Destruction might be taken over by the OwnershipPolicy)
        void Destroy()
        { delete pointee_; }
        
        // Default value to initialize the pointer
        static StoredType Default()
        { return 0; }
    
    private:
        // Data
        StoredType pointee_;
    };


////////////////////////////////////////////////////////////////////////////////
// class template RefCounted
// Implementation of the OwnershipPolicy used by CSmartPtr
// Provides a classic external reference counting implementation
////////////////////////////////////////////////////////////////////////////////
    template <class P>
    class RefCounted
    {
    public:
        RefCounted() 
        {
            pCount_ = new unsigned int;
            assert(pCount_);
            *pCount_ = 1;
        }
        
        RefCounted(const RefCounted& rhs) 
        : pCount_(rhs.pCount_)
        {}
        
        // MWCW lacks template friends, hence the following kludge
        template <typename P1>
        RefCounted(const RefCounted<P1>& rhs) 
        : pCount_(reinterpret_cast<const RefCounted&>(rhs).pCount_)
        {}
        
        P Clone(const P& val)
        {
            ++*pCount_;
            return val;
        }
        
        bool Release(const P&)
        {
            if (!--*pCount_)
            {
                delete pCount_;
                return true;
            }
            return false;
        }
        
        void Swap(RefCounted& rhs)
        { std::swap(pCount_, rhs.pCount_); }
    
        enum { destructiveCopy = false };

    private:
        // Data
        unsigned int* pCount_;
    };


////////////////////////////////////////////////////////////////////////////////
// class template AllowConversion
// Implementation of the ConversionPolicy used by CSmartPtr
// Allows implicit conversion from CSmartPtr to the pointee type
////////////////////////////////////////////////////////////////////////////////
    struct AllowConversion
    {
        enum { allow = true };

        void Swap(AllowConversion&)
        {}
    };


////////////////////////////////////////////////////////////////////////////////
// class template DisallowConversion
// Implementation of the ConversionPolicy used by CSmartPtr
// Does not allow implicit conversion from CSmartPtr to the pointee type
// You can initialize a DisallowConversion with an AllowConversion
////////////////////////////////////////////////////////////////////////////////
    struct DisallowConversion
    {
        DisallowConversion()
        {}
        
        DisallowConversion(const AllowConversion&)
        {}
        
        enum { allow = false };

        void Swap(DisallowConversion&)
        {}
    };


////////////////////////////////////////////////////////////////////////////////
// class template NoCheck
// Implementation of the CheckingPolicy used by CSmartPtr
// Well, it's clear what it does :o)
////////////////////////////////////////////////////////////////////////////////
    template <class P>
    struct NoCheck
    {
        NoCheck()
        {}
        
        template <class P1>
        NoCheck(const NoCheck<P1>&)
        {}
        
        static void OnDefault(const P&)
        {}

        static void OnInit(const P&)
        {}

        static void OnDereference(const P&)
        {}

        static void Swap(NoCheck&)
        {}
    };


////////////////////////////////////////////////////////////////////////////////
// class template AssertCheck
// Implementation of the CheckingPolicy used by CSmartPtr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////
    template <class P>
    struct AssertCheck
    {
        AssertCheck()
        {}
        
        template <class P1>
        AssertCheck(const AssertCheck<P1>&)
        {}
        
        template <class P1>
        AssertCheck(const NoCheck<P1>&)
        {}
        
        static void OnDefault(const P&)
        {}

        static void OnInit(const P&)
        {}

        static void OnDereference(P val)
        { assert(val); (void)val; }

        static void Swap(AssertCheck&)
        {}
    };


////////////////////////////////////////////////////////////////////////////////
// class template AssertCheckStrict
// Implementation of the CheckingPolicy used by CSmartPtr
// Checks the pointer against zero upon initialization and before dereference
// You can initialize an AssertCheckStrict with an AssertCheck 
////////////////////////////////////////////////////////////////////////////////
    template <class P>
    struct AssertCheckStrict
    {
        AssertCheckStrict()
        {}
        
        template <class U>
        AssertCheckStrict(const AssertCheckStrict<U>&)
        {}
        
        template <class U>
        AssertCheckStrict(const AssertCheck<U>&)
        {}
        
        template <class P1>
        AssertCheckStrict(const NoCheck<P1>&)
        {}
        
        static void OnDefault(P val)
        { assert(val); }
        
        static void OnInit(P val)
        { assert(val); }
        
        static void OnDereference(P val)
        { assert(val); }
        
        static void Swap(AssertCheckStrict&)
        {}
    };


////////////////////////////////////////////////////////////////////////////////
// class NullPointerException
// Used by some implementations of the CheckingPolicy used by CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    struct NullPointerException : public std::runtime_error
    {
        NullPointerException() : std::runtime_error("")
        { }
        const char* what() const throw()
        { return "Null Pointer Exception"; }
    };

  
////////////////////////////////////////////////////////////////////////////////
// class template RejectNullStatic
// Implementation of the CheckingPolicy used by CSmartPtr
// Checks the pointer upon initialization and before dereference
////////////////////////////////////////////////////////////////////////////////
    template <class P>
    struct RejectNullStatic
    {
        RejectNullStatic()
        {}
        
        template <class P1>
        RejectNullStatic(const RejectNullStatic<P1>&)
        {}
        
        template <class P1>
        RejectNullStatic(const NoCheck<P1>&)
        {}
        
        template <class P1>
        RejectNullStatic(const AssertCheck<P1>&)
        {}
        
        template <class P1>
        RejectNullStatic(const AssertCheckStrict<P1>&)
        {}
        
        static void OnDefault(const P&)
        {
            // Make it depended on template parameter
            static const bool DependedFalse = sizeof(P*) == 0;

            STATIC_CHECK(DependedFalse, ERROR_This_Policy_Does_Not_Allow_Default_Initialization);
        }
        
        static void OnInit(const P& val)
        { if (!val) throw NullPointerException(); }
        
        static void OnDereference(const P& val)
        { if (!val) throw NullPointerException(); }
        
        static void Swap(RejectNullStatic&)
        {}
    };


////////////////////////////////////////////////////////////////////////////////
// class template RejectNull
// Implementation of the CheckingPolicy used by CSmartPtr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////
    template <class P>
    struct RejectNull
    {
        RejectNull()
        {}
        
        template <class P1>
        RejectNull(const RejectNull<P1>&)
        {}
        
        static void OnInit(P val)
        { if (!val) throw NullPointerException(); }

        static void OnDefault(P val)
        { OnInit(val); }
        
        void OnDereference(P val)
        { OnInit(val); }
        
        void Swap(RejectNull&)
        {}        
    };


////////////////////////////////////////////////////////////////////////////////
// class template RejectNullStrict
// Implementation of the CheckingPolicy used by CSmartPtr
// Checks the pointer upon initialization and before dereference
////////////////////////////////////////////////////////////////////////////////
    template <class P>
    struct RejectNullStrict
    {
        RejectNullStrict()
        {}
        
        template <class P1>
        RejectNullStrict(const RejectNullStrict<P1>&)
        {}
        
        template <class P1>
        RejectNullStrict(const RejectNull<P1>&)
        {}
        
        static void OnInit(P val)
        { if (!val) throw NullPointerException(); }

        void OnDereference(P val)
        { OnInit(val); }
        
        void Swap(RejectNullStrict&)
        {}        
    };


////////////////////////////////////////////////////////////////////////////////
// class template ByRef
// Transports a reference as a value
// Serves to implement the Colvin/Gibbons trick for CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    template <class T>
    class ByRef
    {
    public:
        ByRef(T& v) : value_(v) {}
        operator T&() { return value_; }
        // gcc doesn't like this:
        // operator const T&() const { return value_; }
    private:
        ByRef& operator=(const ByRef &);
        T& value_;
    };


////////////////////////////////////////////////////////////////////////////////
// class template CSmartPtr (declaration)
// The reason for all the fuss above
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OwnershipPolicy = RefCounted,
        class ConversionPolicy = DisallowConversion,
        template <class> class CheckingPolicy = AssertCheck,
        template <class> class StoragePolicy = DefaultSPStorage
    >
    class CSmartPtr;


////////////////////////////////////////////////////////////////////////////////
// class template CSmartPtrDef (definition)
// this class added to unify the usage of CSmartPtr 
// instead of writing CSmartPtr<T,OP,CP,KP,SP> write CSmartPtrDef<T,OP,CP,KP,SP>::type
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OwnershipPolicy = RefCounted,
        class ConversionPolicy = DisallowConversion,
        template <class> class CheckingPolicy = AssertCheck,
        template <class> class StoragePolicy = DefaultSPStorage
    >
    struct CSmartPtrDef
    {
        typedef CSmartPtr
        <
            T,
            OwnershipPolicy,
            ConversionPolicy,
            CheckingPolicy,
            StoragePolicy
        >
        type;
    };


////////////////////////////////////////////////////////////////////////////////
// class template CSmartPtr (definition)
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OwnershipPolicy,
        class ConversionPolicy,
        template <class> class CheckingPolicy,
        template <class> class StoragePolicy
    >
    class CSmartPtr
        : public StoragePolicy<T>
        , public OwnershipPolicy<typename StoragePolicy<T>::PointerType>
        , public CheckingPolicy<typename StoragePolicy<T>::StoredType>
        , public ConversionPolicy
    {
        typedef StoragePolicy<T> SP;
        typedef OwnershipPolicy<typename StoragePolicy<T>::PointerType> OP;
        typedef CheckingPolicy<typename StoragePolicy<T>::StoredType> KP;
        typedef ConversionPolicy CP;
        
    public:
        typedef typename SP::PointerType PointerType;
        typedef typename SP::StoredType StoredType;
        typedef typename SP::ReferenceType ReferenceType;
        
        typedef typename Select<OP::destructiveCopy, 
                CSmartPtr, const CSmartPtr>::Result
            CopyArg;
    
    private:
        struct NeverMatched;
       
#ifdef SMARTPTR_CONVERSION_CONSTRUCTOR_POLICY
        typedef typename Select< CP::allow, const StoredType&, NeverMatched>::Result ImplicitArg;
        typedef typename Select<!CP::allow, const StoredType&, NeverMatched>::Result ExplicitArg;
#else
        typedef const StoredType& ImplicitArg;
        typedef typename Select<false, const StoredType&, NeverMatched>::Result ExplicitArg;
#endif

    public:

        CSmartPtr()
        { KP::OnDefault(GetImpl(*this)); }
        
        explicit
        CSmartPtr(ExplicitArg p) : SP(p)
        { KP::OnInit(GetImpl(*this)); }

        CSmartPtr(ImplicitArg p) : SP(p)
        { KP::OnInit(GetImpl(*this)); }

        CSmartPtr(CopyArg& rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
        { GetImplRef(*this) = OP::Clone(GetImplRef(rhs)); }

        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        CSmartPtr(const CSmartPtr<T1, OP1, CP1, KP1, SP1>& rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
        { GetImplRef(*this) = OP::Clone(GetImplRef(rhs)); }

        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        CSmartPtr(CSmartPtr<T1, OP1, CP1, KP1, SP1>& rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
        { GetImplRef(*this) = OP::Clone(GetImplRef(rhs)); }

        CSmartPtr(ByRef<CSmartPtr> rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
        {}
        
        operator ByRef<CSmartPtr>()
        { return ByRef<CSmartPtr>(*this); }

        CSmartPtr& operator=(CopyArg& rhs)
        {
            CSmartPtr temp(rhs);
            temp.Swap(*this);
            return *this;
        }

        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        CSmartPtr& operator=(const CSmartPtr<T1, OP1, CP1, KP1, SP1>& rhs)
        {
            CSmartPtr temp(rhs);
            temp.Swap(*this);
            return *this;
        }
        
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        CSmartPtr& operator=(CSmartPtr<T1, OP1, CP1, KP1, SP1>& rhs)
        {
            CSmartPtr temp(rhs);
            temp.Swap(*this);
            return *this;
        }
        
        void Swap(CSmartPtr& rhs)
        {
            OP::Swap(rhs);
            CP::Swap(rhs);
            KP::Swap(rhs);
            SP::Swap(rhs);
        }
        
        ~CSmartPtr()
        {
            if (OP::Release(GetImpl(*static_cast<SP*>(this))))
            {
                SP::Destroy();
            }
        }
        
        friend inline void Release(CSmartPtr& sp, typename SP::StoredType& p)
        {
            p = GetImplRef(sp);
            GetImplRef(sp) = SP::Default();
        }
        
        friend inline void Reset(CSmartPtr& sp, typename SP::StoredType p)
        { CSmartPtr(p).Swap(sp); }

        PointerType operator->()
        {
            KP::OnDereference(GetImplRef(*this));
            return SP::operator->();
        }

        PointerType operator->() const
        {
            KP::OnDereference(GetImplRef(*this));
            return SP::operator->();
        }

        ReferenceType operator*()
        {
            KP::OnDereference(GetImplRef(*this));
            return SP::operator*();
        }
        
        ReferenceType operator*() const
        {
            KP::OnDereference(GetImplRef(*this));
            return SP::operator*();
        }
        
        bool operator!() const // Enables "if (!sp) ..."
        { return GetImpl(*this) == 0; }
        

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        bool operator==(const CSmartPtr<T1, OP1, CP1, KP1, SP1>& rhs) const
        { return GetImpl(*this) == GetImpl(rhs); }

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        bool operator!=(const CSmartPtr<T1, OP1, CP1, KP1, SP1>& rhs) const
        { return !(*this == rhs); }

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        bool operator<(const CSmartPtr<T1, OP1, CP1, KP1, SP1>& rhs) const
        { return GetImpl(*this) < GetImpl(rhs); }

    private:
        // Helper for enabling 'if (sp)'
        struct Tester
        {
            Tester(int) {}
            void dummy() {}
        };
        
        typedef void (Tester::*unspecified_boolean_type_)();

        typedef typename Select<CP::allow, Tester, unspecified_boolean_type_>::Result
            unspecified_boolean_type;

    public:
        // enable 'if (sp)'
        operator unspecified_boolean_type() const
        {
            return !*this ? 0 : &Tester::dummy;
        }

    private:
        // Helper for disallowing automatic conversion
        struct Insipid
        {
            Insipid(PointerType) {}
        };
        
        typedef typename Select<CP::allow, PointerType, Insipid>::Result
            AutomaticConversionResult;
    
    public:        
        operator AutomaticConversionResult() const
        { return GetImpl(*this); }
    };

////////////////////////////////////////////////////////////////////////////////
// free comparison operators for class template CSmartPtr
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = CSmartPtr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator==(const CSmartPtr<T, OP, CP, KP, SP>& lhs,
        U* rhs)
    { return GetImpl(lhs) == rhs; }


////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = raw pointer, rhs = CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator==(U* lhs,
        const CSmartPtr<T, OP, CP, KP, SP>& rhs)
    { return rhs == lhs; }


////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = CSmartPtr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator!=(const CSmartPtr<T, OP, CP, KP, SP>& lhs,
        U* rhs)
    { return !(lhs == rhs); }


////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = raw pointer, rhs = CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator!=(U* lhs,
        const CSmartPtr<T, OP, CP, KP, SP>& rhs)
    { return rhs != lhs; }


////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = CSmartPtr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator<(const CSmartPtr<T, OP, CP, KP, SP>& lhs, U* rhs)
    { return GetImpl(lhs) < rhs; }


////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = raw pointer, rhs = CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator<(U* lhs, const CSmartPtr<T, OP, CP, KP, SP>& rhs)
    { return GetImpl(rhs) < lhs; }


////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = CSmartPtr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator>(const CSmartPtr<T, OP, CP, KP, SP>& lhs,
        U* rhs)
    { return rhs < lhs; }


////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = raw pointer, rhs = CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator>(U* lhs,
        const CSmartPtr<T, OP, CP, KP, SP>& rhs)
    { return rhs < lhs; }


////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = CSmartPtr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator<=(const CSmartPtr<T, OP, CP, KP, SP>& lhs,
        U* rhs)
    { return !(rhs < lhs); }


////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = raw pointer, rhs = CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator<=(U* lhs,
        const CSmartPtr<T, OP, CP, KP, SP>& rhs)
    { return !(rhs < lhs); }


////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = CSmartPtr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator>=(const CSmartPtr<T, OP, CP, KP, SP>& lhs,
        U* rhs)
    { return !(lhs < rhs); }


////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = raw pointer, rhs = CSmartPtr
////////////////////////////////////////////////////////////////////////////////
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator>=(U* lhs,
        const CSmartPtr<T, OP, CP, KP, SP>& rhs)
    { return !(lhs < rhs); }


////////////////////////////////////////////////////////////////////////////////
// specialization of std::less for CSmartPtr
////////////////////////////////////////////////////////////////////////////////
namespace std
{
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP
    >
    struct less< CSmartPtr<T, OP, CP, KP, SP> >
        : public binary_function<CSmartPtr<T, OP, CP, KP, SP>,
            CSmartPtr<T, OP, CP, KP, SP>, bool>
    {
        bool operator()(const CSmartPtr<T, OP, CP, KP, SP>& lhs,
            const CSmartPtr<T, OP, CP, KP, SP>& rhs) const
        { return less<T*>()(GetImpl(lhs), GetImpl(rhs)); }
    };
}

#endif //__SMART_PTR_H__
