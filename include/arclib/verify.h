// =======================================================================================
// 
// A Really Cool Library - arclib
// Copyright (C) 2026 Patrick Chen <pchen.2215@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// =======================================================================================

#ifndef __ARCLIB_VERIFY_H
#define __ARCLIB_VERIFY_H

#include "int.h"
#include <string>
#include <source_location>
#include <stdexcept>

namespace arcl {

// =======================================================================================
// VERIFICATION FUNCTIONALITY
// =======================================================================================

    class verification_error: public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    /// <summary>
    /// Checks if an expression is true and throws an arcl::verification_failure exception
    /// otherwise.
    /// </summary>
    /// <param name="val">The expression.</param>
    /// <param name="msg">The message.</param>
    /// <param name="loc">The callsite. The caller should not attempt to pass a
    /// non-default argument for this parameter.</param>
    void verify(bool val, std::string msg = "",
                std::source_location loc = std::source_location::current()) {
        if (val) { return; }

        // Get callsite information
        std::string filename = loc.file_name();
        std::string function = loc.function_name();
        uint64 line = loc.line();
        uint64 col = loc.column();

        // Print error message and throw exception
        msg = "ARCLIB VERIFICATION FAILED: " + msg + '\n' +
            "  File: " + filename + " (" + std::to_string(line) + ':' +
            std::to_string(col) + ")\n  Function: " + function + '\n';
        throw verification_error(msg);
        return;
    }

}

// =======================================================================================
#endif
