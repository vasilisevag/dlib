// Copyright (C) 2005  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_AUTO_MUTEX_EXTENSIOn_
#define DLIB_AUTO_MUTEX_EXTENSIOn_

#include "threads_kernel.h"
#include "rmutex_extension.h"
#include "read_write_mutex_extension.h"
#include "auto_mutex_extension_abstract.h"

namespace dlib
{

// ----------------------------------------------------------------------------------------

    class auto_mutex
    {
        /*!
            INITIAL VALUE
                - if (m != 0) then 
                    - the mutex pointed to by m is locked
                    
            CONVENTION
                - if (m != 0) then
                    - the mutex pointed to by m is locked
        !*/
        
    public:
        explicit auto_mutex (const MUTEX& m_) : m(&m_) {
            m->lock();
        }

        void unlock() {
            m->unlock();
            m = nullptr;
        }

        ~auto_mutex () {
            unlock();
        }
        
    private:
        const MUTEX* m;
        // restricted functions
        auto_mutex(auto_mutex&);        // copy constructor
        auto_mutex& operator=(auto_mutex&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------

    class auto_mutex_readonly
    {
    public:

        explicit auto_mutex_readonly (
            const read_write_mutex& rw_
        ) : rw(rw_), _has_write_lock(false), _has_read_lock(true)
        {
            rw.lock_readonly();
        }

        ~auto_mutex_readonly (
        )
        {
            unlock();
        }

        void lock_readonly (
        )
        {
            if (!_has_read_lock)
            {
                unlock();
                rw.lock_readonly();
                _has_read_lock = true;
            }
        }

        void lock_write (
        )
        {
            if (!_has_write_lock)
            {
                unlock();
                rw.lock();
                _has_write_lock = true;
            }
        }

        void unlock (
        )
        {
            if (_has_write_lock)
            {
                rw.unlock();
                _has_write_lock = false;
            }
            else if (_has_read_lock)
            {
                rw.unlock_readonly();
                _has_read_lock = false;
            }
        }

        bool has_read_lock (
        ) { return _has_read_lock; }

        bool has_write_lock (
        ) { return _has_write_lock; }

    private:

        const read_write_mutex& rw;
        bool _has_write_lock;
        bool _has_read_lock;

        // restricted functions
        auto_mutex_readonly(auto_mutex_readonly&);        // copy constructor
        auto_mutex_readonly& operator=(auto_mutex_readonly&);    // assignment operator
    };

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_AUTO_MUTEX_EXTENSIOn_

