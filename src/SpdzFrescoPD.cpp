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

#include <sharemind/ExecutionModelEvaluator.h>
#include <sharemind/ExecutionProfiler.h>
#include "SpdzFrescoModule.h"
#include "SpdzFrescoPD.h"


namespace sharemind {

SpdzFrescoPD::SpdzFrescoPD(const std::string & pdName,
                       const std::string & pdConfiguration,
                       SpdzFrescoModule & module)
    : m_configuration(module.logger())
    , m_name(pdName)
    , m_profiler(module.profiler())
{
    if (!m_configuration.load(pdConfiguration))
        throw ConfigurationException();

    try {
        m_modelEvaluator.reset(
                new ExecutionModelEvaluator(module.logger(),
                    m_configuration.modelEvaluatorConfiguration()));
    } catch (const ExecutionModelEvaluator::ConfigurationException &) {
        throw ConfigurationException();
    }
}

SpdzFrescoPD::~SpdzFrescoPD() noexcept = default;

} /* namespace sharemind { */
