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

#include <cassert>
#include <LogHard/Logger.h>
#include <sharemind/compiler-support/GccIsNothrowDestructible.h>
#include <sharemind/ExecutionProfiler.h>
#include <sharemind/libemulator_protocols/Binary.h>
#include <sharemind/libemulator_protocols/Nullary.h>
#include <sharemind/libemulator_protocols/Ternary.h>
#include <sharemind/libemulator_protocols/Unary.h>
#include <sharemind/libmodapi/api_0x1.h>
#include "SpdzFrescoModule.h"
#include "SpdzFrescoPDPI.h"
#include "Syscalls/Common.h"
#include "Syscalls/CoreSyscalls.h"
#include "Syscalls/Meta.h"


namespace {

using namespace sharemind;

SHAREMIND_MODULE_API_0x1_SYSCALL(get_domain_name,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (!SyscallArgs<1u, true, 0u, 0u>::check(num_args, refs, crefs, returnValue))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    VMHandles handles;
    if (!handles.get(c, args))
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;

    try {
        SpdzFrescoPDPI * const pdpi = static_cast<SpdzFrescoPDPI*>(handles.pdpiHandle);
        const std::string & pdName = pdpi->pdName();

        const uint64_t mem_size = pdName.size() + 1u;
        const uint64_t mem_hndl = (* c->publicAlloc)(c, mem_size);
        char * const mem_ptr = static_cast<char *>((* c->publicMemPtrData)(c, mem_hndl));
        strncpy(mem_ptr, pdName.c_str(), mem_size);
        returnValue[0u].uint64[0u] = mem_hndl;

        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (...) {
        return catchModuleApiErrors ();
    }
}

} // anonymous namespace


extern "C" {

SHAREMIND_MODULE_API_MODULE_INFO("spdz_fresco",
                                 0x00010000,   /* Version 0.1.0.0 */
                                 0x1);         /* Support API version 1. */

SHAREMIND_MODULE_API_0x1_INITIALIZER(c) __attribute__ ((visibility("default")));
SHAREMIND_MODULE_API_0x1_INITIALIZER(c) {
    assert(c);

    const SharemindModuleApi0x1Facility * const flogger =
            c->getModuleFacility(c, "Logger");

    if (!flogger || !flogger->facility)
        return SHAREMIND_MODULE_API_0x1_MISSING_FACILITY;

    const SharemindModuleApi0x1Facility * const fprofiler =
            c->getModuleFacility(c, "Profiler");

    if (!fprofiler || !fprofiler->facility)
        return SHAREMIND_MODULE_API_0x1_MISSING_FACILITY;

    const LogHard::Logger & logger =
        *static_cast<LogHard::Logger *>(flogger->facility);

    sharemind::ExecutionProfiler & profiler =
        *static_cast<sharemind::ExecutionProfiler *>(fprofiler->facility);

    /*
     Initialize the module handle
    */
    try {
        c->moduleHandle = new sharemind::SpdzFrescoModule(logger, profiler);
        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (...) {
        return catchModuleApiErrors ();
    }
}

SHAREMIND_MODULE_API_0x1_DEINITIALIZER(c) __attribute__ ((visibility("default")));
SHAREMIND_MODULE_API_0x1_DEINITIALIZER(c) {
    assert(c);
    assert(c->moduleHandle);

    static_assert(std::is_nothrow_destructible<sharemind::SpdzFrescoModule>::value,
            "SpdzFrescoModule is not noexcept!");
    delete static_cast<sharemind::SpdzFrescoModule *>(c->moduleHandle);
    #ifndef NDEBUG
    c->moduleHandle = nullptr; // Not needed, but may help debugging.
    #endif
}


/*
 * Define wrappers for named syscalls
 */
NAMED_SYSCALL_WRAPPER(new_uint32_vec, new_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(new_uint64_vec, new_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(init_uint32_vec, init_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(init_uint64_vec, init_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(set_shares_uint32_vec, set_shares<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(set_shares_uint64_vec, set_shares<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(get_shares_uint32_vec, get_shares<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(get_shares_uint64_vec, get_shares<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(fill_uint32_vec, fill_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(fill_uint64_vec, fill_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(assign_uint32_vec, assign_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(assign_uint64_vec, assign_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(delete_uint32_vec, delete_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(delete_uint64_vec, delete_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(load_uint32_vec, load_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(load_uint64_vec, load_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(store_uint32_vec, store_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(store_uint64_vec, store_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(classify_uint32_vec, classify_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(classify_uint64_vec, classify_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(declassify_uint32_vec, declassify_vec<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(declassify_uint64_vec, declassify_vec<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(get_type_size_uint32, get_type_size<sf_uint32_t>)
NAMED_SYSCALL_WRAPPER(get_type_size_uint64, get_type_size<sf_uint64_t>)
NAMED_SYSCALL_WRAPPER(add_uint32_vec, binary_arith_vec<sf_uint32_t, AdditionProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(add_uint64_vec, binary_arith_vec<sf_uint64_t, AdditionProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(sub_uint32_vec, binary_arith_vec<sf_uint32_t, SubtractionProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(sub_uint64_vec, binary_arith_vec<sf_uint64_t, SubtractionProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(mul_uint32_vec, binary_arith_vec<sf_uint32_t, MultiplicationProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(mul_uint64_vec, binary_arith_vec<sf_uint64_t, MultiplicationProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(eq_uint32_vec, binary_vec<sf_uint32_t, sf_uint32_t, sf_uint32_t, EqualityProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(eq_uint64_vec, binary_vec<sf_uint64_t, sf_uint64_t, sf_uint64_t, EqualityProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(gt_uint32_vec, binary_vec<sf_uint32_t, sf_uint32_t, sf_uint32_t, GreaterThanProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(gt_uint64_vec, binary_vec<sf_uint64_t, sf_uint64_t, sf_uint64_t, GreaterThanProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(gte_uint32_vec, binary_vec<sf_uint32_t, sf_uint32_t, sf_uint32_t, GreaterThanOrEqualProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(gte_uint64_vec, binary_vec<sf_uint64_t, sf_uint64_t, sf_uint64_t, GreaterThanOrEqualProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(lt_uint32_vec, binary_vec<sf_uint32_t, sf_uint32_t, sf_uint32_t, LessThanProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(lt_uint64_vec, binary_vec<sf_uint64_t, sf_uint64_t, sf_uint64_t, LessThanProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(lte_uint32_vec, binary_vec<sf_uint32_t, sf_uint32_t, sf_uint32_t, LessThanOrEqualProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(lte_uint64_vec, binary_vec<sf_uint64_t, sf_uint64_t, sf_uint64_t, LessThanOrEqualProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(conv_uint64_to_uint32_vec, unary_vec<sf_uint64_t, sf_uint32_t, ConversionProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(conv_uint32_to_uint64_vec, unary_vec<sf_uint32_t, sf_uint64_t, ConversionProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(choose_uint32_vec, ternary_vec<sf_uint32_t, sf_uint32_t, sf_uint32_t, sf_uint32_t, ObliviousChoiceProtocol<SpdzFrescoPDPI>>)
NAMED_SYSCALL_WRAPPER(choose_uint64_vec, ternary_vec<sf_uint64_t, sf_uint64_t, sf_uint64_t, sf_uint64_t, ObliviousChoiceProtocol<SpdzFrescoPDPI>>)


SHAREMIND_MODULE_API_0x1_SYSCALL_DEFINITIONS(

  /**
   *  Shared unsigned integers
   */

   // Variable management
    NAMED_SYSCALL_DEFINITION("spdz_fresco::new_uint32_vec", new_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::new_uint64_vec", new_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::init_uint32_vec", init_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::init_uint64_vec", init_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::set_shares_uint32_vec", set_shares_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::set_shares_uint64_vec", set_shares_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::get_shares_uint32_vec", get_shares_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::get_shares_uint64_vec", get_shares_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::fill_uint32_vec", fill_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::fill_uint64_vec", fill_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::assign_uint32_vec", assign_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::assign_uint64_vec", assign_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::delete_uint32_vec", delete_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::delete_uint64_vec", delete_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::load_uint32_vec", load_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::load_uint64_vec", load_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::store_uint32_vec", store_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::store_uint64_vec", store_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::classify_uint32_vec", classify_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::classify_uint64_vec", classify_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::declassify_uint32_vec", declassify_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::declassify_uint64_vec", declassify_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::get_type_size_uint32", get_type_size_uint32)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::get_type_size_uint64", get_type_size_uint64)

    // Unsigned integer arithmetic
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::add_uint32_vec", add_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::add_uint64_vec", add_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::sub_uint32_vec", sub_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::sub_uint64_vec", sub_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::mul_uint32_vec", mul_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::mul_uint64_vec", mul_uint64_vec)

    // Comparisons
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::eq_uint32_vec", eq_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::eq_uint64_vec", eq_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::gt_uint32_vec", gt_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::gt_uint64_vec", gt_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::gte_uint32_vec", gte_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::gte_uint64_vec", gte_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::lt_uint32_vec", lt_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::lt_uint64_vec", lt_uint64_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::lte_uint32_vec", lte_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::lte_uint64_vec", lte_uint64_vec)

    // Casting
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::conv_uint64_to_uint32_vec", conv_uint64_to_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::conv_uint32_to_uint64_vec", conv_uint32_to_uint64_vec)

    // Special functions
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::choose_uint32_vec", choose_uint32_vec)
  , NAMED_SYSCALL_DEFINITION("spdz_fresco::choose_uint64_vec", choose_uint64_vec)

  /**
   *  Other functions
   */

  , { "spdz_fresco::get_domain_name", get_domain_name }
);


SHAREMIND_MODULE_API_0x1_PD_STARTUP(spdz_fresco_emu_startup, w) __attribute__ ((visibility("hidden")));
SHAREMIND_MODULE_API_0x1_PD_STARTUP(spdz_fresco_emu_startup, w) {
    assert(w);
    assert(w->moduleHandle);
    assert(w->conf);
    assert(w->conf->pd_name);
    assert(w->conf->pd_conf_string);

    sharemind::SpdzFrescoModule * const m =
        static_cast<sharemind::SpdzFrescoModule*>(w->moduleHandle);

    try {
        w->pdHandle = new SpdzFrescoPD(w->conf->pd_name,
                                     w->conf->pd_conf_string,
                                     *m);
        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (const SpdzFrescoPD::ConfigurationException & e) {
        m->logger().error() << "Error on protection domain '"
            << w->conf->pd_name << "' startup: " << e.what();
        return SHAREMIND_MODULE_API_0x1_INVALID_PD_CONFIGURATION;
    } catch (...) {
        return catchModuleApiErrors ();
    }
}

SHAREMIND_MODULE_API_0x1_PD_SHUTDOWN(spdz_fresco_emu_shutdown, w) __attribute__ ((visibility("hidden")));
SHAREMIND_MODULE_API_0x1_PD_SHUTDOWN(spdz_fresco_emu_shutdown, w) {
    assert(w);
    assert(w->pdHandle);
    assert(w->moduleHandle);

    static_assert(std::is_nothrow_destructible<sharemind::SpdzFrescoPD>::value,
            "SpdzFrescoPD is not noexcept!");
    delete static_cast<sharemind::SpdzFrescoPD *>(w->pdHandle);
    #ifndef NDEBUG
    w->pdHandle = nullptr; // Not needed, but may help debugging.
    #endif
}

SHAREMIND_MODULE_API_0x1_PDPI_STARTUP(spdz_fresco_emu_PDPI_startup, w) __attribute__ ((visibility("hidden")));
SHAREMIND_MODULE_API_0x1_PDPI_STARTUP(spdz_fresco_emu_PDPI_startup, w) {
    assert(w);
    assert(w->pdHandle);

    try {
        sharemind::SpdzFrescoPD * const pd =
            static_cast<sharemind::SpdzFrescoPD*>(w->pdHandle);
        w->pdProcessHandle = new SpdzFrescoPDPI(*pd);
        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (...) {
        return catchModuleApiErrors ();
    }
}

SHAREMIND_MODULE_API_0x1_PDPI_SHUTDOWN(spdz_fresco_emu_PDPI_shutdown, w) __attribute__ ((visibility("hidden")));
SHAREMIND_MODULE_API_0x1_PDPI_SHUTDOWN(spdz_fresco_emu_PDPI_shutdown, w) {
    assert(w);
    assert(w->pdHandle);
    assert(w->pdProcessHandle);

    static_assert(std::is_nothrow_destructible<sharemind::SpdzFrescoPDPI>::value,
            "SpdzFrescoPDPI is not noexcept");
    delete static_cast<sharemind::SpdzFrescoPDPI *>(w->pdProcessHandle);
    #ifndef NDEBUG
    w->pdProcessHandle = nullptr; // Not needed, but may help debugging.
    #endif
}

SHAREMIND_MODULE_API_0x1_PDK_DEFINITIONS(
    {
        "spdz_fresco",
        &spdz_fresco_emu_startup,
        &spdz_fresco_emu_shutdown,
        &spdz_fresco_emu_PDPI_startup,
        &spdz_fresco_emu_PDPI_shutdown
    }
);

} // extern "C" {
