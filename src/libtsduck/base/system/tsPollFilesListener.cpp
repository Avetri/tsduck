//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2023, Thierry Lelegard
// BSD-2-Clause license, see LICENSE.txt file or https://tsduck.io/license
//
//----------------------------------------------------------------------------

#include "tsPollFilesListener.h"

bool ts::PollFilesListener::updatePollFiles(UString& wildcard, std::chrono::milliseconds& poll_interval, std::chrono::milliseconds& min_stable_delay)
{
    return true;
}

ts::PollFilesListener::~PollFilesListener()
{
}
