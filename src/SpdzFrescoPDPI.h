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

#ifndef MOD_SPDZ_FRESCO_EMU_SHARED3PPDPI_H
#define MOD_SPDZ_FRESCO_EMU_SHARED3PPDPI_H

#include <sharemind/libemulator_protocols/ShareVector.h>
#include <sharemind/libemulator_protocols/ValueHeap.h>
#include "SpdzFrescoPD.h"

namespace sharemind {

class ExecutionModelEvaluator;
class ExecutionProfiler;
class SpdzFrescoConfiguration;

class __attribute__ ((visibility("internal"))) SpdzFrescoPDPI {

public: /* Methods: */

    SpdzFrescoPDPI(SpdzFrescoPD & pd);
    inline ~SpdzFrescoPDPI() noexcept {}

    inline const std::string & pdName() const noexcept
    { return m_pd.name(); }

    inline const SpdzFrescoConfiguration & pdConfiguration() const noexcept
    { return m_pd.configuration(); }

    inline ExecutionModelEvaluator & modelEvaluator() noexcept
    { return m_modelEvaluator; }

    inline const ExecutionModelEvaluator & modelEvaluator() const noexcept
    { return m_modelEvaluator; }

    inline ExecutionProfiler & profiler() noexcept
    { return m_profiler; }

    inline const ExecutionProfiler & profiler() const noexcept
    { return m_profiler; }

    template <typename T>
    inline bool isValidHandle(void * hndl) const {
        return m_heap.check<T>(hndl);
    }

    template <typename T>
    inline bool registerVector(share_vec<T> * vec) {
        return m_heap.insert(vec);
    }

    template <typename T>
    inline bool freeRegisteredVector(share_vec<T> * vec) {
        return m_heap.erase(vec);
    }

private: /* Fields: */

    SpdzFrescoPD & m_pd;
    SpdzFrescoConfiguration & m_pdConfiguration;

    ExecutionModelEvaluator & m_modelEvaluator;
    ExecutionProfiler & m_profiler;

    value_heap m_heap;

}; /* class SpdzFrescoPDPI { */

} /* namespace sharemind { */

#endif /* MOD_SPDZ_FRESCO_EMU_SHARED3PPDPI_H */
