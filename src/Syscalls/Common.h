/*
 * Copyright (C) 2015 Cybernetica
 *
 * Research/Commercial License Usage
 * Licensees holding a valid Research License or Commercial License
 * for the Software may use this file according to the written
 * agreement between you and Cybernetica.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl-3.0.html.
 *
 * For further information, please contact us at sharemind@cyber.ee.
 */

#ifndef MOD_SPDZ_FRESCO_EMU_SYSCALLS_COMMON_H
#define MOD_SPDZ_FRESCO_EMU_SYSCALLS_COMMON_H

#include <inttypes.h>
#include <sharemind/compiler-support/GccPR55015.h>
#include <sharemind/Concat.h>
#include <sharemind/ExecutionModelEvaluator.h>
#include <sharemind/ExecutionProfiler.h>
#include <sharemind/libmodapi/api_0x1.h>
#include <sstream>
#include "../ValueTraits.h"

namespace sharemind {

/**
 * Template to get stack value given shared value type.
 */
template <typename T>
inline typename T::public_type getStack(const SharemindCodeBlock & arg);

template <>
inline uint32_t getStack<sf_uint32_t>(const SharemindCodeBlock & arg)
{ return arg.uint32[0]; }
template <>
inline uint64_t getStack<sf_uint64_t>(const SharemindCodeBlock & arg)
{ return arg.uint64[0]; }

/**
 * Basic syscall parameter verification.
 */
template < const size_t NumArgs
         , const bool   NeedReturnValue = false
         , const size_t NumRefs = 0u
         , const size_t NumCRefs = 0u
         >
struct __attribute__ ((visibility("internal"))) SyscallArgs {
    template <class T>
    static inline size_t countRefs(const T * refs) {
        if (!refs)
            return 0u;

        assert(refs->pData);
        size_t r = 1u;
        while ((++refs)->pData)
            r++;
        return r;
    }

    static inline bool check(const size_t num_args,
                             const SharemindModuleApi0x1Reference * const refs,
                             const SharemindModuleApi0x1CReference * const crefs,
                             const SharemindCodeBlock * const returnValue)
    {
        if (num_args != NumArgs)
            return false;

        if (NeedReturnValue && !returnValue)
            return false;

        if (countRefs(refs) != NumRefs)
            return false;

        if (countRefs(crefs) != NumCRefs)
            return false;

        return true;
    }
};

/**
 * Virtual machine handle representation. Validates that the handle is correct.
 */
class __attribute__ ((visibility("internal"))) VMHandles
        : public SharemindModuleApi0x1PdpiInfo
{
public: /* Methods: */
    VMHandles() {
        pdpiHandle = 0;
        pdHandle = 0;
        pdkIndex = 0u;
        moduleHandle = 0;
    }


    inline bool get(SharemindModuleApi0x1SyscallContext * c,
                    SharemindCodeBlock * args, size_t index = 0)
    {
        assert(c != 0 && args != 0);

        const size_t SHARED3P_INDEX = 0u;

        const SharemindModuleApi0x1PdpiInfo * pdpiInfo =
                (*(c->get_pdpi_info))(c, args[index].uint64[0]);
        if (!pdpiInfo) {
            fprintf(stderr, "get_pd_process_instance_handle (%" PRIu64 ")\n",
                    args[index].uint64[0]);
            return false;
        }

        if (pdpiInfo->pdkIndex != SHARED3P_INDEX            // or wrong pdk is returned
            || pdpiInfo->moduleHandle != c->moduleHandle)   // or module handle pointers mismatch
        {
            return false;
        }

        assert(pdpiInfo->pdpiHandle);

        SharemindModuleApi0x1PdpiInfo::operator = (*pdpiInfo);

        return true;
    }
};

/**
 * Centralized exception handling.
 */
inline SharemindModuleApi0x1Error catchModuleApiErrors() noexcept
        __attribute__ ((visibility("internal")));

inline SharemindModuleApi0x1Error catchModuleApiErrors() noexcept {
    try {
        if (const auto eptr = std::current_exception()) {
            std::rethrow_exception(eptr);
        } else {
            return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
        }
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }
}

/**
 * Macros for defining named syscalls and their wrappers
 */
#define NAMED_SYSCALL(fname,name,args,argc,refs,crefs,retVal,c) \
    SharemindModuleApi0x1Error fname( \
            const char * name, \
            SharemindCodeBlock * args, \
            size_t argc, \
            const SharemindModuleApi0x1Reference * refs, \
            const SharemindModuleApi0x1CReference * crefs, \
            SharemindCodeBlock * retVal, \
            SharemindModuleApi0x1SyscallContext * c)

#if SHAREMIND_GCCPR55015
#define NAMED_SYSCALL_WRAPPER(name,...) \
    SharemindModuleApi0x1Error name( \
        SharemindCodeBlock * args, \
        size_t argc, \
        const SharemindModuleApi0x1Reference * refs, \
        const SharemindModuleApi0x1CReference * crefs, \
        SharemindCodeBlock * retVal, \
        SharemindModuleApi0x1SyscallContext * c) \
    { \
        return __VA_ARGS__(("spdz_fresco::" #name), args, argc, refs, crefs, retVal, c); \
    }

#define NAMED_SYSCALL_DEFINITION(signature,fptr) \
  { (signature), &(fptr) }
#else /* SHAREMIND_GCCPR55015 */
#define NAMED_SYSCALL_WRAPPER(name,...) \
    auto name = [](SharemindCodeBlock * args, \
       size_t argc, \
       const SharemindModuleApi0x1Reference * refs, \
       const SharemindModuleApi0x1CReference * crefs, \
       SharemindCodeBlock * retVal, \
       SharemindModuleApi0x1SyscallContext * c) \
            -> SharemindModuleApi0x1Error \
    { \
        return __VA_ARGS__(("spdz_fresco::" #name), args, argc, refs, crefs, retVal, c); \
    };

#define NAMED_SYSCALL_DEFINITION(signature,fptr) \
  { (signature), (fptr) }
#endif /* SHAREMIND_GCCPR55015 */


/**
 * Macros for profiling syscalls
 */
/// \todo evaluate() returns double. Make sure we can cast it to UsTime.
#ifdef SHAREMIND_NETWORK_STATISTICS_ENABLE
#define PROFILE_SYSCALL(profiler,evaluator,name,parameter) \
    do { \
        static const uint32_t sectionTypeId = \
            (profiler).newSectionType((name)); \
        sharemind::ExecutionModelEvaluator::Model * const timeModel = \
            (evaluator).model("TimeModel", (name)); \
        if (timeModel) \
            (profiler).addSection(sectionTypeId, (parameter), 0u, \
                    timeModel->evaluate((parameter)), \
                    sharemind::MinerNetworkStatistics(), \
                    sharemind::MinerNetworkStatistics()); \
    } while (false)
#else
#define PROFILE_SYSCALL(profiler,evaluator,name,parameter) \
    do { \
        static const uint32_t sectionTypeId = \
            (profiler).newSectionType((name)); \
        sharemind::ExecutionModelEvaluator::Model * const timeModel = \
            (evaluator).model("TimeModel", (name)); \
        if (timeModel) \
            (profiler).addSection(sectionTypeId, (parameter), 0u, \
                    timeModel->evaluate((parameter))); \
    } while (false)
#endif

} /* namespace sharemind */

#endif /* MOD_SPDZ_FRESCO_EMU_SYSCALLS_COMMON_H */
