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

#ifndef MOD_SPDZ_FRESCO_EMU_VALUETRAITS_H
#define MOD_SPDZ_FRESCO_EMU_VALUETRAITS_H

#include <cstdint>
#include <sharemind/libemulator_protocols/ValueTraits.h>
#include <type_traits>


namespace sharemind {

/*
 * Hierarchy of types.
 */
struct __attribute__ ((visibility("internal"))) numeric_value_tag : public any_value_tag { };
struct __attribute__ ((visibility("internal"))) unsigned_value_tag : public numeric_value_tag { };

template <typename T>
struct __attribute__ ((visibility("internal"))) is_unsigned_value_tag :
    std::is_base_of<unsigned_value_tag, typename value_traits<T>::value_category>
{ };

template <typename T>
struct __attribute__ ((visibility("internal"))) is_integral_value_tag :
    std::is_base_of<numeric_value_tag, typename value_traits<T>::value_category>
{ };

struct __attribute__ ((visibility("internal"))) sf_uint32_t {
    using value_category = unsigned_value_tag;
    using share_type = uint32_t;
    using public_type = uint32_t;
    static constexpr uint8_t heap_type_id = 0x1u;
    static constexpr size_t num_of_bits = 32u;
    static constexpr size_t log_of_bits = 5u;
};

struct __attribute__ ((visibility("internal"))) sf_uint64_t {
    using value_category = unsigned_value_tag;
    using share_type = uint64_t;
    using public_type = uint64_t;
    static constexpr uint8_t heap_type_id = 0x2u;
    static constexpr size_t num_of_bits = 64u;
    static constexpr size_t log_of_bits = 6u;
};

} /* namespace sharemind */

#endif /* MOD_SPDZ_FRESCO_EMU_VALUETRAITS_H */
