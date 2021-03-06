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

#ifndef MOD_SPDZ_FRESCO_EMU_SHARED3PMODULE_H
#define MOD_SPDZ_FRESCO_EMU_SHARED3PMODULE_H

#include <memory>
#include <sharemind/visibility.h>


namespace LogHard { class Logger; }

namespace sharemind {

class SHAREMIND_VISIBILITY_INTERNAL SpdzFrescoModule {

public: /* Methods: */

    SpdzFrescoModule(const LogHard::Logger & logger);

    const LogHard::Logger & logger() const noexcept
    { return m_logger; }

private:

    const LogHard::Logger & m_logger;

}; /* class SpdzFrescoModule { */

} /* namespace sharemind { */

#endif /* MOD_SPDZ_FRESCO_EMU_SHARED3PMODULE_H */
